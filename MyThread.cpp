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
	connect(this, &ProcessThread::ReadyProcess, this, &ProcessThread::Process);
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
	emit DisplaySeData(data);
	serialport->waitForBytesWritten(10);
	qDebug() << "Current Thread is : " << QThread::currentThread();
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
//void ProcessThread::ReceviceSerialData()
//{
//	buffer = serialport->readAll();
//	qDebug() << QString(buffer) << "当前线程ID：" << QThread::currentThreadId();
//
//}
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

bool ProcessThread::SetSerialPort(QString comName)
{
	//SetSerialPortList();
	//if (checked)
	//{
		qDebug() << "Thread is ：" << " || " << QThread::currentThread();
		//serialport = new QSerialPort();
		//设置串口名，不在这设置了
		//serial->setPortName(ui.ComComboBox->currentText());
		serialport->setPortName(comName);
		//serialport->setPortName("COM10");
		//打开串口，读写
		bool stat=serialport->open(QIODevice::ReadWrite);
		//设置波特率，后面改成可变的
		serialport->setBaudRate(QSerialPort::Baud115200);
		//设置数据位数，后面改成可变的
		serialport->setDataBits(QSerialPort::Data8);
		//设置校验位，后面改成可变的
		serialport->setParity(QSerialPort::NoParity);
		//设置停止位，后面改成可变的
		serialport->setStopBits(QSerialPort::OneStop);
		//设置流控制
		serialport->setFlowControl(QSerialPort::NoFlowControl);
		//设置读取数据的缓存大小，后面改成可变的
		serialport->setReadBufferSize(1024);
		////关闭设置菜单使能（好像不要这个功能）
		//连接信号槽，点开始通信后就开始，下位机一有数据发送过来就会相应此槽函数：connect（ob1，sig1，ob2，slot1）
		QObject::connect(serialport, &QSerialPort::readyRead, this, &ProcessThread::Read);


		//QByteArray Test = Seed.Datasend();
		qDebug() << "portName() = " << serialport->portName();
		qDebug() << "isOpen = " << serialport->isOpen();
		qDebug() << "Error is " << serialport->error();
	return stat;
}

void ProcessThread::CloseSerialPort()
{
	if (serialport->isOpen())
	{
		serialport->clear();
		serialport->close();
	}
	qDebug() << "串口关闭状态 : "<<serialport->isOpen();
}

void ProcessThread::Read()//改返回值为bool型或者别的
{
	int head = buffer.size();//添加一个变量指向当前buffer存储的位置，即当前添加后的第一个位置
	buffer.append(serialport->readAll());
	int length = buffer.size();
	//想想怎么分情况讨论
	// 
	//分情况讨论，三种情况
		//1°半句+一句：前面的残留以及后面的完整一句（或者不完整） 
		// 
		//2° 只有半句：后面的剩余不一定完整，要分情况讨论；如果接的后半句+buffer为空，直接舍弃掉，不空就保留；如果为前半句，先保留；
		// 
		//3°完整一句：直接截取一句
	    //
	    //4°全是残缺，55 AA 在中间：

		//这里处理有问题，需要修改，没有处理要舍弃的情况，或者是多余的情况，检测是否有 AA 55，分两种情况
	if (buffer.size() == 0) {
		return;
	}
	else if (head != 0) {//原本里面有缓存
		if ((unsigned char)buffer[0] == 0xAA && (unsigned char)buffer[1] == 0x55) {
			if ((buffer.size() == (unsigned char)buffer[3] + 7)||((buffer.size()>(unsigned char)buffer[3]+7)&&buffer[(unsigned int)buffer[3]+7]==0xAA)) {//1.半句+半句；2.半句+多余的，凑成一句+开头
				qDebug() << "输出的字符串为：" << buffer.chopped(length - ((unsigned char)buffer[3] + 7));
				//emit ReadyProcess(buffer.chopped(length - ((unsigned char)buffer[3] + 7)));
				data = buffer.chopped(length - ((unsigned char)buffer[3] + 7));//chop：前半部分
				buffer=buffer.sliced((unsigned char)buffer[3] + 7);//slice:从该位置开始的后半部分
			}
			else {//半句+一句多,不考虑后续超过一句的情况？
				int head1 = buffer.indexOf(0xAA, head);
				int head2 = buffer.indexOf(0x55, head);
				if (head1 != -1 && head2 != -1) {
					buffer = buffer.sliced(head1);
					if (buffer.size() != (unsigned char)buffer[3] + 7) {

					}
				}
			}
			//保留剩下的部分
		}
	}
	else if ((unsigned char)buffer[head] != 0xAA || (unsigned char)buffer[head + 1] != 0x55) {//1°, 4°, 2°.1
		//两种情况：1.单纯半条 1.1前半条 1.2 后半条; 2.半条+一条
		int headAA = buffer.indexOf(0xAA, head+1);
		int head55 = buffer.indexOf(0x55, head+2);
		if (headAA != -1 && head55 != -1 && headAA == head55 + 1) {
			buffer = buffer.sliced(headAA);
			if (buffer.size() == (unsigned char)buffer[3] + 7) {//感觉条件会出问题
				data = buffer;
				buffer.clear();
				buffer.resize(0);
			}
		}
		else {
			buffer.clear();
			buffer.resize(0);
		}
	}
	else if(head==0&&(unsigned char)buffer[head]==0xAA&&(unsigned char)buffer[head+1]==0x55){//2°.2, 3°
		if (length != (unsigned char)(buffer[head + 3]) + 7) {

			return;
		}
		data = buffer;
		buffer.clear();//已经扩展成固定的长度但是clear不会改变其长度
		buffer.resize(0);
	}
	//else if ((unsigned char)buffer[head]==0xAA&&(unsigned char)buffer[head+1]==0x55&& length != (unsigned char)(buffer[head + 3]) + 7) {

	//	return;
	//}
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
	qDebug() << QString(buffer) << "当前线程ID：" << QThread::currentThreadId();
	//改成发送信号通知可以进行后续处理了
	emit ReadyProcess(data);
	emit DisplayReData(data);
	return ;
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
	if (data[0]!=0xAA&&data[1]!=0x55)//判断字头，判断放到read函数中去，至少部分要放进去以判断长度等
	{
		
		qDebug() << "帧头不对，处理函数中接收到的：" << data;
		data.clear();
		data.resize(0);
		return;

	}
	else if (data[3]!=length-7)//长度校验
	{
		//if (data[3]>length-7)
		//{

		//}
		//else if (data[3]<length-7)
		//{

		//}
		qDebug() << "长度不对，处理函数中接收到的：" << data;
		data.clear();
		data.resize(0);
		return;

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
			Amp2->ReInfoData(data);
			emit processed(2, Amp2);
			break;
		case 0x03://com3
			Amp3->ReInfoData(data);
			emit processed(3, Amp3);
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
		data.clear();
		data.resize(0);
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


