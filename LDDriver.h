#pragma once
#ifndef LDDRIVER_H_
#define LDDRIVER_H_

#include<QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include<bitset>
#include<QDebug>

enum Status
{
    off,
    on,
    broken
};
union {
    float f;
    quint8 buf[4];
}myfloat;

class QtLambdapump :public QObject//:public QtWidgetsApplication1
{
public:
    QtLambdapump();
    void Setcurrent(const int value);//要不基类写个虚函数要子类去重构？
    void Set27Wcurrent(const double value);
    void Set9Wcurrent(const double value);
    void StatusQuery();
    void ReInfoData(const QByteArray data);
    void OnstatusQuery();
    void MMOnstatusQuery();
    void MMstatusQuery();
    void SetStatus(QtLambdapump* Amp);
    Status& Ampstatus() { return ampstatus; }
    int Pumpcurrent() { return repumpcurrent; }
    double Pumptemp() { return repumptemp; }
    int Pumppower() { return repumppower; }
    int PD1power() { return pd1power; }
    int PD2power() { return pd2power; }
    int PD3power() { return pd3power; }
    int PD4power() { return pd4power; }
    double Modeltemp() { return rebacktemp; }
    double LD27wtemp() { return reld27wtemp; }
    double LD27wcur() { return reld27wcur; }
    double LD9wcur() { return reld9wcur; }
    double LD27wsetcur() { return reld27wsetcur; }
    double LD9wsetcur() { return reld9wsetcur; }
    QByteArray& Datasend() { return datasend; }
    
    //~QtLambdapump();//
private:
    Status ampstatus = off;
    QByteArray datasend;//发送的数据
    QByteArray datareceive;//收到的数据
    QByteArray setzero;//直接电流设置为0
    int setcurvalue;//设置的电流
    int resetcurrent;//返回指令中设置的电流
    int resettectemp;//返回指令中设置的温度
    int repumpcurrent;//返回指令中ld的电流
    int repumppower;//返回指令中ld功率
    int reteccurrent;//返回指令中tec电流
    double remodeltemp;//返回指令中背板温度
    double repumptemp;//返回指令中多模LD（27W）温度
    int pd1power;//返回指令中pd1功率
    double rebacktemp;//背板温度
    double retectemp;//tec温度
    double recavtemp;//腔温度
    int pd2power;//pd2功率
    int pd3power;//pd3功率
    int pd4power;//pd4功率
    double reld27wtemp;//多模ld（27w）温度
    double reld27wcur;//多模ld（27w）电流
    double reld9wcur;//多模ld（9w）电流
    double reld27wsetcur;//多模ld（27w）设置电流
    double reld9wsetcur;//多模ld（9w）设置电流

};

class QtDhkjpump : public QObject//:public QtWidgetsApplication1
{
public:
    QtDhkjpump();
    void Onoffenginmode(bool s);
    void Onofflaser(bool s);
    void OnoffstatusQuery();
    void SetLD1current(float value);
    void SetLD2current(float value);
    void ReInfoData(const QByteArray data);
    void LD1curQuery();
    void LD2curQuery();
    void LD1SetcurQuery();
    void LD2SetcurQuery();
    void LD1maxcurQuery();
    void LD2maxcurQuery();
    float LD1current() { return repump1cur; }
    float LD2current() { return repump2cur; }
    Status& Ampstatus() { return ampstatus; }
    QByteArray Datasend() { return datasend; }
   void floatToIEEE754(float value);
   void SetStatus(QtDhkjpump* Amp);
private:
    Status ampstatus = off;
    Status enginmodes = off;
    QByteArray datasend;
    QByteArray datareceive;
    float setpump1cur;
    float setpump2cur;
    float resetpump1cur;
    float resetpump2cur;
    float repump1cur;
    float repump2cur;

};
class QtDhkjMMpump : public QObject//
{
public:
    QtDhkjMMpump();
    void Onofflaser(bool s);
    void SetMMLDcurrent(float value);
    void ReInfoData(const QByteArray data);
    void MMLDcurQuery();
    float MMLDcurrent() { return repumpcur; }
    Status& Ampstatus() { return ampstatus; }
    QByteArray Datasend() { return datasend; }
    void floatToIEEE754(float value);
    void SetStatus(QtDhkjMMpump* Amp);
private:
    Status ampstatus = off;
    QByteArray datasend;
    QByteArray datareceive;
    float resetpumpcur;
    float repumpcur;

};
class QtGolightpump :public QSerialPort//:public QtWidgetsApplication1
{
public:
    QtGolightpump();
    void SetData();
    void Setcurrent(const int value);
    void CurrentQuery();
    void TempQuery();
    void BacklightQuery();
    void ReInfoData(const QByteArray data);
    void Onofflaser(const QByteArray data);
    Status& Ampstatus() { return ampstatus; }
    int Ampcurrent() { return repumpcurrent; }
    int Ampsetcur() { return resetcurrent; }
    int Amppower() { return repumppower; }
    double Amptemp() { return repumptemp; }
    QByteArray& Datasend() { return datasend; }
    int setcurvalue;
private:
    Status ampstatus = off;
    QByteArray datasend;
    QByteArray datareceive;
    QByteArray setzero;
    int resetcurrent;
    int repumpcurrent;
    int repumppower;
    double repumptemp;
};

class QtCotek :public QSerialPort  //:public QtWidgetsApplication1
{
public:
    QtCotek();
    void Setcurrent(QByteArray value);
    void OutputCurrentQuery();//试试这样有问题吗,其实可以列为常量的
    void OutputVoltageQuery();
    void TempQuery();
    void ReInfoData(const QByteArray data);
    void OnOffstatusQuery();
    Status DeviceStatusQuery();
    Status OnOffDevice();
    Status& Ampstatus() { return ampstatus; }
    double ampcurrent() { return repumpcurrent; }
    int ampsetcur() { return resetcurrent; }
    int amptemp() { return retemp; }

    QByteArray Datasend() { return datasend; }
private:
    Status ampstatus = off;
    QByteArray datasend;
    QByteArray datareceive;
    QByteArray setzero;
    int resetcurrent;
    double repumpcurrent;
    double repumpvoltage;
    double retemp;
};

//QtLambdapump Com0;
//QtLambdapump Com1;
//QtLambdapump Com2;
//QtLambdapump Com3;
//QtGolightpump Com4;
//QtGolightpump Com5;
//QtGolightpump Com6;
//QtGolightpump Com7;

#endif // !LDDRIVER_H_


