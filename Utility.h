//
// Created by Kanari on 2016/6/21.
//

#ifndef RENDERER_UTILITY_H
#define RENDERER_UTILITY_H

#include "Common.h"
#include <random>
#include <ctime>
#include <thread>

// Utility functions

class Random {
public:
	static inline double uniform_pm1() {
#ifdef __APPLE__
		static std::mt19937 *generator = nullptr;
#else
		static thread_local std::mt19937 *generator = nullptr;
#endif
		if (!generator)
			generator = new std::mt19937(clock() + std::hash<std::thread::id>()(
					std::this_thread::get_id()));
		std::uniform_real_distribution<> dis(-1, 1);
		return dis(*generator);
	}

	static inline double uniform_01() {
#ifdef __APPLE__
		static std::mt19937 *generator = nullptr;
#else
		static thread_local std::mt19937 *generator = nullptr;
#endif
		if (!generator)
			generator = new std::mt19937(clock() + std::hash<std::thread::id>()(
					std::this_thread::get_id()));
		std::uniform_real_distribution<> dis(0, 1);
		return dis(*generator);
	}

	static inline Vec3 vector() {
		return Vec3{(float)Random::uniform_01(),
		            (float)Random::uniform_01(),
		            (float)Random::uniform_01()};
	}
};

template <class T>
inline T clamp(const T &val, const T &l, const T &r) {
	return std::max(l, std::min(r, val));
}

template <class T>
inline T sqr(const T &x) {
	return x * x;
}

inline double length2(const Vec3 &a) {
	double ret = sqr(a.val[0]) + sqr(a.val[1]) + sqr(a.val[2]);
	return ret;
}

inline double length(const Vec3 &a) {
	double ret = length2(a);
	return sqrt(ret);
}

// Returns whether a is chosen
inline bool roulette(const double &a, const double &b) {
	double p = a / (a + b);
	double x = Random::uniform_01();
	return x <= p;
}

inline Vec3::value_type max(const Vec3 &a) {
	return std::max(a.val[0], std::max(a.val[1], a.val[2]));
}

#endif //RENDERER_UTILITY_H
