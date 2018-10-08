#include "CrossLine.h"

CrossLine::CrossLine()
{
}


CrossLine::~CrossLine()
{
}

void CrossLine::drawCrossLine(Mat & img, Point2f & p, int count){

	//��������
	int thick = 1;
	int type = 8;

	//��ȡ���ĵ�ģ�x��y��
	int px = (int)(p.x);
	int py = (int)(p.y);

	//�����ߵĵ�һ������ص�
	Point pTop(px - 15, py);
	Point pBot(px + 15, py);

	//�����ߵĵڶ�������ص�
	Point pLef(px, py - 15);
	Point pRig(px, py + 15);

	//��ʼ����ʮ��˿
	cv::line(img, pTop, pBot, Scalar(0, 0, 255), thick);
	cv::line(img, pLef, pRig, Scalar(0, 0, 255), thick);

	//��ʼ��ע
	if (count != -1){
		std::string text = std::to_string(count + 1);
		//int fontFace = FONT_HERSHEY_SCRIPT_COMPLEX;
		int fontFace = FONT_HERSHEY_PLAIN;
		double fontScale = 2.5;
		int thickness = 2;
		Point txtOrg(px + 10, py - 10);
		putText(img, text, txtOrg, fontFace, fontScale, Scalar(0, 0, 255), thickness, 8);

	}

}

void CrossLine::drawCircle(Mat & img, Point & p){

	int radius = 3;

	circle(img, p, radius, Scalar(0, 255, 255), 1);

}


