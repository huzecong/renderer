//
// Created by Kanari on 2016/6/23.
//

#include "Scene.h"
#include "Material.h"
#include <algorithm>
using namespace std;

void Scene::render() {
	Vec3 y_axis(0.0, 1.0, 0.0);
	Vec3 view_ray = cv::normalize(m_lookat - m_camera);
	Vec3 right = view_ray.cross(y_axis);
	Vec3 up = view_ray.cross(right);
	double unit_length = m_field_length / m_canvas.width();

	for (int i = 0; i < m_canvas.height(); ++i)
		for (int j = 0; j < m_canvas.width(); ++j) {
			double ver_bias = (i - m_canvas.height() / 2) * unit_length;
			double hor_bias = (j - m_canvas.width() / 2) * unit_length;
			Color col;

			if (j == 314 && i == 166) {
				int a = 0;
				++a;
			}

			for (int k = 0; k < kIters; ++k) {
				double vb = ver_bias + Random::uniform_pm1() * unit_length / 2;
				double hb = hor_bias + Random::uniform_pm1() * unit_length / 2;
				Vec3 target = m_lookat + vb * up + hb * right;
				Ray ray(target, target - m_camera);
				col += trace(ray, 0);
			}
			col /= kIters;
			m_canvas.setColor(i, j, col);
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
