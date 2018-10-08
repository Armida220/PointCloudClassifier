///##############################################################
///
///		Author: Neverland_LY     Date:2018-7-9 22:56
///
///		���Ǳ�����������沿�֣�Ҳ�ǳ�������ڣ��ܹ��������¹��ܣ�
///		
///		���ļ��������ļ��Ĵ򿪲������Լ�����Ĺرյ�
///
///		�����ߡ��Ե��Ƶ�һЩ�������ݲ����������ѹ���������
///
///		���˲����Ե��ƽ���һЩ���봦����ȡ�����
///
///     ����ͼ���Ե��Ƶ�չʾ����һЩ�����ȣ�����ͬһ������ʾ������Ƶ�
///
///		������ת�������Ƹ�ʽ��ת������ *.txt��*.pcd��*.las��*.lyz ��
///
///		���״����ݴ������״����ݵ�һЩ�򵥲���
///		
///		���������������˵����
///
///		���⣬���滹�����˵����б��ڡ����Դ��ڡ�����̨���ڡ���ͼ���ڵ�
///		
///##############################################################

/// C++11
#include "pclviewer.h"
#include "ui_pclviewer.h"

#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <string>

///OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>

///�Զ���ĶԻ���
#include "dlgPassThroughFilter.h"
#include "dlgIntensityFix.h"
#include "dlgTxt2PCD.h"
#include "dlgPCD2TXT.h"
#include "dlgLAS2TXT.h"
#include "dlgPCDBinary2PCDAscii.h"
#include "dlgKdTreeCluster.h"
#include "dlgAddPointCloud.h"
#include "dlgBinarySignal.h"
#include "dlgTestDialog.h"
#include "dlgMultiViewer.h"
#include "dlgCopyFile.h"

#include "ThreadRGBResampling.h"
#include "ThreadLoadingCloudPoints.h"
#include "ThreadCalcPointCloudMsg.h"
#include "ThreadSaveAs.h"

PCLViewer::PCLViewer(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::PCLViewer)
{
	ui->setupUi(this);

	this->setWindowTitle("PointCloudClassifier@Neverland_LY");
	this->setWindowIcon(QIcon("images/logo.png"));
	this->grabKeyboard(); //���в�������¼�����

	//��ʼ��
	initPara(); //��ʼ��
	inintTableWidget();  //��ʼ�� QTableWidget
	initConnector();

	qDebug() << ">>> UI-Thread :   " << QThread::currentThread();
}

PCLViewer::~PCLViewer(){
	delete threadDelegater;  ///���û��ָ�������������ڹر������ڵ�ʱ��Ҫ�������٣�����ᵼ���ڴ�й¶
	delete rect;
	delete selectedCloud;
	delete itemModel;

	delete ui;

}

