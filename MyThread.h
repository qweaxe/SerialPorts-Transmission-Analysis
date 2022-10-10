#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QElapsedTimer>
#include<QDebug>
#include<QThread>
#include<qtimer.h>
#include"LDDriver.h"

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
	
	void Process(QByteArray data);
	QSerialPort* port;

public slots:
	QByteArray Read(QByteArray data);
	void begin();//需要在子线程分配的资源，全部在此函数进行，该槽函数绑定到线程的started()信号上
	void send();//数值还是直接发整理好的内容？
signals:
	void working(int num);
	void finish(QString elapsedTime);//可以传递多个参数，槽会匹配并忽略多余的参数
	void processed(int n, QtLambdapump* Amp);

private:
	QByteArray buffer;
	QByteArray processdata;
	QtLambdapump *Amp0=new QtLambdapump;
	QtLambdapump *Amp1=new QtLambdapump;
	QtLambdapump Amp2;
	QtLambdapump Amp3;
	QtGolightpump Amp4;
	QtGolightpump Amp5;
	QElapsedTimer time;
	QTimer* timer;
};


#endif // !MYTHREAD_H

