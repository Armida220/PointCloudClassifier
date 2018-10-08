#include "ThreadBuildSingulation.h"

#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

/// openCV
#include <iostream>   
#include <core/core.hpp>    
#include <highgui/highgui.hpp>   

#include "math.h"
#include "float.h"
#include <algorithm>  
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "Windows.h"

#include <QFile>
#include <QString>
#include <QTextStream>

#define NORMALIZED_MAX 255
#define NORMALIZED_MIN 0
#define ROOT_TWO 1.41421

using namespace std;

ThreadBuildSingulation::ThreadBuildSingulation(QObject *parent)
: QObject(parent)
{
	SRC_PATH = "E:/cloud27-";

	alpha = 0.2; beta = 0.8;

	GSD = 0.5;

	treeK = 20;
	radius = GSD/*ROOT_TWO * GSD / 2*/;

	RUN_BEGIN = RUN_END = INDEX = 0;

	vecPij.clear();
}

ThreadBuildSingulation::~ThreadBuildSingulation()
{

}

void ThreadBuildSingulation::runTask(){

	




}

#if 0
void ThreadBuildSingulation::runTask(){

	///��һ������ȡ����
	emit changeValue(INDEX, 10);
	std::string path = "E:/graymat-" + std::to_string(INDEX) + ".txt";
	std::ofstream ofs(path.data());
	std::string DEST_PATH = SRC_PATH + std::to_string(INDEX) + ".pcd";
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	if (pcl::io::loadPCDFile<pcl::PointXYZRGB>(DEST_PATH, *cloud) == -1){
		return;
	}

	///�ڶ��������ݵ��ƣ��� Z ����Ϊ 0 ����Ѱ�������Сֵ
	emit changeValue(INDEX, 20);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudTemp(new pcl::PointCloud<pcl::PointXYZRGB>);
	if (pcl::io::loadPCDFile<pcl::PointXYZRGB>(DEST_PATH, *cloudTemp) == -1){
		return;
	}

	///��������ȷ����ֵ
	/*
	GSD 0.5
	width (229.6- 97.7) / 0.5 = 263 = 9 * 27 + 20 , BASED_NUM = 27
	height (148.2 + 58.9) / 0.5 = 414
	*/
	emit changeValue(INDEX, 30);
	Xmin = 97.724998;
	Xmax = 229.604996;  
	Ymin = -58.930698;
	Ymax = 148.229294;
	Zmin = 400.657013;
	Zmax = 438.446991;

	///���Ĳ�������
	emit changeValue(INDEX, 40);
	for (size_t i = 0; i < cloudTemp->points.size(); i++){
		cloudTemp->points[i].z = 0;
	}

	///���岽������ width �� height
	emit changeValue(INDEX, 50);
	width = (int)((Xmax - Xmin) / GSD);  //��Ƭ�����ؿ϶���Сһ��
	height = (int)((Ymax - Ymin) / GSD);

	///�������������飬��ȡPij
	emit changeValue(INDEX, 60);
	for (int i = RUN_BEGIN; i < RUN_END; i++){

		for (int j = 0; j < height; j++){

			///���߲�������������
			pcl::PointXYZRGB searchPoint;
			searchPoint.x = i * GSD + GSD / 2 + Xmin;
			searchPoint.y = j * GSD + GSD / 2 + Ymin;
			searchPoint.z = 0.0;

			///�ڰ˲������� KD-Tree
			pcl::KdTreeFLANN<pcl::PointXYZRGB> kdtree;
			kdtree.setInputCloud(cloudTemp);
			float Pij = 0.0;
			if (kdtree.radiusSearch(searchPoint, radius, pointIdxRadiusSearch, pointRadiusSquaredDistance) > 0){

				///�ھŲ����޳������
				for (int k = 0; k < pointIdxRadiusSearch.size(); k++){
					if (cloudTemp->points[pointIdxRadiusSearch[k]].x < searchPoint.x - GSD / 2 ||
						cloudTemp->points[pointIdxRadiusSearch[k]].x > searchPoint.x + GSD / 2 ||
						cloudTemp->points[pointIdxRadiusSearch[k]].y < searchPoint.y - GSD / 2 ||
						cloudTemp->points[pointIdxRadiusSearch[k]].y > searchPoint.y + GSD / 2){
						std::vector<int>::iterator it = pointIdxRadiusSearch.begin() + k;
						pointIdxRadiusSearch.erase(it);
						k--;
					}
				}

				///��ʮ����Ѱ�� ij �ڵ� H ���ֵ��Сֵ
				if (pointIdxRadiusSearch.size() == 0){
					Pij = 0.0;
				}

				if (pointIdxRadiusSearch.size() == 1){
					float diatance1 = cloud->points[pointIdxRadiusSearch[0]].x - searchPoint.x;
					float diatance2 = cloud->points[pointIdxRadiusSearch[0]].y - searchPoint.y;
					float distance = sqrt(diatance1 * diatance1 + diatance2 * diatance2);
					Pij = alpha * ROOT_TWO * GSD / distance + beta * 1.0 * 0.0;
				}

				if (pointIdxRadiusSearch.size() >= 2){
					std::vector<float> Pijk; Pijk.clear();
					Hmin = Hmax = cloud->points[pointIdxRadiusSearch[0]].z;
					for (int m = 1; m < pointIdxRadiusSearch.size(); m++){
						if (cloud->points[pointIdxRadiusSearch[m]].z < Hmin)
							Hmin = cloud->points[pointIdxRadiusSearch[m]].z;
						else if (cloud->points[pointIdxRadiusSearch[m]].z > Hmax)
							Hmax = cloud->points[pointIdxRadiusSearch[m]].z;
					}

					Hmin -= 1.0; Hmax += 1.0;  ///�߳�����
					float deltaMin = abs(Hmin - Zmin);
					float deltaMax = abs(Zmax - Hmax);

					///��ʮһ�������� Pjik
					for (int n = 0; n < pointIdxRadiusSearch.size(); n++){
						float diatance1 = cloud->points[pointIdxRadiusSearch[n]].x - searchPoint.x;
						float diatance2 = cloud->points[pointIdxRadiusSearch[n]].y - searchPoint.y;
						float distance = sqrt(diatance1 * diatance1 + diatance2 * diatance2);
						float PijkD = ROOT_TWO * GSD / distance;

						float PijkT = abs((cloud->points[pointIdxRadiusSearch[n]].z - Hmin) * deltaMin / deltaMax);

						Pijk.push_back(alpha * PijkD + beta * PijkT);
					}

					///��ʮ�������� Pij
					float sum;
					for (int t = 0; t < Pijk.size(); t++){
						Pij += Pijk[t] * cloud->points[pointIdxRadiusSearch[t]].z;
						sum += Pijk[t];
					}

					Pij = Pijk.size() * Pij / sum;
				}
			}
			else{
				Pij = 0.0;
			}

			vecPij.push_back(Pij);

			///��ʮ������ɾ������
			pointIdxRadiusSearch.clear();
			pointRadiusSquaredDistance.clear();

		} /// J ѭ������

		///��ʮһ������һ��
		std::vector<float>::iterator max = std::max_element(std::begin(vecPij), std::end(vecPij));
		std::vector<float>::iterator min = std::min_element(std::begin(vecPij), std::end(vecPij));
		int tmdMax = *max;
		int tmdMin = *min; 
		int tmdDelta = tmdMax - tmdMin;
		if (tmdDelta != 0){
			for (size_t n = 0; n < vecPij.size(); n++)
				vecPij[n] = NORMALIZED_MIN + (vecPij[n] - *min)  * (NORMALIZED_MAX - NORMALIZED_MIN) / (*max - *min);
		}

		///��ʮ���������ɻҶ�ͼ
		for (size_t f = 0; f < vecPij.size() - 1; f++)
			ofs << (int)vecPij[f] << " ";
		ofs << (int)vecPij[vecPij.size() - 1] << "\n";

		vecPij.clear();

		emit changeValue(INDEX, (float)(i - RUN_BEGIN) / (RUN_END - RUN_BEGIN) * 100);

	}

	ofs.close();

	emit changeValue(INDEX, 100);
}

#endif
