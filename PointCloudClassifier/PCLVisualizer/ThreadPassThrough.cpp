///##############################################################
///		Author: Neverland_LY     Date:2018-7-9 22:56
///
///		�ⲿ�ִ���ʵ���˵���ֱͨ�˲�����
///		ֱͨ�˲���Ϊɸѡ��ĳһ�ֶ�һ����Χ�ڵĵ���
///		����ɸѡ���ĵ��ƽ��б��������޳�����
///
///##############################################################


#include "ThreadPassThrough.h"

ThreadPassThrough::ThreadPassThrough(QObject *parent)
	: QObject(parent)
{
}

ThreadPassThrough::~ThreadPassThrough()
{
}

void ThreadPassThrough::runTask(){

	///���Ĺ��ܵ�ʵ���ܳ���
	pcl::PassThrough<pcl::PointXYZRGB> pass;
	pass.setInputCloud(cloud->cloud);
	pass.setFilterFieldName(filed);
	pass.setFilterLimits(dMin, dMax);
	if (passType == "DELETE")
		pass.setNegative(false);
	else
		pass.setFilterLimitsNegative(true);

	LY::XYZRGBCloud * cloud_result;
	pass.filter(*(cloud_result->cloud));

	//emit finishTask(cloud_result);;
}
