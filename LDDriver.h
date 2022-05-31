#pragma once
#ifndef LDDRIVER_H_
#define LDDRIVER_H_

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

enum Status
{
    off,
    on,
    broken
};

class QtLambdapump :public QSerialPort//:public QtWidgetsApplication1
{
public:
    QtLambdapump();
    void SetData();
    void Setcurrent(const int value);//要不基类写个虚函数要子类去重构？
    void SetMMcurrent(const double value);
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
    double Modeltemp() { return backtemp; }
    double LD27wtemp() { return ld27wtemp; }
    double LD27wcur() { return ld27wcur; }
    double LD9wcur() { return ld9wcur; }
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
    double backtemp;//背板温度
    double tectemp;//tec温度
    double cavtemp;//腔温度
    int pd2power;//pd2功率
    int pd3power;//pd3功率
    int pd4power;//pd4功率
    double ld27wtemp;//多模ld（27w）温度
    double ld27wcur;//多模ld（27w）电流
    double ld9wcur;//多模ld（9w）电流
    double ld27wsetcur;//多模ld（27w）设置电流
    double ld9wsetcur;//多模ld（9w）设置电流

};

class QtDhkjpump : public QSerialPort //:public QtWidgetsApplication1
{
public:
    QtDhkjpump();
    void SetData();
    void Setcurrent(double value);
    void SetcavTemp(double value);
    void ReInfoData(const QByteArray data);
    double Pumpcurrent() { return repumpcurrent; }
    double Pumppower() { return repumppower; }
    double Pumptemp() { return repumptemp; }
    double Tempcurrent() { return retempcur; }
    double Cavtemp() { return recavtemp; }
    Status& Ampstatus() { return ampstatus; }
    QByteArray Datasend() { return datasend; }
private:
    Status ampstatus = off;
    QByteArray datasend;
    QByteArray datareceive;
    QByteArray setzero;
    int setcurvalue;
    int resetcurrent;
    double repumpcurrent;
    double repumppower;
    double repumptemp;
    double retempcur;
    double recavtemp;
    int u3;
    int r;
    double R7;
    double lnR7;
    //data received
    QByteArray reseed[8];

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