void PCLViewer::initPara(){

	/// UI ��������--------------------------------------------------------------------------------

	//��ʼ�����ڣ��������
	desktopRect = QApplication::desktop()->availableGeometry();
	iTitleBarHeight = style()->pixelMetric(QStyle::PM_TitleBarHeight);  // ��ȡ�������߶�
	setGeometry(0, iTitleBarHeight, desktopRect.width(), desktopRect.height() - iTitleBarHeight);
	resize(desktopRect.width(), desktopRect.height() - iTitleBarHeight);   // �̶������С

	//��ʼ������̨�������ӡ��Ϣ
	itemModel = new QStandardItemModel(this);
	ui->listView->setUpdatesEnabled(true);
	ui->listView->setEditTriggers(false);
	ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	logExport(QString::fromLocal8Bit("��ӭʹ�ñ������"));

	//��ʼ��������
	ui->proBar->setValue(0);

	//����  dockwidget 
	ui->dockWidget1->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	ui->dockWidget2->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	ui->dockWidget3->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

	//��ʼ�� QTreeWidget
	ui->treeWidget->setHeaderHidden(true);
	ui->treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	ui->treeWidget->setHorizontalScrollMode((QAbstractItemView::ScrollPerPixel));
	ui->treeWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	ui->treeWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");

	//��ʼ����Ĭ�ϴ���
	ui->tabWidget->setCurrentIndex(0);

	/// ȫ�ֱ�������----------------------------------------------------------------------------------

	//�����̵߳ĳ�ʼ������
	threadGUI = new QThread(this);
	threadDelegater = new ThreadDelegate;
	threadDelegater->moveToThread(threadGUI);
	threadGUI->start();

	//��ʼ�����ƴ�С
	pointSize = 1;

	//��ʼ��Ĭ��ѡ��ĵ���
	selectedCloud = nullptr;

	//��ʼ�� viewer
	viewer1.reset(new pcl::visualization::PCLVisualizer("3D Viewer1", false)); //false ����ʾ����
	viewer2.reset(new pcl::visualization::PCLVisualizer("3D Viewer2", false));
	viewer3.reset(new pcl::visualization::PCLVisualizer("3D Viewer3", false));

	//���ñ���ɫ
	viewer1->setBackgroundColor(0, 0, 0);
	viewer2->setBackgroundColor(0, 0, 0);
	viewer3->setBackgroundColor(0, 0, 0);

	//vuewer �� VTK �Ĺ���
	ui->qvtkWidget1->SetRenderWindow(viewer1->getRenderWindow());
	ui->qvtkWidget2->SetRenderWindow(viewer2->getRenderWindow());
	ui->qvtkWidget3->SetRenderWindow(viewer3->getRenderWindow());

	//ע���������¼�
	//viewer1->registerKeyboardCallback(keyboardEventOccurred);
	//viewer1->registerMouseCallback(mouseEventOccurred);

	//����VTK���ڽӿ�
	viewer1->setupInteractor(ui->qvtkWidget1->GetInteractor(), ui->qvtkWidget1->GetRenderWindow());
	viewer2->setupInteractor(ui->qvtkWidget2->GetInteractor(), ui->qvtkWidget2->GetRenderWindow());
	viewer3->setupInteractor(ui->qvtkWidget3->GetInteractor(), ui->qvtkWidget3->GetRenderWindow());

	//�����������״����ο�
	viewer1->addCoordinateSystem(0.5);
	viewer2->addCoordinateSystem(0.5);
	viewer3->addCoordinateSystem(0.5);

	//Ԥ���ص����ļ�
	pclLogo.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
	if (pcl::io::loadPCDFile<pcl::PointXYZRGB>("../init/PCL.pcd", *pclLogo) == -1){
		std::cout << ">>> The PCL-logo point cloud is loading failed ...\n";
	}
	else{
		viewer1->addPointCloud<pcl::PointXYZRGB >(pclLogo, "PCL-LOGO");
	}

	//�����������
	viewer1->resetCamera(); viewer2->resetCamera(); viewer3->resetCamera();

	//����FPS
	viewer1->setShowFPS(false); viewer2->setShowFPS(false); viewer3->setShowFPS(false);

	//ˢ�´���
	ui->qvtkWidget1->update(); ui->qvtkWidget2->update(); ui->qvtkWidget3->update();

}

void PCLViewer::inintTableWidget(){

	//�����б���
	vecTable.push_back(QString::fromLocal8Bit("��������"));  // 0
	vecTable.push_back(QString::fromLocal8Bit("����"));
	vecTable.push_back(QString::fromLocal8Bit("���С"));
	vecTable.push_back(QString::fromLocal8Bit("����"));
	vecTable.push_back(QString::fromLocal8Bit("��С��"));
	vecTable.push_back(QString::fromLocal8Bit("����"));
	vecTable.push_back(QString::fromLocal8Bit("X ��Χ"));
	vecTable.push_back(QString::fromLocal8Bit("Y ��Χ"));
	vecTable.push_back(QString::fromLocal8Bit("Z ��Χ"));
	vecTable.push_back(QString::fromLocal8Bit("����"));
	vecTable.push_back(QString::fromLocal8Bit("����"));
	vecTable.push_back(QString::fromLocal8Bit("����"));  //11

	vecTable.push_back(QString::fromLocal8Bit("�����������")); // 12
	vecTable.push_back(QString::fromLocal8Bit("·��"));
	vecTable.push_back(QString::fromLocal8Bit("��ɫ"));

	//�����б��������
	ui->tableWidget->setRowCount(vecTable.size()); //����  15
	ui->tableWidget->setColumnCount(2); //����
	ui->tableWidget->verticalHeader()->setDefaultSectionSize(25); //�и�
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ɱ༭
	ui->tableWidget->horizontalHeader()->setVisible(false); //���ñ�ͷ���ɵ����Ĭ�ϵ�����������
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //���ó�������
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //����ѡ����Ϊ������Ϊ��λ
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //����ѡ��ģʽ��ѡ����
	ui->tableWidget->setFrameShape(QFrame::NoFrame); //�����ޱ߿�
	ui->tableWidget->setShowGrid(false); //���ò���ʾ������
	ui->tableWidget->verticalHeader()->setVisible(false); //���ô�ֱͷ���ɼ�
	ui->tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:20px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 20px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");

	ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	//ui->tableWidget->setHorizontalScrollMode((QAbstractItemView::ScrollPerPixel));

	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu); //�Ҽ��˵���ʵ��

	//���QTableWidget
	for (int i = 0; i < vecTable.size(); i++){
		if (i == 0 || i == 12){
			QTableWidgetItem * item1 = new QTableWidgetItem(vecTable[i]);
			QTableWidgetItem * item2 = new QTableWidgetItem("");
			item1->setBackgroundColor(QColor(54, 54, 54)); item1->setTextColor(QColor(255, 255, 255));
			item2->setBackgroundColor(QColor(54, 54, 54)); item2->setTextColor(QColor(255, 255, 255));

			ui->tableWidget->setItem(i, 0, item1);
			ui->tableWidget->setItem(i, 1, item2);
		}
		else{
			if (i % 2 == 0){
				QTableWidgetItem * item1 = new QTableWidgetItem(vecTable[i]);
				QTableWidgetItem * item2 = new QTableWidgetItem("");
				item1->setBackgroundColor(QColor(238, 233, 233));
				item2->setBackgroundColor(QColor(238, 233, 233));
				ui->tableWidget->setItem(i, 0, item1);
				ui->tableWidget->setItem(i, 1, item2);
			}
			else{
				ui->tableWidget->setItem(i, 0, new QTableWidgetItem(vecTable[i]));
				ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));
			}
		}
	}

	ui->tableWidget->resizeColumnToContents(0);
	ui->tableWidget->resizeColumnToContents(1);
}

