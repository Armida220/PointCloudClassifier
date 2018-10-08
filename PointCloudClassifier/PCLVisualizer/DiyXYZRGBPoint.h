#pragma once

///PCL  ���ƿ�
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>
#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/console/parse.h>
#include <pcl/common/centroid.h>  //��������

///OpenCV ��

/**
* @brief �����ռ�ļ򵥸��� \n(����)
* �����ռ����ϸ����
*/

namespace LY{
	struct XYZRGBCloud
	{
		QString name;
		QString path;
		pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud;

	};


	enum THREAD_TYPE{
		ToolPression, 
		ToolCluster, 
		ToolKdTreeK, 
		ToolKdTreeRadius,
		
		FilterPassThrough,
		FilterVoxelGrid, 
		FilterStaticalOutlinerRemoval, 
		FilterProjection, 
		FilterGray2Image,
		FilterVegetation,
		
		ConvertLas2Txt, 
		ConvertTxt2Pcd, 
		ConvertPcd2Pcd, 
		ConvertPcd2Txt,
		
		LasFix, 
		LasKdTree, 
		LasBinarySignal,
		LasImage2Cloud
	};
}
