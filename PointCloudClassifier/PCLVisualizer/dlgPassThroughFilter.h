///##############################################################
///
///		Author: Neverland_LY     Date:2018-7-9 22:56
///
///		�ⲿ�ִ�����ֱͨ�˲��Ĵ��ڵ�ʵ���ļ�
///		
///##############################################################


#pragma once

#include <QDialog>
#include <QKeyEvent>
#include <QThread>
#include "ThreadPassThrough.h"

#include "DiyXYZRGBPoint.h"

namespace Ui { class dlgPassThroughFilter; };

class dlgPassThroughFilter : public QDialog
{
	Q_OBJECT

public:
	dlgPassThroughFilter(LY::XYZRGBCloud & selectedRgbCloud, QWidget *parent = Q_NULLPTR);
	~dlgPassThroughFilter();

	void keyPressEvent(QKeyEvent *);


public slots:
	void convertButton();

signals:
	void startPass();

private:
	Ui::dlgPassThroughFilter *ui;

	LY::XYZRGBCloud cloud;

	QThread * thread;
	ThreadPassThrough *convertThread;

};
