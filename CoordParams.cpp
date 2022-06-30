#include "CoordParams.h"

__int64 CoordParams::SerialDataFromPoints(std::vector<cv::Point2f> points,unsigned char* buf, unsigned int size)
{
	unsigned char* begin = buf;
	if (size < 2*points.size()+1 || ((size & 1) == 0))
		return 0;
	*(buf++) = (unsigned char)points.size();
	for (auto& i : points)
	{
		*(buf++) = (unsigned char) i.x;
		*(buf++) = (unsigned char) i.y;
	}
	return buf - begin;
}
