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
	//emit finish(QString::number(time.elapsed()));
}

ProcessThread::ProcessThread(QObject* parent) :QObject(parent)
{

}

void ProcessThread::begin()
{
	qDebug() << "处理线程开始工作：" << " || " << QThread::currentThread();
	time.start();
	//for (int i = 0; i < num; i++)
	//{
	//	QThread::msleep(1);//处理任务
	//	
	//	emit working(i);
	//}
	//processdata=Read(data);//读和处理不要放到初始函数上
	//Process(processdata);
	SetSerialPortList();
	qDebug() << "处理线程，共耗时： " << time.elapsed() << "ms。";
	//emit finish(QString::number(time.elapsed()));//改变一下，不在这里销毁，仅仅作为结果输出
}

void ProcessThread::send()//发送函数，改成bool？
{

}

void ProcessThread::SetSerialPort(bool checked)
{
	if (checked)
	{

		serialport = new QSerialPort;
		//设置串口名，不在这设置了
		//serial->setPortName(ui.ComComboBox->currentText());
		
		//打开串口，读写F
		//serial->open(QIODevice::ReadWrite);
		serialport->open(QIODevice::ReadWrite);
		//设置波特率，后面改成可变的
		//serial->setBaudRate(QSerialPort::Baud115200);
		serialport->setBaudRate(QSerialPort::Baud115200);
		//设置数据位数，后面改成可变的
		//serial->setDataBits(QSerialPort::Data8);
		serialport->setDataBits(QSerialPort::Data8);
		//设置校验位，后面改成可变的
		//serial->setParity(QSerialPort::NoParity);
		serialport->setParity(QSerialPort::NoParity);
		//设置停止位，后面改成可变的
		//serial->setStopBits(QSerialPort::OneStop);
		serialport->setStopBits(QSerialPort::OneStop);
		//设置流控制
		//serial->setFlowControl(QSerialPort::NoFlowControl);
		serialport->setFlowControl(QSerialPort::NoFlowControl);
		//设置读取数据的缓存大小，后面改成可变的
		//serial->setReadBufferSize(1024);
		serialport->setReadBufferSize(1024);
		////关闭设置菜单使能（好像不要这个功能）
		//连接信号槽，点开始通信后就开始，下位机一有数据发送过来就会相应此槽函数：connect（ob1，sig1，ob2，slot1）
		//QObject::connect(serial, &QSerialPort::readyRead, this, &QtWidgetsApplication1::ReadData);
		QObject::connect(serialport, &QSerialPort::readyRead, this, &ProcessThread::Read);
		////多线程版本，2022年10月8日，需要修改
		////QObject::connect(serial, &QSerialPort::readyRead, Processwork, &ProcessThread::Read);
		//Comstatus = on;
		////图标提示
		//ui.statuslabel->setPixmap(QPixmap(":/images/on.png"));
		//ui.pushButton->setText(tr("关闭串口"));
		////connect，循环访问信息
		//QObject::connect(timer, &QTimer::timeout, this, &QtWidgetsApplication1::LaserStatusQuery);

		//QByteArray Test = Seed.Datasend();
	}
	else
	{
		//timer->stop();
		//serial->clear();
		serialport->clear();
		serialport->close();
		serialport->deleteLater();
		//serial->close();
		//serial->deleteLater();
		//Comstatus = off;
		////图标提示
		//ui.statuslabel->setPixmap(QPixmap(":/images/off.png"));
		//ui.pushButton->setText(tr("开启通讯"));
		//ui.GetLDStatusBtn->setChecked(0);//用信号机制去修改ui？
		//ui.EnlaserButton->setChecked(0);
		//ui.GetLDStatusBtn->setText(tr("开始获取"));
		//ui.EnlaserButton->setText(tr("使能激光器"));
	}
}

void ProcessThread::SetSerialPortList()
{
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		//QSerialPort serial;
		serial.setPort(info);
		if (serial.open(QIODevice::ReadWrite))
		{
			//ui.ComComboBox->addItem(serial.portName());
			emit ProcessThread::SerialPortChanged(serial.portName());
			serial.close();
		}
	}
}

QByteArray ProcessThread::Read()
{
	buffer.append(serialport->readAll());
	int length = buffer.size();
	//想想怎么分情况讨论
	
	//if (data[0] != 0xAA && data[1] != 0x55)
	//{
	//	
	//	buffer = data;

	//}

	//else if (data[3]!=length-7)
	//{
	//	if (data[3] > length - 7)
	//	{
	//		buffer = data.sliced(((uint)data[3]) + 7);
	//		//data.chop(length-(((uint)data[3]) + 7));
	//		data.truncate(((uint)data[3]) + 7);
	//	}

	//	else if (data[3] < length - 7)
	//	{
	//		buffer = data;
	//	}
	//}
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

void ProcessThread::SerialPortName(QString text)
{
	serialport->setPortName(text);
}


