//
// Created by Kanari on 2016/6/23.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include "Utility.h"
#include "Ray.h"
#include "Object.h"
#include "Canvas.h"
#include "Light.h"
#include <vector>

class Scene {
	Canvas m_canvas;
	std::vector<Object *> m_obj;

	std::vector<Light *> m_lights;

	Point m_camera, m_lookat;
	double m_field_length;

public:
	Scene(int _width, int _height) : m_canvas(_width, _height) { }

	inline void addObject(Object *obj) {
		m_obj.push_back(obj);
	}

	inline void addLight(Light *light) {
		m_lights.push_back(light);
	}

	inline const decltype(m_lights) &lights() const {
		return m_lights;
	}

	inline void display() {
		m_canvas.show();
	}

	bool findIntersection(const Ray &ray, IntersectData *data) const;

	inline void setView(const Point &camera, const Point &lookat,
						const double &field_length) {
		m_camera = camera;
		m_lookat = lookat;
		m_field_length = field_length;
	}

	Color trace(const Ray &ray, int depth) const;

	void render();
};


#endif //RENDERER_SCENE_H
