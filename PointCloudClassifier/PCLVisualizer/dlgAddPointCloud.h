///##############################################################
///
///		Author: Neverland_LY     Date:2018-7-9 22:56
///		
///		��δ���ʵ���˵����ļ��Ĵ򿪲���
///		
///		Ŀǰֻ֧�� XYZRGB.pcd  �ļ��Ķ�ȡ�������Ժ�����������ʽ����������
///
///##############################################################


#pragma once

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QKeyEvent>
#include <QCloseEvent>

#include "ThreadLoadingCloudPoints.h"

namespace Ui { class dlgAddPointCloud; };

class dlgAddPointCloud : public QDialog
{
	Q_OBJECT

public:
	dlgAddPointCloud(QWidget *parent, ThreadLoadingCloudPoints *);
	~dlgAddPointCloud();

	void keyPressEvent(QKeyEvent *);

	void setValue(int);

private:
	Ui::dlgAddPointCloud *ui;

	QString file_name;
	
};