void PCLViewer::initConnector(){

	///�����̹߳���
	connect(this, &PCLViewer::delegaterStartRunTask, threadDelegater, &ThreadDelegate::runManageThread);
	connect(threadDelegater, &ThreadDelegate::signalBarValue, this, &PCLViewer::setBarValue);

	///���� Qtreewidget �ĵ�����˫����Ӧ�¼�
	connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeItemSelected(QTreeWidgetItem*, int)));
	connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(showSelectedItemCloud(QTreeWidgetItem*, int)));

	///����tablewidget���Ҽ��˵�
	connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));

	///�������ļ����Ĳ����¼�
	connect(ui->action_open, SIGNAL(triggered()), this, SLOT(actionOpenFile()));
	connect(ui->action_saveas, SIGNAL(triggered()), this, SLOT(actionSaveAs()));
	connect(ui->action_exit, SIGNAL(triggered()), this, SLOT(actionExit()));

	//���������ߡ��Ĳ����¼�
	connect(ui->action_zoomin, SIGNAL(triggered()), this, SLOT(actionZoomIn()));
	connect(ui->action_zoomout, SIGNAL(triggered()), this, SLOT(actionZoomOut()));
	connect(ui->action_zoom, SIGNAL(triggered()), this, SLOT(actionZoom()));
	connect(ui->action_compress, SIGNAL(triggered()), this, SLOT(actionCompressCloud()));
	connect(ui->action_cluster, SIGNAL(triggered()), this, SLOT(actionClusteCloud()));
	connect(ui->action_KNN, SIGNAL(triggered()), this, SLOT(actionKdtreeKNN()));
	connect(ui->action_Radius, SIGNAL(triggered()), this, SLOT(actionKdtreeRadius()));
	connect(ui->action_voxel, SIGNAL(triggered()), this, SLOT(actionVoxelization()));
	connect(ui->action_trim, SIGNAL(triggered()), this, SLOT(actionTrimCloud()));
	connect(ui->action_screenshot, SIGNAL(triggered()), this, SLOT(actionScreenShot()));
	connect(ui->action_copy, &QAction::triggered, [=](){
		dlgCopyFile * t = new dlgCopyFile(this);
		t->setAttribute(Qt::WA_DeleteOnClose);
		t->exec();
	});

	//�������˲����Ĳ����¼�
	connect(ui->action_pass, SIGNAL(triggered()), this, SLOT(actionPassThroughFilter()));
	connect(ui->action_voxelgrid, SIGNAL(triggered()), this, SLOT(actionVoxelCloud()));
	connect(ui->action_outliner, SIGNAL(triggered()), this, SLOT(actionOutlinerRemove()));
	connect(ui->action_project, SIGNAL(triggered()), this, SLOT(actionProjection()));
	connect(ui->action_cloud2grayimage, SIGNAL(triggered()), this, SLOT(actionGrayImage()));
	connect(ui->action_vegetation, &QAction::triggered, [=](){
		/*if (selectedRgbCloud == NULL)
			return;*/
		logExport(QString::fromLocal8Bit("��ʼ����ֲ������..."));
		//emit delegaterStartRunTask(LY::FilterVegetation, *selectedRgbCloud, vecPara);
	});

	//��������ͼ���Ĳ����¼�
	connect(ui->action_viewer1, SIGNAL(triggered()), this, SLOT(actionViewer1()));
	connect(ui->action_viewer2, SIGNAL(triggered()), this, SLOT(actionViewer2()));
	connect(ui->action_viewer4, SIGNAL(triggered()), this, SLOT(actionViewer4()));
	connect(ui->action_fullscreen, &QAction::triggered,
		[=](){
		/*if (ui->tabWidget->currentIndex() == 0)
			viewer1->setFullScreen(true);
			if (ui->tabWidget->currentIndex() == 1)
			viewer2->setFullScreen(true);
			if (ui->tabWidget->currentIndex() == 2)
			viewer3->setFullScreen(true);*/
	}
	);
	connect(ui->action_coordinate, &QAction::triggered,
		[=](){
		pcl::ModelCoefficients coeffs;
		coeffs.values.push_back(0.0);
		coeffs.values.push_back(0.0);
		coeffs.values.push_back(1.0);
		coeffs.values.push_back(0.0);
		viewer1->addPlane(coeffs, "PLANE-Z-0");
		viewer1->resetCamera();
		ui->qvtkWidget1->update();
	}
	);

	//����������ת�����Ĳ����¼�
	connect(ui->action_las2txt, SIGNAL(triggered()), this, SLOT(actionLAS2TXT()));
	connect(ui->action_txt2pcd, SIGNAL(triggered()), this, SLOT(actionTXT2PCDASCII()));
	connect(ui->action_pcdb2pcda, SIGNAL(triggered()), this, SLOT(actionPCDBINARY2PCDASCII()));
	connect(ui->action_pcd2txt, SIGNAL(triggered()), this, SLOT(actionPCDASCII2TXT()));

	//�������״����ݴ����Ĳ����¼�
	connect(ui->action_fixintensity_2, SIGNAL(triggered()), this, SLOT(actionIntensityFix()));
	connect(ui->action_kdtree_2, SIGNAL(triggered()), this, SLOT(actionKdTreeCluster()));
	connect(ui->action_binarysignal_2, SIGNAL(triggered()), this, SLOT(actionBinarySignal()));
	connect(ui->action_image2cloud, SIGNAL(triggered()), this, SLOT(actionImage2Cloud()));
	connect(ui->action_rgbresampling, &QAction::triggered, [=](){
		ThreadRGBResampling * tResampling = new ThreadRGBResampling;
		connect(tResampling, &ThreadRGBResampling::finished, tResampling, &QObject::deleteLater);
		tResampling->start();
	});

	

	//���������ڡ��Ĳ����¼�
	connect(ui->action_setting, SIGNAL(triggered()), this, SLOT(actionSetting()));
	connect(ui->action_about, SIGNAL(triggered()), this, SLOT(actionAbout()));
	connect(ui->action_help, SIGNAL(triggered()), this, SLOT(actionHelp()));
	connect(ui->action_beta, &QAction::triggered, [=](){

		///��������------------------------------------------





		///���Խ���----------------------------------------------
	});

}

