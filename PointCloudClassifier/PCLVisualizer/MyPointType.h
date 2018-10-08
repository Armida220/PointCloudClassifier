#define PCL_NO_PRECOMPILE    ///��仰����Ҫ

#ifndef MYPOINTTYPE_H
#define MYPOINTTYPE_H

#include <pcl/point_types.h>

struct XYZ_IRHRGBBinary
{
	PCL_ADD_POINT4D;
	float intensity;
	float returnsNum;
	float height;
	float r;
	float g;
	float b;
	float bin;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
}EIGEN_ALIGN16;


POINT_CLOUD_REGISTER_POINT_STRUCT(XYZ_IRHRGBBinary,// ע������ͺ�
	(float, x, x)
	(float, y, y)
	(float, z, z)
	(float, intensity, intensity)
	(float, returnsNum, returnsNum)
	(float, height, height)
	(float, r, r)
	(float, g, g)
	(float, b, b)
	(float, bin, bin)
	)


#endif