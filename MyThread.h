#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QElapsedTimer>
#include<QDebug>
#include<QThread>
#include<qtimer.h>
#include"LDDriver.h"
#include<Windows.h>

class MainThread :public QObject
{
	Q_OBJECT
public:
	explicit MainThread(QObject* parent = nullptr);
	void begin(int num);
signals:
	void working(int num);
	void finish(QString elapsedTime);

};

class ProcessThread :public QObject
{
	Q_OBJECT
public:
	explicit ProcessThread(QObject* parent = nullptr);
	
	//~ProcessThread();
	void SetData();

	QSerialPort* port;
	void Checksum(QByteArray& data);
	void CRC16Checksum(QByteArray& data);
	void DhkjChecksum(QByteArray& data);
	QStringList GetSerialAvailable();

public slots:
	void Read();
	
	void Process(QByteArray data);//接受完之后的处理函数
	void begin();//需要在子线程分配的资源，全部在此函数进行，该槽函数绑定到线程的started()信号上
	bool send(const int com, QByteArray data);//数值还是直接发整理好的内容？
	bool SetSerialPort(QString comName);
	void CloseSerialPort();
	void SetSerialPortList();
	void SerialPortName(QString text);

signals:
	void working(int num);
	void finish(QString elapsedTime);//可以传递多个参数，槽会匹配并忽略多余的参数
	void processed(int n, QtDhkjpump* Amp);
	void mmprocessed(int n, QtDhkjMMpump* Amp);
	void SerialPortChanged(QString info);
	void ReadyProcess(QByteArray data);
	void DisplayReData(QByteArray data);
	void DisplaySeData(QByteArray data);

private:
	QByteArray buffer;
	QByteArray processdata;
	QByteArray data;
	QByteArray Senddata;
	QByteArray Comandlen;
	QByteArray Header;
	QByteArray Ender;
	QByteArray checksum;
	bool isSend = 0;
	QtDhkjpump *SeedandAmp1=new QtDhkjpump;
	QtDhkjpump*Amp2=new QtDhkjpump;
	QtDhkjpump*Amp3=new QtDhkjpump;
	QElapsedTimer time;
	QTimer* timer;
	//QSerialPort serial;
	QSerialPort* serialport;
};


#endif // !MYTHREAD_H