///���ļ����������� ----------------------------------------------------------------------------------

void PCLViewer::actionOpenFile(){
	///�����߳�
	ThreadLoadingCloudPoints * tLoading = new ThreadLoadingCloudPoints; ///Ĭ�Ͼ��ǿ�ָ��
	connect(tLoading, &ThreadLoadingCloudPoints::submitCloud, this, &PCLViewer::addNewPointCloud);
	connect(tLoading, &ThreadLoadingCloudPoints::finished, tLoading, &QObject::deleteLater);
	///��������
	dlgAddPointCloud *t = new dlgAddPointCloud(this, tLoading); ///�̵߳����ô���
	connect(tLoading, &ThreadLoadingCloudPoints::msgBarValue, t, &dlgAddPointCloud::setValue);
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->exec();
}

void PCLViewer::addNewPointCloud(LY::XYZRGBCloud cloud){
	///�ж��Ƿ��Ѿ����ڸõ���
	if (mapXYZRGB.contains(cloud.name) == true){ ///�����ӵĵ����Ѿ�����
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("����ӵĵ����Ѿ����ڣ����޸��ļ�����������ӡ�"));
		return;
	}

	///���µ����б�
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setText(0, cloud.name);
	ui->treeWidget->addTopLevelItem(item);
	QTreeWidgetItem *itemChild = new QTreeWidgetItem;
	itemChild->setText(0, cloud.name);
	ui->treeWidget->topLevelItem(mapXYZRGB.size())->addChild(itemChild);
	ui->treeWidget->topLevelItem(mapXYZRGB.size())->setExpanded(true);
	itemChild->setCheckState(0, Qt::Checked);

	///����ѡ��
	vecCloud.push_back(cloud);
	mapXYZRGB.insert(cloud.name, cloud.path); ///���� mapXYZRGB
	selectedCloud = &vecCloud[vecCloud.size() - 1];
	treeItemSelected(ui->treeWidget->topLevelItem(mapXYZRGB.size() - 1)->child(0), 0);

	///��ʾ�������� 
	viewer1->removePointCloud("PCL-LOGO");  ///�����Ƴ� pcllogo ����
	viewer1->addPointCloud<pcl::PointXYZRGB>(cloud.cloud, cloud.name.toStdString());
	cloudNumOfViewer1++;  ///������ʾ�ĵ�������
	viewer1->resetCamera();
	ui->qvtkWidget1->update();

	///��ӡ��־
	logExport(QString::fromLocal8Bit("�ɹ���ӵ��� %1 ( %2 ) ").arg(cloud.name).arg(cloud.path));

}

