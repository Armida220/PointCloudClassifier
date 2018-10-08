/*****************************************************************************
*  OpenST Basic tool library                                                 *
*  Copyright (C) 2014 Henry.Wen  renhuabest@163.com.                         *
*                                                                            *
*  This file is part of OST.                                                 *
*                                                                            *
*  This program is free software; you can redistribute it and/or modify      *
*  it under the terms of the GNU General Public License version 3 as         *
*  published by the Free Software Foundation.                                *
*                                                                            *
*  You should have received a copy of the GNU General Public License         *
*  along with OST. If not, see <http://www.gnu.org/licenses/>.               *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*  @file     Example.h                                                       *
*  @brief    ���ļ��ļ���                                                    *
*  Details.                                                                  *
*                                                                            *
*  @author   Henry.Wen                                                       *
*  @email    renhuabest@163.com                                              *
*  @version  1.0.0.1(�汾��)                                                 *
*  @date     renhuabest@163.com                                              *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2014/01/24 | 1.0.0.1   | Henry.Wen      | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/


#pragma once

// Qt
#include <QMainWindow>
#include <QThread>
#include <QMouseEvent>
#include <QMap>
#include <QTreeWidgetItem>
#include <QRect>
#include <QEvent>
#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QApplication>
#include <QDesktopWidget>
#include <QProgressDialog>
#include <QProgressBar>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QCheckBox>
#include <QMenu>
#include <QFile>
#include <QString>
#include <QDockWidget>
#include <QTreeWidgetItemIterator>
#include <QCursor>
#include <QStyle>
#include <QIcon>
#include <QLinearGradient>
#include <QDateTime>
#include <QHeaderView>
#include <QScrollBar>
#include <QFont>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QFrame>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCursor>
#include <QModelIndex>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QFileDialog>
#include <QPropertyAnimation>

// Point Cloud Library
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <pcl/visualization/cloud_viewer.h>
#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/console/parse.h>

#include <pcl/common/centroid.h>  //��������
//#include <pcl/ModelCoefficients.h>

// Visualization Toolkit (VTK) 
#include <vtkRenderWindow.h>
#include <vtk3DWidget.h>


#include "DiyXYZRGBPoint.h"
#include "ThreadDelegate.h"

Q_DECLARE_METATYPE(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)
Q_DECLARE_METATYPE(LY::XYZRGBCloud)
Q_DECLARE_METATYPE(LY::THREAD_TYPE)

namespace Ui
{
	class PCLViewer;
}

class PCLViewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit PCLViewer(QWidget *parent = 0);
	~PCLViewer();

	/**
	* @brief ������Ҫ˵��-���Ժ���
	* @param index    ����1
	* @param t        ����2 @see CTest
	*
	* @return ����˵��
	*     -<em>false</em> fail
	*     -<em>true</em> succeed
	*/

	//��ʼ������
	void initPara();
	void inintTableWidget();
	void initConnector();

	//���в���
	void setBarValue(LY::THREAD_TYPE, int);  //���ý���������
	void addNewPointCloud(LY::XYZRGBCloud);
	void showMultiCloud(std::vector<QString>);
	void updateAttributes(std::vector<QString>);
	virtual bool event(QEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	void closeEvent(QCloseEvent *event);
    void logExport(QString );

	void keyboardEventOccurred(const pcl::visualization::KeyboardEvent &event, void* viewer_void);
	void mouseEventOccurred(const pcl::visualization::MouseEvent &event, void* viewer_void);

protected:
	virtual void keyPressEvent(QKeyEvent *event);

public slots:
	
	///���ļ�����Ӧ����-----------------------
	void actionOpenFile();
	void actionSaveAs();
	void actionExit();

	///�����ߡ���Ӧ����-----------------------
	void actionZoomIn();
	void actionZoomOut();
	void actionZoom();
	void actionCompressCloud();
	void actionClusteCloud();
	void actionKdtreeKNN();
	void actionKdtreeRadius();
	void actionVoxelization();
	void actionTrimCloud();
	void actionScreenShot();

	///���˲�����Ӧ����----------------------
	void actionPassThroughFilter();
	void actionVoxelCloud();
	void actionOutlinerRemove();
	void actionProjection();
	void actionGrayImage();

	///����ͼ����Ӧ����----------------------
	void actionViewer1();
	void actionViewer2();
	void actionViewer4();

	///������ת������Ӧ����------------------
	void actionLAS2TXT();
	void actionTXT2PCDASCII();
	void actionPCDBINARY2PCDASCII();
	void actionPCDASCII2TXT();

	///���״����ݴ�����Ӧ����--------------
	void actionIntensityFix();
	void actionKdTreeCluster();
	void actionBinarySignal();
	void actionImage2Cloud();
	
	///����������Ӧ����---------------------
	void actionSetting();
	void actionAbout();
	void actionHelp();

	///����������Ӧ����-----------------------------------
	void treeItemSelected(QTreeWidgetItem*, int);
	void showSelectedItemCloud(QTreeWidgetItem*, int);
	void showMenu(QPoint pos);

signals:
	void delegaterStartRunTask(LY::THREAD_TYPE, LY::XYZRGBCloud &, std::vector<QString>);
	
private:
	Ui::PCLViewer *ui;

	//�������� viewer
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer1, viewer2,viewer3; ///�����Ӵ�

	//�����ڵ����йصı���
	std::vector<LY::XYZRGBCloud> vecCloud;   ///���ص��ڴ��е����е���
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pclLogo;  ///Ԥ���ص�PCL����
	QMap<QString, QString> mapXYZRGB;  ///�Ѽ��صĵ����б�
	int cloudNumOfViewer1;  ///PCLviewer1 �е�չʾ�ĵ�������
	int pointSize; ///���ƵĴ�С
	ThreadDelegate * threadDelegater; ///��̨��פ�߳�
	QThread *threadGUI; ///QThread

	//����tablewidget�õ���
	std::vector<QString> vecTable; ///ˢ�� QTableWidget �õ���
		 
	//˫���������õ���
	QRect desktopRect, *rect; ///���������С
	int iTitleBarHeight; ///�������ĸ߶�

	//treeWidget �õ���
	LY::XYZRGBCloud * selectedCloud;  ///��¼ѡ�еĵ���

	//����̨����õ���
	QStandardItemModel *itemModel;  ///����̨����ģ��
	QStringList consoleList;///��ӡ��Ϣ�б�

};

