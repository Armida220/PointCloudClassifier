///##############################################################
///
///		Author: Neverland_LY     Date:2018-7-10 16:49
///
///		������һ�������࣬������ͼƬ�ϻ�ʮ��˿
///		
///		
///
///##############################################################

#pragma once

#ifndef _CROSSLINE_H_
#define _CROSSLINE_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>


using namespace cv;

class CrossLine
{
public:
	CrossLine();
	~CrossLine();

	void drawCrossLine(Mat &, Point2f &, int count = 0); //һ���ʮ��˿
	void drawCircle(Mat &, Point &);  //��Բ��

private:
	Point2f _p;  //���ĵ�
};


#endif  //_CROSSLINE_H_