void PCLViewer::showSelectedItemCloud(QTreeWidgetItem* item, int column){

	if (item->parent() == NULL)  ///������㴰����Ч
		return;

	if (item->checkState(0) == Qt::Checked){
		item->setCheckState(0, Qt::Unchecked);
		viewer1->removePointCloud(item->text(0).toStdString());
		viewer1->resetCamera();
		ui->qvtkWidget1->update();
		cloudNumOfViewer1--;
	}
	else{
		item->setCheckState(0, Qt::Checked);
		std::vector<LY::XYZRGBCloud>::iterator it;
		for (it = vecCloud.begin(); it != vecCloud.end(); it++){
			if (it->name == item->text(0)){
				viewer1->addPointCloud(it->cloud, it->name.toStdString());
				///��Ӱ�Χ����
				//pcl::PointXYZRGB minPt, maxPt;
				//pcl::getMinMax3D(*(it->cloud), minPt, maxPt);
				viewer1->resetCamera();
				ui->qvtkWidget1->update();
				cloudNumOfViewer1++;
				break;
			}
		}
	}

	if (cloudNumOfViewer1 == 0){
		viewer1->addPointCloud<pcl::PointXYZRGB >(pclLogo, "PCL-LOGO");
		viewer1->resetCamera();
		ui->qvtkWidget1->update();
	}
	else{
		viewer1->removePointCloud("PCL-LOGO");
	}
}

void PCLViewer::treeItemSelected(QTreeWidgetItem* item, int column){

	///��������б�����Ч
	if (item->parent() == nullptr)
		return;

	///����ѡ�����ĸ��������ݣ�ֱ�Ӳ�������
	int count = 0;
	for (std::vector<LY::XYZRGBCloud>::iterator i = vecCloud.begin(); i != vecCloud.end(); i++){
		if (i->name == item->text(0)){
			selectedCloud = &vecCloud[count];
			break;
		}
		count++;
	}

	///�����̼߳�����������Ϣ
	/*ThreadCalcPointCloudMsg * tCalc = new ThreadCalcPointCloudMsg(NULL);
	tCalc->cloud = selectedCloud;
	connect(tCalc, &ThreadCalcPointCloudMsg::submitMsg, this, &PCLViewer::updateAttributes);
	connect(tCalc, &ThreadCalcPointCloudMsg::submitMsg, tCalc, &QObject::deleteLater);
	tCalc->start();*/

	///����ѡ�е���ɫ
	QTreeWidgetItemIterator it(ui->treeWidget);
	while (*it){
		(*it)->setBackgroundColor(0, QColor(255, 255, 255));
		for (int i = 0; i < (*it)->childCount(); i++){
			(*it)->child(i)->setBackgroundColor(0, QColor(255, 255, 255));
		}
		++it;
	}

	item->setBackgroundColor(0, QColor(255, 248, 220)); /// �{ԇ�ɫ

	///��ӡ��Ϣ
	logExport(QString::fromLocal8Bit("����Ҫ�Ե��� %1 ���в���").arg(item->text(0)));
}

void PCLViewer::updateAttributes(std::vector<QString> vec){

	logExport(QString::fromLocal8Bit("�ұ�������..."));

	return;

	///��ʼ���
	for (int i = 0; i < vec.size(); i++){
		if (i == 0 || i == vec.size() - 3)
			continue;
		ui->tableWidget->item(i, 1)->setText(vec[i]);
	}

	///���²��þ���
	ui->tableWidget->resizeColumnToContents(0);
	ui->tableWidget->resizeColumnToContents(1);

	///��ӡ��Ϣ
	logExport(QString::fromLocal8Bit("������Ϣ�Ѹ���"));
}

void PCLViewer::actionSaveAs(){

	return;

	if (selectedCloud == nullptr)
		return;

	///ѡ���ļ�
	QString path= QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("���Ʊ���·��"), QString("C:/"), "TXT(.txt) ;; PCD(*.pcd)");

	if (path.isEmpty())
		return;

	///������������߳�
	ThreadSaveAs * tSave = new ThreadSaveAs(NULL);
	connect(tSave, &ThreadSaveAs::submitMsg, [=](){
		logExport(QString::fromLocal8Bit("����ɹ����浽����"));
	});
	connect(tSave, &ThreadSaveAs::submitMsg, tSave, &QObject::deleteLater);

	///��ֵ
	tSave->cc = selectedCloud; ///ֱ�Ӵ���ָ�����������
	tSave->path = path;

	///��ʼ�������
	tSave->start();
}

