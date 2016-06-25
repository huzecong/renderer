//
// Created by Kanari on 2016/6/24.
//

#ifndef RENDERER_LIGHT_H
#define RENDERER_LIGHT_H

#include "Utility.h"

class Light {
protected:
	Color col;
public:
	Light(const Color &_color) : col(_color) { }

	inline Color color(const double &dist) const {
		return col / sqr(dist);
	}

	virtual Point sample() = 0;
};

class PointLight : public Light {
protected:
	Point p;
public:
	PointLight(const Point &_p, const Color &_color = kColor::White)
			: p(_p), Light(_color) { }

	inline Point sample() {
		return p;
	}
};

class AreaLight : public Light {
protected:
	Vec3 axis[2];
	Point p;
public:
	AreaLight(const Point &_p, const Vec3 &x_axis, const Vec3 &y_axis,
			  const Color &_color = kColor::White)
			: p(_p), axis{x_axis, y_axis}, Light(_color) { }

	inline Point sample() {
		return p + Random::uniform_01() * axis[0] +
			   Random::uniform_01() * axis[1];
	}
};

#endif //RENDERER_LIGHT_H
