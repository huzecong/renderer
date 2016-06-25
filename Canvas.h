//
// Created by Kanari on 2016/6/21.
//

#ifndef RENDERER_CANVAS_H
#define RENDERER_CANVAS_H

#include "Utility.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

class Canvas {
	int m_width, m_height;
	Mat canvas;
/*
	std::thread *m_thread;
	bool window_should_open;
	std::mutex m_mutex;

	inline void displayImage(int wait_time) {
		int cnt = 0;
		cv::namedWindow("Canvas");
		while (window_should_open) {
			std::cerr << "display " << cnt++ << std::endl;
			std::unique_lock<std::mutex> lock(m_mutex);
			cv::imshow("Canvas", canvas);
			lock.unlock();
			cv::waitKey(wait_time);
		}
	}
*/
public:
	Canvas(int _width, int _height) : canvas(_height, _width, CV_32FC3) {
		m_width = _width;
		m_height = _height;
//		m_thread = NULL;
//		window_should_open = false;
	}

	inline const int width() const {
		return m_width;
	}

	inline const int height() const {
		return m_height;
	}

	void show() {
		cv::imshow("Canvas", canvas);
		cv::waitKey(0);
	}
/*
	template <class Rep, class Period>
	inline void show(std::chrono::duration<Rep, Period> duration) {
		if (m_thread == NULL) {
			auto wait_time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
			window_should_open = true;
			m_thread = new std::thread(&Canvas::displayImage, this, wait_time);
		}
	}

	inline void closeWindow() {
		if (m_thread != NULL) {
			window_should_open = false;
			m_thread->join();
			delete m_thread;
			m_thread = NULL;
		}
	}
*/
	inline void setColor(int x, int y, const Vec3 &col) {
//		std::lock_guard<std::mutex> lock(m_mutex);
		canvas.at<Vec3>(x, y) = col;
	}
};

#endif //RENDERER_CANVAS_H