void PCLViewer::actionExit(){
	QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
	animation->setDuration(500);
	animation->setStartValue(1);
	animation->setEndValue(0.5);
	animation->start();
	this->close();
}

///�����ߡ���������-----------------------------------------------------------------------------------

void PCLViewer::actionZoomIn(){}

void PCLViewer::actionZoomOut(){}

void PCLViewer::actionZoom(){}

void PCLViewer::actionCompressCloud(){}

void PCLViewer::actionClusteCloud(){}

void PCLViewer::actionKdtreeKNN(){}

void PCLViewer::actionKdtreeRadius(){}

void PCLViewer::actionVoxelization(){}

void PCLViewer::actionTrimCloud(){}

void PCLViewer::actionScreenShot(){
	QString path;
	while (1){
		path = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("��������·��"), QString("C:/"), "PNG(*.png)");
		if (path == NULL)
			continue;
		break;
	}

	if (ui->tabWidget->currentIndex() == 0)
		viewer1->saveScreenshot(path.toStdString());
	if (ui->tabWidget->currentIndex() == 1)
		viewer2->saveScreenshot(path.toStdString());
	if (ui->tabWidget->currentIndex() == 2)
		viewer3->saveScreenshot(path.toStdString());
};

///���˲�����������-----------------------------------------------------------------------------------

void PCLViewer::actionPassThroughFilter(){


	dlgPassThroughFilter * psfilter = new dlgPassThroughFilter(*selectedCloud, this);
	psfilter->setAttribute(Qt::WA_DeleteOnClose);
	psfilter->exec();
}

void PCLViewer::actionVoxelCloud(){}

void PCLViewer::actionOutlinerRemove(){}

void PCLViewer::actionProjection(){}

void PCLViewer::actionGrayImage(){
	dlgTestDialog * t = new dlgTestDialog;
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->exec();
}

///����ͼ����Ӧ����----------------------------------------------------------------------------------

void PCLViewer::actionViewer1(){
	dlgMultiViewer * tp = new dlgMultiViewer(0, mapXYZRGB, this);
	connect(tp, &dlgMultiViewer::signalMap, this, &PCLViewer::showMultiCloud);
	tp->setAttribute(Qt::WA_DeleteOnClose);
	tp->exec();
}

void PCLViewer::actionViewer2(){
	dlgMultiViewer * tp = new dlgMultiViewer(1, mapXYZRGB, this);
	connect(tp, &dlgMultiViewer::signalMap, this, &PCLViewer::showMultiCloud);
	tp->setAttribute(Qt::WA_DeleteOnClose);
	tp->exec();
}

void PCLViewer::actionViewer4(){
	dlgMultiViewer * tp = new dlgMultiViewer(2, mapXYZRGB, this);
	connect(tp, &dlgMultiViewer::signalMap, this, &PCLViewer::showMultiCloud);
	tp->setAttribute(Qt::WA_DeleteOnClose);
	tp->exec();
}

///������ת������Ӧ����----------------------------------------------------------------------------

void PCLViewer::actionLAS2TXT(){
	dlgLAS2TXT *t = new dlgLAS2TXT(this);
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->exec();
}

void PCLViewer::actionTXT2PCDASCII(){
	dlgTxt2PCD * tp = new dlgTxt2PCD(this);
	tp->setAttribute(Qt::WA_DeleteOnClose);
	tp->exec();
}

void PCLViewer::actionPCDBINARY2PCDASCII(){
	//QMessageBox::information(this, "Information", "This function is temporarily not open to the outside world, thanks !");
	dlgPCDBinary2PCDAscii * t = new dlgPCDBinary2PCDAscii(this);
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->exec();
}

void PCLViewer::actionPCDASCII2TXT(){
	dlgPCD2TXT * tp = new dlgPCD2TXT(this);
	tp->setAttribute(Qt::WA_DeleteOnClose);
	tp->exec();
}

///���״����ݴ�����Ӧ����---------------------------------------------------------------------------

void PCLViewer::actionIntensityFix(){
	dlgIntensityFix *fix = new dlgIntensityFix(this);
	fix->setAttribute(Qt::WA_DeleteOnClose);
	fix->exec();
}

void PCLViewer::actionKdTreeCluster(){
	dlgKdTreeCluster * t = new dlgKdTreeCluster(this);
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->exec();
}

void PCLViewer::actionBinarySignal(){
	dlgBinarySignal *t = new dlgBinarySignal;
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->exec();
}

void PCLViewer::actionImage2Cloud(){}

