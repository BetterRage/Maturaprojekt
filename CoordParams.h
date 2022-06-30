#pragma once
#include "opencv2/core.hpp"
#include <vector>
class CoordParams
{
public:
	static __int64 SerialDataFromPoints(std::vector<cv::Point2f> points, unsigned char* buf, unsigned int size);
};

