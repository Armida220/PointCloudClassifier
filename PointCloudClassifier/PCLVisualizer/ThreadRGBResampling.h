#pragma once

#include <QThread>


/*��
* @class  ThreadRGBResampling  ��ά������ɫ�ز���
*
* @brief  �ڶ�άƽ����ѡ�̿��Ƶ㣬������ά���ƣ�����  Z == 0��
*
*	��1����ά����������Ϊģ�棬�����ݸ�ʽΪ  X Y Z R G B
*	��2��������ά�������ݣ������ݸ�ʽΪ X Y Z Intensity returnsNum
*	��3��������ά���ƣ��ڶ�άƽ�����ҵ�����������Ķ��䣬Ȼ�󽫸õ�� R G B
*	��4�����txt�ĵ����ļ�    X Y Z Intensity returnsNum height R G B bin
*/	

class ThreadRGBResampling : public QThread
{
	Q_OBJECT

public:
	ThreadRGBResampling(QObject *parent = nullptr);
	~ThreadRGBResampling();

	void run();
	void split(std::string &str, std::vector<std::string> & result, std::string pattern);
	int getTxtLineCount(std::string file_name);

signals:

	void finished();
};