///����������Ӧ����--------------------------------------------------------------------------------

void PCLViewer::actionSetting(){}

void PCLViewer::actionAbout(){}

void PCLViewer::actionHelp(){}

///���к���----------------------------------------------------------------------------------------

void PCLViewer::setBarValue(LY::THREAD_TYPE type, int value){
	ui->proBar->setValue(value);
	switch (type)
	{
	case LY::ToolPression:
		break;
	case LY::ToolCluster:
		break;
	case LY::ToolKdTreeK:
		break;
	case LY::ToolKdTreeRadius:
		break;
	case LY::FilterPassThrough:
		break;
	case LY::FilterVoxelGrid:
		break;
	case LY::FilterStaticalOutlinerRemoval:
		break;
	case LY::FilterProjection:
		break;
	case LY::FilterGray2Image:
		break;
	case LY::FilterVegetation:
		logExport("Vegetation successful");
		break;
	case LY::ConvertLas2Txt:
		break;
	case LY::ConvertTxt2Pcd:
		break;
	case LY::ConvertPcd2Pcd:
		break;
	case LY::ConvertPcd2Txt:
		break;
	case LY::LasFix:
		break;
	case LY::LasKdTree:
		break;
	case LY::LasBinarySignal:
		break;
	case LY::LasImage2Cloud:
		break;
	default:
		break;
	}

}

void PCLViewer::showMultiCloud(std::vector<QString> vec){

	if (vec.size() == 1){
		ui->tabWidget->setCurrentIndex(0);
		std::vector<LY::XYZRGBCloud>::iterator it;
		for (it = vecCloud.begin(); it != vecCloud.end(); it++){
			if (it->name == vec[0]){
				viewer1->addPointCloud<pcl::PointXYZRGB>(it->cloud, it->name.toStdString());
				break;
			}
		}
		viewer1->resetCamera();
		ui->qvtkWidget1->update();
	}
	else if (vec.size() == 2){
		ui->tabWidget->setCurrentIndex(1);
		std::vector<LY::XYZRGBCloud>::iterator it;
		for (it = vecCloud.begin(); it != vecCloud.end(); it++){
			static int count;
			if (it->name == vec[0]){
				int v1(0);
				viewer2->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
				pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(it->cloud);
				viewer2->addPointCloud<pcl::PointXYZRGB>(it->cloud, rgb, it->name.toStdString(), v1);
				count++;
			}
			if (it->name == vec[1]){
				int v2(0);
				viewer2->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
				pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(it->cloud);
				viewer2->addPointCloud<pcl::PointXYZRGB>(it->cloud, rgb, it->name.toStdString(), v2);
				count++;
			}
			if (count == 2)
				break;
		}
		viewer2->resetCamera();
		ui->qvtkWidget2->update();
	}
	else if (vec.size() == 4){
		ui->tabWidget->setCurrentIndex(2);
		std::vector<LY::XYZRGBCloud>::iterator it;
		for (it = vecCloud.begin(); it != vecCloud.end(); it++){
			static int count;
			if (it->name == vec[0]){
				int v1(0);
				viewer3->createViewPort(0.0, 0.0, 0.5, 0.5, v1);
				pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(it->cloud);
				viewer3->addPointCloud<pcl::PointXYZRGB>(it->cloud, rgb, it->name.toStdString(), v1);
				count++;
			}
			if (it->name == vec[1]){
				int v2(0);
				viewer3->createViewPort(0.5, 0.0, 1.0, 0.5, v2);
				pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(it->cloud);
				viewer3->addPointCloud<pcl::PointXYZRGB>(it->cloud, rgb, it->name.toStdString(), v2);
				count++;
			}
			if (it->name == vec[2]){
				int v3(0);
				viewer3->createViewPort(0.0, 0.5, 0.5, 1.0, v3);
				pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(it->cloud);
				viewer3->addPointCloud<pcl::PointXYZRGB>(it->cloud, rgb, it->name.toStdString(), v3);
				count++;
			}
			if (it->name == vec[3]){
				int v4(0);
				viewer3->createViewPort(0.5, 0.5, 1.0, 1.0, v4);
				pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(it->cloud);
				viewer3->addPointCloud<pcl::PointXYZRGB>(it->cloud, rgb, it->name.toStdString(), v4);
				count++;
			}
			if (count == 4)
				break;
		}
		viewer3->resetCamera();
		ui->qvtkWidget3->update();
	}
}

