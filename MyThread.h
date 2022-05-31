#pragma once
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QElapsedTimer>
#include<QDebug>
#include<QThread>
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
	
	void Process(QByteArray data);
public slots:
	QByteArray Read(QByteArray data);
	void begin(QByteArray data);
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

};
#endif // !MYTHREAD_H

