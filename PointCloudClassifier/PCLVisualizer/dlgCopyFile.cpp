#include "dlgCopyFile.h"
#include "ui_dlgCopyFile.h"

#include <QFileDialog>
#include <QString>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QRadioButton>
#include <QFile>
#include <QFileInfo>

dlgCopyFile::dlgCopyFile(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::dlgCopyFile();
	ui->setupUi(this);

	this->setFixedSize(550, 300);

	connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(toolClicked1()));
	connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(toolClicked2()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(convertButton()));
}

dlgCopyFile::~dlgCopyFile()
{
	delete ui;
}

void dlgCopyFile::toolClicked1(){
	QString path = QFileDialog::getOpenFileName(this, QString(), QString("C:/"), "(*.*)");
	ui->lineEdit->setText(path);
	//file_name = QFileInfo(path).fileName();
}

void dlgCopyFile::toolClicked2(){ ///������
	QString path = QFileDialog::getExistingDirectory(this, QString(), QString("C:/"));
	ui->lineEdit_2->setText(path);
}

QString dlgCopyFile::replaceSymbol(std::string &str){
	std::string::size_type pos(0);
	while ((pos = str.find_first_of('\\', pos)) != std::string::npos){
		str.insert(pos, "\\");
		pos = pos + 2;
	}
	return QString::fromStdString(str);
}

void dlgCopyFile::convertButton(){
	if (ui->lineEdit->text() == "" ||
		ui->lineEdit_2->text() == "")
		return;

	ui->pushButton->setEnabled(false);

	//�����ļ����ж�
	//���·���а��� ��/����ֱ����ȡ�ļ���
	//���·���а����� \��������ȡ�ļ������� "\"ȫ���滻Ϊ��/��
	QString srcPath = ui->lineEdit->text();
	bool flag = true;
	std::string::size_type pos(0);
	while ((pos = srcPath.toStdString().find_first_of("/", pos)) != std::string::npos){
		flag = false;
		pos++;
		pos = srcPath.toStdString().find_last_of("/", pos) + 1;
		file_name = QString::fromStdString(srcPath.toStdString().substr(pos, srcPath.toStdString().length() - 1));
	}

	if (flag == true){
		// �Ƚ� \  ת��Ϊ \\ 
		pos = 0;
		srcPath = replaceSymbol(ui->lineEdit->text().toStdString());
		while ((pos = srcPath.toStdString().find_first_of("\\", pos)) != std::string::npos){
			pos++;
			pos = srcPath.toStdString().find_last_of("\\", pos) + 1;
			file_name = QString::fromStdString(srcPath.toStdString().substr(pos, srcPath.toStdString().length() - 1));
		}
	}
	
	//��������ļ���
	QString dstPath;
	dstPath = ui->lineEdit_2->text() + "/" + file_name;

	//��ʼ����
	if (!QFile::copy(srcPath, dstPath)){
		ui->pushButton->setEnabled(false);
	}

	ui->pushButton->setEnabled(true);
}

void dlgCopyFile::keyPressEvent(QKeyEvent *event){
	switch (event->key()){
	case Qt::Key_Escape:
		break;
	default:
		QDialog::keyPressEvent(event);
		break;
	}
}