bool PCLViewer::event(QEvent *event){

	if (event->type() == QEvent::NonClientAreaMouseButtonDblClick){  ///�Ŵ�
		if (this->width() < desktopRect.width() || this->height() < desktopRect.height() - iTitleBarHeight){
			resize(desktopRect.width(), desktopRect.height() - iTitleBarHeight);   // �̶������С;
			this->move(0, 0);
		}
		else{ ///��С
			resize(desktopRect.width() / 3 * 2, (desktopRect.height() - iTitleBarHeight) / 3 * 2);
			this->move((desktopRect.width() - this->width()) / 2, (desktopRect.height() - iTitleBarHeight - this->height()) / 2);
		}
	}

	return QWidget::event(event);
}

void PCLViewer::closeEvent(QCloseEvent *event){
	QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
	animation->setDuration(500);
	animation->setStartValue(1);
	animation->setEndValue(0.5);
	animation->start();
	auto temp = QMessageBox::warning(this, QString::fromLocal8Bit("�˳�"),
		QString::fromLocal8Bit("��ȷ��Ҫ�˳������"), QMessageBox::Yes | QMessageBox::No);
	if (temp == QMessageBox::Yes){
		event->accept();
	}
	else{
		event->ignore();
		QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
		animation->setDuration(500);
		animation->setStartValue(0.5);
		animation->setEndValue(1);
		animation->start();
	}
}

void PCLViewer::mouseMoveEvent(QMouseEvent *event){
	/*if (event->buttons() == Qt::LeftButton){
		resize(desktopRect.width() / 3 * 2, (desktopRect.height() - iTitleBarHeight) / 3 * 2);
		}*/
}

void PCLViewer::keyPressEvent(QKeyEvent *event){
	if (event->key() == Qt::Key_Escape){

		viewer1->setFullScreen(false);
		/*viewer2->setFullScreen(false);
		viewer3->setFullScreen(false);*/
	}
	QWidget::keyPressEvent(event);
}

void PCLViewer::logExport(QString qstr){
	QDateTime dt = QDateTime::currentDateTime();
	QString time = dt.toString("[ yyyy-MM-dd  hh:mm:ss ]   ");
	qstr = time + qstr;
	QStandardItem *item = new QStandardItem(qstr);
	if (consoleList.size() % 2 == 1){
		QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
		linearGrad.setColorAt(0, QColor(255, 248, 220));
		QBrush brush(linearGrad);
		item->setBackground(brush);
	}
	itemModel->appendRow(item);
	ui->listView->setModel(itemModel);
	ui->listView->scrollToBottom();
	consoleList.append(qstr);
}

void PCLViewer::showMenu(QPoint pos)
{
	logExport(QString::fromLocal8Bit("��ѡ�� ���� / ��С ���ƴ�С��򿪵����ļ�·��"));

	//���ò˵�ѡ�� 
	QMenu *menu = new QMenu(ui->tableWidget);
	QAction * action1 = new QAction(QString::fromLocal8Bit("���ƴ�С����"), ui->tableWidget);
	QAction * action2 = new QAction(QString::fromLocal8Bit("���ƴ�С��С"), ui->tableWidget);
	QAction * action3 = new QAction(QString::fromLocal8Bit("�򿪵����ļ�·��"), ui->tableWidget);

	connect(action1, &QAction::triggered,
		[=](){
		pointSize++;
		viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, pointSize, selectedCloud->name.toStdString());
		ui->qvtkWidget1->update();
	});

	connect(action2, &QAction::triggered,
		[=](){
		if (pointSize == 1)
			return;
		pointSize--;
		viewer1->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, pointSize, selectedCloud->name.toStdString());
		ui->qvtkWidget1->update();
	});

	connect(action3, &QAction::triggered,
		[=](){
		std::string str = selectedCloud->path.toStdString();
		std::string delstr = selectedCloud->name.toStdString();
		str = str.substr(0, str.length() - delstr.length());
		QDesktopServices::openUrl(QString::fromStdString(str));
	});

	menu->addAction(action1);
	menu->addAction(action2);
	menu->addSeparator();
	menu->addAction(action3);

	menu->move(cursor().pos());
	menu->show();

	//����������x��y����� 
	int x = pos.x();
	int y = pos.y();
	QModelIndex index = ui->tableWidget->indexAt(QPoint(x, y));
	int row = index.row();//���QTableWidget�б��������� 
}

void PCLViewer::keyboardEventOccurred(const pcl::visualization::KeyboardEvent &event, void* viewer_void)
{
	if (event.getKeySym() == "r" && event.keyDown())
	{
		logExport("r is presed");
	}
}

void PCLViewer::mouseEventOccurred(const pcl::visualization::MouseEvent &event, void* viewer_void){
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
	if (event.getButton() == pcl::visualization::MouseEvent::LeftButton && event.getType() == pcl::visualization::MouseEvent::MouseButtonRelease)
		return;

}


#if 0
QMessageBox::critical(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("·������Ϊ�գ�����"), QMessageBox::Yes, QMessageBox::Yes);
#endif
