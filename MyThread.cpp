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
	buffer.append(data);
	if (data[0] != 0xAA && data[1] != 0x55)
	{
		
		buffer = data;

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
	uint length = data.size();
	if (data[0]!=0xAA&&data[1]!=0x55)//判断字头
	{
		//分情况讨论，三种情况
		//1°半句+一句：前面的残留以及后面的完整一句（或者不完整） 
		// 
		//2° 只有半句：后面的剩余不一定完整，要分情况讨论
		// 
		//3°其他情况

		//这里处理有问题，需要修改，没有处理要舍弃的情况，或者是多余的情况，检测是否有 AA 55，分两种情况
		//不如在此直接read就完事了，处理放到后面去，切片，缓存等手续直接和处理分析一起



	}
	else if (data[3]!=length-7)//长度校验
	{
		if (data[3]>length-7)
		{

		}
		else if (data[3]<length-7)
		{

		}

	}
	//校验和校验，先省略

	else//正常情况
	{
		switch (data[2])
		{
		case 0x00://com0
			Amp0->ReInfoData(data);
			emit processed(0, Amp0);

			break;
		case 0x01://com1
			Amp1->ReInfoData(data);
			emit processed(1, Amp1);
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
	
}
