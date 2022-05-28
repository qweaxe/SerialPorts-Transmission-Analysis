#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <qvalidator.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include<QBuffer>
#include<QtWidgets/QMessageBox>
#include<qtimer.h>
#include<Windows.h>
#include<QFile>
#include"MyThread.h"
#include<QThread>
#include"LDDriver.h"
#include"CircularBuffer.h"


class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget* parent = Q_NULLPTR);

    //QtWidgetsApplication1();
    void DisplayData(const QByteArray buf);
    void AnalysisData(const QByteArray buf);
    void SetData();
    void QStringtoHex(QString str, QByteArray& SendData);//貌似没有
    char ConvertHexChar(char ch);
    QString AddBlank(QByteArray& arr);//不止，还有自动大写等
    void Checksum(QByteArray& data);
    void CRC16Checksum(QByteArray& data);
    char* ConverttoHex(int value);//没用
    Status Enlaser();
    Status Dislaser();
    void Changeui();
    bool SendDatabyte(const int com,  QByteArray senddata);
    CircularBuffer circularBuffer;
signals:
    void sigLabelChange();
    void sigAmpStatusChange();
    void starting1(int num);
    void starting2(QByteArray data);

public slots:
    void on_pushButton_toggled(bool checked);
    void change_ampstatus();

    //void COM0Changed();
    //void COM1Changed();
    //void COM2Changed();
    //void COM3Changed();
    //void COM4Changed();
    //void COM5Changed();
    //void COM6Changed();
    //void COM7Changed();

private slots:
    void on_ComComboBox_currentIndexChanged(int index);

    void on_GetcomButton_clicked();

    void on_SetCOM0Button_clicked();

    void ReadData();
    void on_SeedPowerEdit_returnPressed();

    void on_EnlaserButton_toggled(bool checked);

    void on_SetMainButton_clicked();

    void on_SetCOM1Button_clicked();

    void on_SetCOM2Button_clicked();

    void on_SetCOM3Button_clicked();

   //void on_SetAmp4Button_clicked();



    void LaserStatusQuery();

    void on_GetLDStatusBtn_toggled(bool checked);

    void on_SetCOM4Button_clicked();

    void on_SetCOM5Button_clicked();

    void on_SetCOM6Button_clicked();

    void on_SetCOM7Button_clicked();

    void on_COMDelayEdit_returnPressed();

    void on_setCOM39WBtn_clicked();

private:
    Ui::QtWidgetsApplication1Class ui;
    Status Comstatus = off;
    Status Laserstatus = off;
    Status Seedstatus = off;
    Status Amp1status = off;
    Status Amp2status = off;
    Status Amp3status = off;
    Status Amp4status = off;
    Status Mainstatus = off;
    Status LaserQuery = off;
    bool isSend = 0;
    QtLambdapump Seed;
    QtLambdapump Amp1;
    QtLambdapump Amp2;
    QtLambdapump Amp3;
    QtGolightpump Amp4;
    QtGolightpump Amp5;
    QtGolightpump Amp6;
    QtGolightpump Amp7;
    QtCotek Mainamp;

    QSerialPort* serial;//全局的串口对象
    QBuffer buffer;//
    QByteArray Senddata;
    QByteArray Header;
    QByteArray Comandlen;
    QByteArray Ender;
    QByteArray SetSeedData;
    QByteArray SetAmp1Data;
    QByteArray SetAmp2Data;
    QByteArray SetAmp3Data;
    QByteArray SetAmp4Data;
    QByteArray SetMainData;
    QByteArray SetSeedzero;
    QByteArray SetAmp1zero;
    QByteArray SetAmp2zero;
    QByteArray SetAmp3zero;
    QByteArray SetAmp4zero;
    QByteArray SetMainzero;
    QByteArray temp;
    QByteArray temp1;
    QByteArray checksum;

    QTimer* timer = new QTimer(this);
    QTimer* sendtime = new QTimer(this);
    qint64 maxcommand = 40;
    int curmin = 0;
    int seedcurmax = 1000;
    int amp1curmax = 1000;
    int amp2curmax = 1000;
    int amp3curmax = 1000;
    double amp4curmin = 0.0;
    double amp4curmax = 10.0;
    double mainmin = 0.0;
    double mainmax = 10.0;//存在问题，稍后再修改
    int sleeptime = 30;
    QString txtfilename = "D:/zzz/Datareceive.txt";
    QFile redatatxt;
    QTextStream stream;
   
    //先试试不new的版本,行不通
    QThread* t1=new QThread;
    QThread* t2=new QThread;

    //创建任务类的对象
    MainThread* Mainwork=new MainThread;
    ProcessThread* Processwork=new ProcessThread;
};

