#include "ThreadUnionFind.h"



#include <QDebug>

#include <iostream>
#include <vector>
#include "math.h"
#include "UnionFind.h"
#include <fstream>
#include <sys/stat.h>

//#include "MyPointType.h"


ThreadUnionFind::ThreadUnionFind(QObject *parent)
: QObject(parent)
{
}

ThreadUnionFind::~ThreadUnionFind()
{
}

void ThreadUnionFind::runTask(){

	qDebug() << "UF Thread is starting ...";

	/************************************************************************/
	///  ��ά�����ز�����ɫ����ֶ�  X Y Z Intensity R G B  
	///  ��������ɵĵ��ƽ��Ϊ    X Y Z Intensity ReturnsNum Height R G B binary
	/************************************************************************/

	///��һ��������һ���Զ������ͣ�XYZ_IRHRGBBinary���ĵ���
	pcl::PointCloud<XYZ_IRHRGBBinary>::Ptr cloud(new pcl::PointCloud<XYZ_IRHRGBBinary>);

	///�ڶ�������ȡ�����ļ�
	if (SRC_PATH.substr(SRC_PATH.length() - 3, SRC_PATH.length() - 1) == "txt"){

		///��ȡ�ļ�����
		std::string file_name = SRC_PATH;
		struct stat s;
		stat(file_name.c_str(), &s);
		///��ȡָ���ı�������
		std::string file_buf(s.st_size + 1, '\0');
		///���ļ��е�����һ���Զ�����
		FILE* fp = fopen(file_name.c_str(), "rb");
		fread(&file_buf[0], sizeof(char), file_buf.size(), fp);
		int line_count = 0; ///��¼�ļ����е�����
		const char* file_buf_tmp = file_buf.c_str(); ///��ȡ�ļ�����ָ��
		while (*file_buf_tmp != '\0')
		{
			///���ҵ�һ������
			const char* p = strchr(file_buf_tmp, '\n');
			if (p == NULL)
			{
				///���һ��û��'\n'
				++line_count;
				break;
			}
			///���˿���
			line_count += p - file_buf_tmp > 0;
			///������һ��'\n'
			file_buf_tmp += p - file_buf_tmp + 1;
		}

		///��ʼ�����ƴ�С
		cloud->width = line_count;
		cloud->height = 1;
		cloud->points.resize(cloud->width * cloud->height);
		
		float x, y, z, intensity, r, g, b;
		std::ifstream ifs(SRC_PATH);
		if (!ifs){
			std::cout << ">> Loading failed ...\n";
			return;
		}

		int num = 1;
		while (!ifs.eof()){

			ifs >> x >> y >> z >> intensity >> r >> g >> b;

			cloud->points[num - 1].x = x;
			cloud->points[num - 1].y = y;
			cloud->points[num - 1].z = z;

			cloud->points[num - 1].intensity = intensity;
			cloud->points[num - 1].returnsNum = 0.0f;
			cloud->points[num - 1].height = 0.0f;

			cloud->points[num - 1].r = r;
			cloud->points[num - 1].g = g;
			cloud->points[num - 1].b = b;

			cloud->points[num - 1].bin = 0.0f;

			num++;
		}
		ifs.close();
	}
	else if (SRC_PATH.substr(SRC_PATH.length() - 3, SRC_PATH.length() - 1) == "pcd"){
		if (pcl::io::loadPCDFile<XYZ_IRHRGBBinary>(SRC_PATH, *cloud) == -1){
			std::cout << ">> Loading failed ...\n";
		}
	}
	else{
		/// ��������չ�������͵���
		std::cout << ">>> Unknon subfix of \n";
		return;
	}

	///������������һ�����鼯
	UnionFind uf(cloud->points.size());

	///���Ĳ�������KDTree
	pcl::KdTreeFLANN<XYZ_IRHRGBBinary> kdtree;
	kdtree.setInputCloud(cloud);
	XYZ_IRHRGBBinary searchPoint;

	///���岽�������ִ�� kdTree

	/*
	*  ���ѡȡһ���㿪ʼ��Ϊ��ʼ������
	*  �Ѿ��������������� K �������һ�� Radius ��Χ�ڵĵ�ϲ�Ϊһ�࣬���ҽ�����λ���ҵ����ڵ�
	*  ����һ������Ϊ�����㣺
	*		��1�������������㼰����Χ�ĵ���ϳɵ������֮ǰ�������غϵĵ㣬��ô�����ཫ�ϲ�Ϊһ��
	*		��2�������������㼰����Χ�ĵ���ϳɵ������֮ǰ����û�й����㣬��ô����Ϊ�����
	*  �������²���
	*/
	
	emit changeBar(222);
	if (clusterType == 1){ /// K ����
		std::vector<int> pointIdxNKNSearch(K);
		std::vector<float> pointNKNSquaredDistance(K);
		for (size_t i = 0; i < cloud->points.size(); i++){
			searchPoint = cloud->points[i];
			if (kdtree.nearestKSearch(searchPoint, K, pointIdxNKNSearch, pointNKNSquaredDistance) > 0){
				for (size_t j = 0; j < pointIdxNKNSearch.size(); j++){
					if (isPrimary == true && isSecdonry == true){
						if ((abs(cloud->points[pointIdxNKNSearch[j]].z - searchPoint.z) < primaryH &&
							abs(cloud->points[pointIdxNKNSearch[j]].intensity - cloud->points[i].intensity) < primaryI) ||
							(abs(cloud->points[pointIdxNKNSearch[j]].z - searchPoint.z) < secondaryH &&
							abs(cloud->points[pointIdxNKNSearch[j]].intensity - cloud->points[i].intensity) < secondaryI)){
							if (uf.find(i) != uf.find(pointIdxNKNSearch[j]))
								uf.unionElements(i, pointIdxNKNSearch[j]);
						}
					}
					else if (isPrimary == true && isSecdonry == false){
						if ((abs(cloud->points[pointIdxNKNSearch[j]].z - searchPoint.z) < primaryH &&
							abs(cloud->points[pointIdxNKNSearch[j]].intensity - cloud->points[i].intensity) < primaryI)){
							if (uf.find(i) != uf.find(pointIdxNKNSearch[j]))
								uf.unionElements(i, pointIdxNKNSearch[j]);
						}
					}
					else{
						if ((abs(cloud->points[pointIdxNKNSearch[j]].z - searchPoint.z) < secondaryH &&
							abs(cloud->points[pointIdxNKNSearch[j]].intensity - cloud->points[i].intensity) < secondaryI)){
							if (uf.find(i) != uf.find(pointIdxNKNSearch[j]))
								uf.unionElements(i, pointIdxNKNSearch[j]);
						}
					}
				}
			}
		}
	}
	else{ /// Radius ����
		std::vector<int> pointIdxRadiusSearch;
		std::vector<float> pointRadiusSquaredDistance;
		for (size_t i = 0; i < cloud->points.size(); i++){
			searchPoint = cloud->points[i];
			if (kdtree.radiusSearch(searchPoint, radius, pointIdxRadiusSearch, pointRadiusSquaredDistance) > 0){
				for (size_t j = 0; j < pointIdxRadiusSearch.size(); j++){
					if (isPrimary == true && isSecdonry == true){
						if ((abs(cloud->points[pointIdxRadiusSearch[j]].z - searchPoint.z) < primaryH &&
							abs(cloud->points[pointIdxRadiusSearch[j]].intensity - cloud->points[i].intensity) < primaryI) ||
							(abs(cloud->points[pointIdxRadiusSearch[j]].z - searchPoint.z) < secondaryH &&
							abs(cloud->points[pointIdxRadiusSearch[j]].intensity - cloud->points[i].intensity) < secondaryI)){
							if (uf.find(i) != uf.find(pointIdxRadiusSearch[j]))
								uf.unionElements(i, pointIdxRadiusSearch[j]);
						}
					}
					else if (isPrimary == true && isSecdonry == false){
						if ((abs(cloud->points[pointIdxRadiusSearch[j]].z - searchPoint.z) < primaryH &&
							abs(cloud->points[pointIdxRadiusSearch[j]].intensity - cloud->points[i].intensity) < primaryI)){
							if (uf.find(i) != uf.find(pointIdxRadiusSearch[j]))
								uf.unionElements(i, pointIdxRadiusSearch[j]);
						}
					}
					else{
						if ((abs(cloud->points[pointIdxRadiusSearch[j]].z - searchPoint.z) < secondaryH &&
							abs(cloud->points[pointIdxRadiusSearch[j]].intensity - cloud->points[i].intensity) < secondaryI)){
							if (uf.find(i) != uf.find(pointIdxRadiusSearch[j]))
								uf.unionElements(i, pointIdxRadiusSearch[j]);
						}
					}
				}
			}
		}
	}

	///��������ͳ�Ƹ��ڵ����������ڵ��Ӧ������
	emit changeBar(333);
	std::vector<int> root;
	static int numRoot = 0;  /// ���ۺϳɶ�����
	for (size_t i = 0; i < cloud->points.size(); i++){
		if (uf.isRoot(i) == true){
			root.push_back(i);  ///i ������������ⲽ����ͳ��һ������ɶ�����
			numRoot++;
		}
	}

	///���߲���ͳ�ƾ����
	emit changeBar(444);
	std::vector<std::vector<int>> cluster;  ///����һ����ά���飬ͳ�Ƹ��ڵ���ӽڵ�
	cluster.clear();
	cluster.resize(numRoot);
	for (int i = 0; i < numRoot; i++)
		cluster[i].push_back(root[i]);

	int pointsNum;
	std::vector<int>::iterator it;
	for (size_t i = 0; i < cloud->points.size(); i++){
		if (uf.isRoot(i) == false){
			it = find(root.begin(), root.end(), uf.find(i));
			if (it != root.end()){ ///�ҵ����ڵ���
				auto index = std::distance(std::begin(root), it);
				cluster[(int)(index)].push_back(i);
			}
		}
		pointsNum++;
	}

	///�ڰ˲������з���ǿ���޸������µ���
	emit changeBar(555);
	float sum_z, sum_intensity;
	for (size_t i = 0; i < root.size(); i++){
		for (size_t j = 0; j < cluster[i].size(); j++){
			//sum_z += cloud->points[cluster[i][j]].z;
			sum_intensity += cloud->points[cluster[i][j]].intensity;
		}

		//sum_z = sum_z / cluster[i].size();
		sum_intensity = sum_intensity / cluster[i].size();

		for (size_t t = 0; t < cluster[i].size(); t++){
			//cloud->points[cluster[i][t]].z = sum_z;
			cloud->points[cluster[i][t]].intensity = sum_intensity;
		}

		sum_z = sum_intensity = 0.0;

		int value = (int)((float)i / cloud->points.size() * 100);
		if (i % 10 == 0 && value <= 99)
			emit changeBar(value);
	}

	///�ھŲ����������� .txt / .pcd
	emit changeBar(666);
	if (SRC_PATH.substr(SRC_PATH.length() - 3, SRC_PATH.length() - 1) == "txt"){
		std::ofstream ofs(DEST_PATH);
		for (size_t i = 0; i < cloud->points.size(); i++){
			ofs << cloud->points[i].x << " "
				<< cloud->points[i].y << " "
				<< cloud->points[i].z << " "
				<< cloud->points[i].intensity << " "
				<< cloud->points[i].returnsNum << " "
				<< cloud->points[i].height << " "
				<< cloud->points[i].r << " "
				<< cloud->points[i].g << " "
				<< cloud->points[i].b << " "
				<< cloud->points[i].bin << "\n";
		}
		ofs.close();
	}
	else if (SRC_PATH.substr(SRC_PATH.length() - 3, SRC_PATH.length() - 1) == "pcd"){
		///�������� .pcd
		pcl::io::savePCDFileASCII(DEST_PATH, *cloud);
	}

	///��󣬷���������ϵ��ź�
	emit finishUF(pointsNum, numRoot);  ///�������

}
