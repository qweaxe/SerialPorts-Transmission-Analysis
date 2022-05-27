#include "MyThread.h"

MainThread::MainThread(QObject* parent) :QObject(parent)
{

}

void MainThread::begin(int num)
{
	qDebug() << "主线程：" << num << " || " << QThread::currentThread();
	QElapsedTimer time;
	time.start();
	for (int i = 0; i < num; i++)
	{
		QThread::msleep(1);//处理任务
		emit working(i);
	}
	qDebug() << "主线程，共耗时： " << time.elapsed() << "ms。";
	emit finish(QString::number(time.elapsed()));
}

ProcessThread::ProcessThread(QObject* parent) :QObject(parent)
{

}

void ProcessThread::begin(QByteArray data)
{
	qDebug() << "处理线程：" << " || " << QThread::currentThread();
	QElapsedTimer time;
	time.start();
	//for (int i = 0; i < num; i++)
	//{
	//	QThread::msleep(1);//处理任务
	//	
	//	emit working(i);
	//}
	processdata=Read(data);
	Process(processdata);

	qDebug() << "处理线程，共耗时： " << time.elapsed() << "ms。";
	emit finish(QString::number(time.elapsed()));//改变一下，不在这里销毁，仅仅作为结果输出
}

QByteArray ProcessThread::Read(QByteArray data)
{
	int length = data.size();
	if (data[0] != 0xAA && data[1] != 0x55)
	{
		
		buffer = data;//这里处理有问题，需要修改，没有处理要舍弃的情况，或者是多余的情况，检测是否有 AA 55，分两种情况


	}

	else if (data[3]!=length-7)
	{
		if (data[3] > length - 7)
		{
			buffer = data.sliced(((uint)data[3]) + 7);
			//data.chop(length-(((uint)data[3]) + 7));
			data.truncate(((uint)data[3]) + 7);
		}

		else if (data[3] < length - 7)
		{
			buffer = data;
		}
	}
	return data;
}

void ProcessThread::Process(QByteArray data)
{
	switch (data[2])
	{
	case 0x00://com0
		//Amp0.ReInfoData(data);
		break;
	case 0x01://com1

		break;
	case 0x02://com2

		break;
	case 0x03://com3

		break;
	case 0x04://com4

		break;
	case 0x05://com5

		break;
	case 0x06://com6

		break;
	case 0x07://com7

		break;
	default:
		break;
	}
}
