//
// Created by Kanari on 2016/6/21.
//

#ifndef RENDERER_UTILITY_H
#define RENDERER_UTILITY_H

#include "Common.h"
#include <random>

// Utility functions

class Random {
	static inline std::mt19937 &generator() {
		static std::random_device rd;
		static std::mt19937 generator(rd());
		return generator;
	}

public:
	static inline double uniform_pm1() {
		static std::uniform_real_distribution<> dis(-1, 1);
		return dis(generator());
	}

	static inline double uniform_01() {
		static std::uniform_real_distribution<> dis(0, 1);
		return dis(generator());
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

inline double distsqr(const Point &a, const Point &b) {
	double ret = sqr(a.val[0] - b.val[0]) +
				 sqr(a.val[1] - b.val[1]) +
				 sqr(a.val[2] - b.val[2]);
	return ret;
}

inline double dist(const Point &a, const Point &b) {
	double ret = distsqr(a, b);
	return sqrt(ret);
}

// Returns whether a is chosen
inline bool roulette(const double &a, const double &b) {
	double p = a / (a + b);
	double x = Random::uniform_01();
	return x <= p;
}

#endif //RENDERER_UTILITY_H
