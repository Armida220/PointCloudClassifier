#include "ThreadLoadingCloudPoints.h"
#include "DiyXYZRGBPoint.h"
#include <QDebug>
#include <fstream>
#include <sys/stat.h>

ThreadLoadingCloudPoints::ThreadLoadingCloudPoints(QObject *parent)
: QThread(parent)
{
	pointCloudPath = "";
	fileName = "";
	qDebug() << ">>> ThreadLoadingCloudPoints construct ...";
}

ThreadLoadingCloudPoints::~ThreadLoadingCloudPoints()
{
	qDebug() << ">>> ThreadLoadingCloudPoints destruct ...";
}

void ThreadLoadingCloudPoints::run(){

	///�жϵ��������ļ���������ӵ���
	LY::XYZRGBCloud cloud;
	cloud.path = pointCloudPath; 
	cloud.name = fileName; 

	std::string str = fileName.toStdString();
	if (str.substr(str.length() - 3, str.length() - 1) == "txt"){

		///��ȡ�ļ�����
		std::string file_name = pointCloudPath.toStdString();
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

		float x, y, z, r, g, b;
		cloud.cloud.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
		cloud.cloud->width = line_count;
		cloud.cloud->height = 1;
		cloud.cloud->points.resize(cloud.cloud->width * cloud.cloud->height);

		std::ifstream ifs(file_name);
		if (!ifs){
			std::cout << ">> Loading " << fileName.toStdString() << " failed ...\n";
			return;
		}

		int num = 1;
		while (!ifs.eof()){

			ifs >> x >> y >> z >> r >> g >> b;

			cloud.cloud->points[num - 1].x = x;
			cloud.cloud->points[num - 1].y = y;
			cloud.cloud->points[num - 1].z = z;

			cloud.cloud->points[num - 1].r = r;
			cloud.cloud->points[num - 1].g = g;
			cloud.cloud->points[num - 1].b = b;

			num++;
			if (num > line_count){
				emit msgBarValue(100);
				break;
			}

			if (num % 1000 == 0)
				emit msgBarValue((int)(float(num) / line_count * 100));
		}
		ifs.close();
	}
	else if (str.substr(str.length() - 3, str.length() - 1) == "pcd"){
		cloud.cloud.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
		emit msgBarValue(30);
		if (pcl::io::loadPCDFile<pcl::PointXYZRGB>(pointCloudPath.toStdString(), *(cloud.cloud)) == -1){
			std::cout << ">> Loading " << fileName.toStdString() << " failed ...\n";
		}
		emit msgBarValue(100);
	}
	else{
		/// ��������չ�������͵���
		std::cout << ">>> Unknon subfix of " << fileName.toStdString() << "\n";
		return;
	}

	emit submitCloud(cloud);
}


