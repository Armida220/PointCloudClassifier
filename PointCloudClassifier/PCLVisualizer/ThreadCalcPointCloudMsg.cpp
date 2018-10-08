#include "ThreadCalcPointCloudMsg.h"
#include <QThread>
#include <QDebug>

ThreadCalcPointCloudMsg::ThreadCalcPointCloudMsg(QObject *parent)
: QThread(parent)
{
	cloud = nullptr;
	std::cout << ">>> ThreadCalcPointCloudMsg construct ...\n";
}

ThreadCalcPointCloudMsg::~ThreadCalcPointCloudMsg()
{
	delete cloud;
	std::cout << ">>> ThreadCalcPointCloudMsg destruct ...\n";
}

void ThreadCalcPointCloudMsg::run(){

	///��¼�����Ϣ
	std::vector<QString> vec;

	///��¼���Ƶ�ͷ0������1����С2������3
	vec.push_back("  ");  ///ռλ��
	vec.push_back(cloud->name);
	vec.push_back("1");
	vec.push_back(QString("%1").arg(cloud->cloud->points.size()));

	///��¼��С��4������5
	pcl::PointXYZRGB minPt, maxPt;
	pcl::getMinMax3D(*(cloud->cloud), minPt, maxPt);
	vec.push_back(QString("%1  %2  %3").arg(minPt.x).arg(minPt.y).arg(minPt.z));
	vec.push_back(QString("%1  %2  %3").arg(maxPt.x).arg(maxPt.y).arg(maxPt.z));

	///��¼X�ķ�Χ6��Y�ķ�Χ7��Z�ķ�Χ8
	vec.push_back(QString("%1 - %2").arg(minPt.x).arg(maxPt.x));
	vec.push_back(QString("%1 - %2").arg(minPt.y).arg(maxPt.y));
	vec.push_back(QString("%1 - %2").arg(minPt.z).arg(maxPt.z));

	///��¼���Ƶ�����9
	/*Eigen::Vector4f centroid;
	pcl::compute3DCentroid(*cloud, centroid);
	std::cout << "The XYZ coordinates of the centroid are: ("
	<< centroid[0] << ", "
	<< centroid[1] << ", "
	<< centroid[2] << ")." << std::endl;*/
	vec.push_back(QString("%1  %2  %3").arg(minPt.x).arg(minPt.y).arg(minPt.z));

	///��¼���Ƶ�����10 ������11
	vec.push_back(QString("%1  %2  %3").arg((minPt.x + maxPt.x)/2).
		arg((minPt.y + maxPt.y) / 2).
		arg((minPt.z + maxPt.z) / 2));
	vec.push_back("  ");

	///��¼������������12��·��13����ɫ14
	vec.push_back("  ");
	vec.push_back(cloud->path);
	vec.push_back("Scalar-RGB");

	emit submitMsg(vec);
}