//
// Created by Kanari on 2016/6/23.
//

#include "Scene.h"
#include "Material.h"
#include <iomanip>

using namespace std;

struct PixelQueue {
	int row, col, n_rows, n_cols;
	mutex m;

	inline void init(int rows, int cols) {
		lock_guard<mutex> lock(m);
		n_rows = rows, n_cols = cols;
		row = 0, col = -1;
	}

	inline pair<int, int> next() {
		lock_guard<mutex> lock(m);
		if (row == -1 && col == -1)
			return make_pair(-1, -1);
		if (++col == n_cols) {
			col = 0;
			if (++row == n_rows)
				row = col = -1;
		}
		return make_pair(row, col);
	}
} pixel_queue;

mutex cerr_mutex;

void Scene::render_threaded(int pid, int iteration, PixelQueue *pixel_queue) {
	{
		lock_guard<mutex> lock(cerr_mutex);
		cerr << "Starting iteration " << iteration << " of thread " << pid <<
		endl;
	}
	while (true) {
		int i, j;
		tie(i, j) = pixel_queue->next();
		if (i == -1 && j == -1) return;

		double ver_bias = (i - m_canvas.height() / 2) * m_viewport_unit;
		double hor_bias = (j - m_canvas.width() / 2) * m_viewport_unit;
		Color col;

		for (int k = 0; k < kSamplesPerIter; ++k) {
			double vb = ver_bias +
						Random::uniform_pm1() * m_viewport_unit / 2;
			double hb = hor_bias +
						Random::uniform_pm1() * m_viewport_unit / 2;
			Vec3 target = m_lookat + vb * m_viewport_y + hb * m_viewport_x;
			Ray ray(target, target - m_camera);
			col += trace(ray, 0);
		}
		col /= kSamplesPerIter;

		col += m_canvas.getColor(i, j) * iteration;
		col /= iteration + 1;
		m_canvas.setColor(i, j, col);
	}
}

void Scene::render(bool multithread) {
	auto start_time = chrono::steady_clock::now();

	Vec3 y_axis(0.0, 1.0, 0.0);
	Vec3 view_ray = cv::normalize(m_lookat - m_camera);
	Vec3 right = view_ray.cross(y_axis);
	Vec3 up = view_ray.cross(right);
	m_viewport_x = right;
	m_viewport_y = up;
	m_viewport_unit = m_field_length / m_canvas.width();

	m_canvas.clear();

	cerr << fixed << setprecision(3);

	if (multithread) {
		for (int iter = 0; iter < kIters; ++iter) {
			pixel_queue.init(m_canvas.height(), m_canvas.width());

			thread *threads[kThreads - 1];
			for (int i = 0; i < kThreads - 1; ++i)
				threads[i] = new thread(&Scene::render_threaded,
										this, i + 1, iter, &pixel_queue);

			render_threaded(0, iter, &pixel_queue);

			for (int i = 0; i < kThreads - 1; ++i)
				threads[i]->join();

			char path[100];
			sprintf(path,
					"/Users/kanari/CLionProjects/renderer/output/iter%03d.png",
					iter);
			m_canvas.save(path);

			cerr << "Finished iteration " << iter << ", image written to " <<
			path << endl;
			double ms = chrono::duration_cast<chrono::milliseconds>(
					chrono::steady_clock::now() - start_time).count();
			cerr << "Time consumed: " << ms / 1000 << endl;

//			m_canvas.show();
		}
	} else {
		for (int i = 0; i < m_canvas.height(); ++i)
			for (int j = 0; j < m_canvas.width(); ++j) {
				double ver_bias =
						(i - m_canvas.height() / 2) * m_viewport_unit;
				double hor_bias =
						(j - m_canvas.width() / 2) * m_viewport_unit;
				Color col;

				for (int k = 0; k < kSamplesPerIter; ++k) {
					double vb = ver_bias +
								Random::uniform_pm1() * m_viewport_unit /
								2;
					double hb = hor_bias +
								Random::uniform_pm1() * m_viewport_unit /
								2;
					Vec3 target =
							m_lookat + vb * m_viewport_y + hb * m_viewport_x;
					Ray ray(target, target - m_camera);
					col += trace(ray, 0);
				}
				col /= kSamplesPerIter;

				m_canvas.setColor(i, j, col);
			}
	}
}

Color Scene::trace(const Ray &ray, int depth) const {
	if (depth > kMaxDepth) return kColor::Black;
	IntersectData data;
	if (!findIntersection(ray, &data)) {
		return kBackgroundColor;
	}

//	return hit_obj->material()->emissiveColor();
	return data.hit_obj->material()->calculateColor(ray, data, depth, *this);
}

bool Scene::castShadowRay(const Ray &ray, IntersectData *data) const {
	IntersectData best;
	best.dist = LLONG_MAX;
	best.hit_obj = nullptr;
	for (Object *obj : m_obj) {
		IntersectData current;
		bool found = obj->intersect(ray, current);
		if (found && current.hit_obj->material()->type() != MTDielectric &&
			current.dist < best.dist)
			best = current;
	}
	if (best.dist == LLONG_MAX) return false;
	else {
		if (data != nullptr) *data = best;
		return true;
	}
}

bool Scene::findIntersection(const Ray &ray, IntersectData *data) const {
	IntersectData best;
	best.dist = LLONG_MAX;
	best.hit_obj = nullptr;
	for (Object *obj : m_obj) {
		IntersectData current;
		bool found = obj->intersect(ray, current);
		if (found && current.dist < best.dist)
			best = current;
	}
	if (best.dist == LLONG_MAX) return false;
	else {
		if (data != nullptr) *data = best;
		return true;
	}
}
