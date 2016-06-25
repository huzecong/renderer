//
// Created by Kanari on 2016/6/24.
//

#ifndef RENDERER_COMMON_H
#define RENDERER_COMMON_H

#include <opencv2/opencv.hpp>

typedef unsigned int uint;

// Shorthands
typedef cv::Vec3f Vec3;
using cv::Vec4f;
using cv::Mat;

// Aliases
typedef Vec3 Point;
typedef Vec3 Color;

// Constants
const int kIters = 10;
const double kEps = 1e-4;
const int kMaxDepth = 3;

namespace kColor {
	const Color Black(0.0, 0.0, 0.0);
	const Color Red(0.0, 0.0, 1.0);
	const Color Green(0.0, 1.0, 0.0);
	const Color Blue(1.0, 0.0, 0.0);
	const Color White(1.0, 1.0, 1.0);
	const Color Grey(0.5, 0.5, 0.5);
}
const Vec3 Zero(0, 0, 0);
const Vec3 One(1, 1, 1);
const Color kBackgroundColor(kColor::Black);

#define is_normalized(x) [&x]() { auto _n = x - cv::normalize(x); return _n.dot(_n) < kEps; }()

#endif //RENDERER_COMMON_H
