#include "ThreadBinarySignal.h"

ThreadBinarySignal::ThreadBinarySignal(QObject *parent)
	: QObject(parent)
{
}

ThreadBinarySignal::~ThreadBinarySignal()
{
}
void ThreadBinarySignal::runTask(){
	///���Ĵ�����

	emit finishConvert();
}
