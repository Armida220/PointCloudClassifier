///##############################################################
///		Author: Neverland_LY     Date:2018-7-9 22:56
///
///		�ⲿ�ִ���ʵ���˵���ֱͨ�˲�����
///		ֱͨ�˲���Ϊɸѡ��ĳһ�ֶ�һ����Χ�ڵĵ���
///		����ɸѡ���ĵ��ƽ��б��������޳�����
///
///##############################################################


#pragma once

#include <QObject>
#include "DiyXYZRGBPoint.h"

#include <pcl/filters/passthrough.h>  //ֱͨ�˲�
#include <pcl/point_types.h>

class ThreadPassThrough : public QObject
{
	Q_OBJECT

public:
	ThreadPassThrough(QObject *parent = 0);
	~ThreadPassThrough();

	void runTask();

	std::string filed;
	std::string passType;
	float dMin;
	float dMax;
	LY::XYZRGBCloud * cloud;

signals:
	void finishTask(pcl::PointCloud<pcl::PointXYZRGB>::Ptr);

};
