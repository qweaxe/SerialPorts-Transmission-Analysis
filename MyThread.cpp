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
	serialport = new QSerialPort;
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
	//SetSerialPortList();
	SetData();
	//qDebug() << "portName() = " << serialport->portName();
	//qDebug() << "isOpen = " << serialport->isOpen();
	//qDebug() << "Error is " << serialport->error();
	qDebug() << "处理线程，共耗时： " << time.elapsed() << "ms。";
	//emit finish(QString::number(time.elapsed()));//改变一下，不在这里销毁，仅仅作为结果输出
}

bool ProcessThread::send(const int com, QByteArray data)//发送函数，改成bool？
{
	Senddata.resize(1);
	Senddata[0] = 0x00;
	Comandlen[1] = 0x00;
	switch (com)
	{
	case 0x00:
		Comandlen[0] = 0x00;
		CRC16Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;
	case 0x01:
		Comandlen[0] = 0x01;
		CRC16Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;
	case 0x02:
		Comandlen[0] = 0x02;
		CRC16Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;

	case 0x03:
		Comandlen[0] = 0x03;
		CRC16Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;
	case 0x04:
		Comandlen[0] = 0x04;
		Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;

	case 0x05:
		Comandlen[0] = 0x05;
		Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;

	case 0x06:
		Comandlen[0] = 0x06;
		Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;

	case 0x07:
		Comandlen[0] = 0x07;
		Checksum(data);
		Comandlen[1] = data.size();
		data = Header + Comandlen + data + Senddata;
		break;
	default:
		//QMessageBox::warning(this, tr("警告⚠"), tr("Error Occured!"));

		break;
	}
	Checksum(data);
	data = data + Ender;
	qint64 size = serialport->write(data);
	serialport->waitForBytesWritten(10);
	Sleep(30);//考虑下需不需要这句话？
	//ui.SendtextEdit->clear();
	//ui.SendtextEdit->append(AddBlank(senddata));
	if (size == -1)
	{
		//QMessageBox::warning(this, tr("警告⚠"), tr("Error Occured!"));
		isSend = 0;
	}
	else
		isSend = 1;//不一定，可能发过去了那边没收到

	return isSend;

}
void ProcessThread::SetSerialPortList()
{
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		QSerialPort serialport;
		serialport.setPort(info);
		//emit ProcessThread::SerialPortChanged(info.portName());
		if (serialport.open(QIODevice::ReadWrite))
		{
			//ui.ComComboBox->addItem(serial.portName());
			qDebug() << "Thread is ："  << " || " << QThread::currentThread();
			qDebug() << "PortName is : " << serialport.portName();
			emit ProcessThread::SerialPortChanged(serialport.portName());
			serialport.close();
		}
	}
}

bool ProcessThread::SetSerialPort(bool checked)
{
	//SetSerialPortList();
	if (checked)
	{
		qDebug() << "Thread is ：" << " || " << QThread::currentThread();
		//serialport = new QSerialPort();
		//设置串口名，不在这设置了
		//serial->setPortName(ui.ComComboBox->currentText());
		//serialport->setPortName("COM10");
		//打开串口，读写F
		//serial->open(QIODevice::ReadWrite);
		bool stat=serialport->open(QIODevice::ReadWrite);
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
		//多线程版本，2022年10月8日，需要修改
		//QObject::connect(serial, &QSerialPort::readyRead, Processwork, &ProcessThread::Read);
		//Comstatus = on;
		////图标提示
		//ui.statuslabel->setPixmap(QPixmap(":/images/on.png"));
		//ui.pushButton->setText(tr("关闭串口"));
		////connect，循环访问信息
		//QObject::connect(timer, &QTimer::timeout, this, &QtWidgetsApplication1::LaserStatusQuery);

		//QByteArray Test = Seed.Datasend();
		qDebug() << "portName() = " << serialport->portName();
		qDebug() << "isOpen = " << serialport->isOpen();
		qDebug() << "Error is " << serialport->error();
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
	return stat;
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

void ProcessThread::SetData()
{
	Header.resize(2);
	Header[0] = 0x55;
	Header[1] = 0xAA;
	Ender.resize(2);
	Ender[0] = 0x0D;
	Ender[1] = 0x0A;
	Comandlen.resize(2);
	Comandlen[0] = 0x08;
	Comandlen[1] = 0x00;
	checksum.resize(1);
	checksum[0] = 0x00;
	Senddata.resize(1);
	Senddata[0] = 0x00;

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

void ProcessThread::Checksum(QByteArray& data)
{
	int length = data.size();
	//QByteArray checksum;
	//checksum.resize(1);
   // checksum[0] = 0x00;
	data[length - 1] = 0x00;
	unsigned char temp = data[length - 1];//2022年4月12日修改
	for (int i = 0; i < length; i++)//感觉data+=的重载有问题，所以用此方法
	{
		temp += data[i];
	}
	data[length - 1] = temp;
	checksum[0] = data[length - 1];
}

void ProcessThread::CRC16Checksum(QByteArray& data)
{
	int len = data.size();
	data.resize(len + 2);
	uint crc = 0xFFFF;
	uint high = 0;
	uint low = 0;
	for (int pos = 0; pos < len; pos++)
	{
		crc ^= (unsigned char)data[pos];//xor byte into least sig. byte of crc；不然会转换为有符号数
		for (int i = 8; i != 0; i--)
		{
			if ((crc & 0x0001) != 0)// Last is 1
			{
				crc >>= 1;//shift right and xor 0xA001
				crc ^= 0xA001;
			}
			else// last is 0
				crc >>= 1;//only shift right
		}
	}
	//高低字节转换
	//crc = ((crc & 0x00FF) << 8 | (crc & 0xFF00) >> 8);
	low = (crc & 0x00FF);
	high = ((crc & 0xFF00) >> 8);
	//data.resize(8);
	data[len] = low;
	data[len + 1] = high;
}

QStringList ProcessThread::GetSerialAvailable()
{
	QStringList mPortsList;
	foreach(const QSerialPortInfo & Info, QSerialPortInfo::availablePorts())
	{
		mPortsList << Info.portName();
	}
	return mPortsList;
}

void ProcessThread::SerialPortName(QString text)
{
	serialport->setPortName(text);
}


