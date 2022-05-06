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
//#include<Include.h>

#ifndef QCIRCULARBUFFER_CHUNKSIZE
#define QCIRCULARBUFFER_CHUNKSIZE 1024
#endif
//enum Status
//{
//    off,
//    on,
//    broken
//};

enum
{
    //1G-1byte
    MaxAllocSize = (1 << (std::numeric_limits<int>::digits - 1)) - 1
};
enum
{
    //1G-1-16bytes
    MaxByteArraySize = MaxAllocSize - sizeof(QtPrivate::qRemovePointerLike_t<QByteArray::DataPointer>::parameter_type)
};
class CircularBuffer :public QByteArray
{
public:
    //默认分配QCIRCULARBUFFER_CHUNKSIZE大小的buffer
    CircularBuffer(int growth = QCIRCULARBUFFER_CHUNKSIZE) :begin(0), end(0), endBuffer(0), basicBlockSize(growth), buffersize(0) {}
    ~CircularBuffer() { }
//signals:
//    void DataChanged(QByteArray newData);
    bool isEmpty() { return buffersize == 0; }//判断是否为空
    //bool isFull() const;
    //获取环形缓冲区指定位置的指针，length：输出这个指定位置到缓冲区结尾的长度
    char* ReadPointeratPosition(qint64 pos, qint64& length);
    //申请空间：从尾开始，返回新空间的指针
    char* Reserve(qint64 bytes);
   //申请空间：从头开始，返回新空间的指针
    char* ReserveFront(qint64 bytes);
  //缩短空间
    void Truncate(qint64 pos) { if (pos < buffersize)chop(buffersize - pos); }
    //从头读取一个字符并转换为int返回
    int getchar() 
    {
        if (isEmpty())
            return -1;
        char c = *readPointer();
        free(1);
        return int(uchar(c));
    }
    //在缓冲区尾部添加字符
    void putChar(char c) { char* ptr = Reserve(1); *ptr = c; }
    //在缓冲区头部添加字符
    void ungetChar(char c) 
    {
        if (begin > 0)
        {
            --begin;
            buffers.first()[begin] = c;
            ++buffersize;
        } 
        else
        {
            char* ptr = ReserveFront(1);
            *ptr = c;
        }
    }
    //读取buffers中的第一个buffer
    QByteArray Read();
    //从指定位置pos拷贝maxlength长度的数据到data中，返回实际截取的长度
    qint64 Peek(char* data, qint64 maxlength, qint64 pos = 0);
    //扩展最后一个buffer
    void append(const char* data, qint64 size);
    //在最后添加一个新buffer
    void append(const QByteArray& qba);
    //从头释放length长度的空间，需要配合reserve使用
    qint64 skip(qint64 length)
    {
        qint64 bytesToskip = qMin(length, buffersize);
        free(bytesToskip);
        return bytesToskip;
    }
    //从尾释放length长度空间，需要配合reserve使用
    void chop(qint64 length);
    //读取一行，包括该行的结束标志'\n'
    qint64 Readline(char* data, qint64 maxlength);
    //能否读一行
    bool canReadline() { return Indexof('\n', buffersize) >= 0; }
    void clear();//清空缓冲区
    //void Print(CircularBuffer* cb);
    //int Increase(CircularBuffer* cb, int p) { return (p + 1) & (2 * cb->capacity - 1); }//start and end pointers incrementation is done module 2*size
   // void Wirte(CircularBuffer* cb, const QByteArray bytes);
    //读取maxlength长度数据到data中，若buffer中数据<maxlength就读取全部，返回长度
    qint64 Read(char* data, qint64 maxlength);
private:
    qint64 buffersize;//buffers数据总大小
    int begin, end;//表示第一个buffer起始位置和最后一个buffer的大小
    int endBuffer;//大小为buffers.size()-1,若为0即只剩一个
    int basicBlockSize;//初始分配空间大小
    QVector<QByteArray> buffers;
    //获取下一个数据块大小，若只剩一个buffer，返回最后一个所含数据大小，否则第一个
    qint64 nextDataBlockSize() { return (endBuffer == 0 ?end : buffers.first().size()) - begin; }
    //获取缓冲区第一个有效数据的指针
    char* readPointer() { return buffersize == 0 ? Q_NULLPTR : (buffers.first().data() + begin); }
   //搜寻内部内容
    qint64 Indexof(char c, qint64 maxlength, qint64 pos = 0);
    //释放空间
    void free(qint64 bytes);
};
//class QtDhkjpump : public QSerialPort //:public QtWidgetsApplication1
//{
//public:
//    QtDhkjpump();
//    void SetData();
//    void Setcurrent(double value);
//    void SetcavTemp(double value);
//    void ReInfoData(const QByteArray data);
//    double Pumpcurrent() { return repumpcurrent; }
//    double Pumppower() { return repumppower; }
//    double Pumptemp() { return repumptemp; }
//    double Tempcurrent() { return retempcur; }
//    double Cavtemp() { return recavtemp; }
//    Status& Ampstatus() { return ampstatus; }
//    QByteArray Datasend() { return datasend; }
//private:
//    Status ampstatus = off;
//    QByteArray datasend;
//    QByteArray datareceive;
//    QByteArray setzero;
//    int setcurvalue;
//    int resetcurrent;
//    double repumpcurrent;
//    double repumppower;
//    double repumptemp;
//    double retempcur;
//    double recavtemp;
//    int u3;
//    int r;
//    double R7;
//    double lnR7;
////data received
//    QByteArray reseed[8];
//
//};

//class QtLambdapump :public QSerialPort//:public QtWidgetsApplication1
//{
//public:
//    QtLambdapump();
//    void SetData();
//    void Setcurrent(const int value);//要不基类写个虚函数要子类去重构？
//    void SetMMcurrent(const double value);
//    void Set9Wcurrent(const double value);
//    void StatusQuery();
//    void ReInfoData(const QByteArray data);
//    void OnstatusQuery();
//    void MMOnstatusQuery();
//    void MMstatusQuery();
//    Status& Ampstatus() { return ampstatus; }
//    int Pumpcurrent() { return repumpcurrent; }
//    double Pumptemp() { return repumptemp; }
//    int Pumppower() { return repumppower; }
//    int PD1power() { return pd1power; }
//    int PD2power() { return pd2power; }
//    int PD3power() { return pd3power; }
//    int PD4power() { return pd4power; }
//    double Modeltemp() { return backtemp; }
//    double LD27wtemp() { return ld27wtemp; }
//    double LD27wcur() { return ld27wcur; }
//    double LD9wcur() { return ld9wcur; }
//    QByteArray& Datasend() { return datasend; }
//    //~QtLambdapump();//
//private:
//    Status ampstatus = off;
//    QByteArray datasend;//发送的数据
//    QByteArray datareceive;//收到的数据
//    QByteArray setzero;//直接电流设置为0
//    int setcurvalue;//设置的电流
//    int resetcurrent;//返回指令中设置的电流
//    int resettectemp;//返回指令中设置的温度
//    int repumpcurrent;//返回指令中ld的电流
//    int repumppower;//返回指令中ld功率
//    int reteccurrent;//返回指令中tec电流
//    double remodeltemp;//返回指令中背板温度
//    double repumptemp;//返回指令中多模LD（27W）温度
//    int pd1power;//返回指令中pd1功率
//    double backtemp;//背板温度
//    double tectemp;//tec温度
//    double cavtemp;//腔温度
//    int pd2power;//pd2功率
//    int pd3power;//pd3功率
//    int pd4power;//pd4功率
//    double ld27wtemp;//多模ld（27w）温度
//    double ld27wcur;//多模ld（27w）电流
//    double ld9wcur;//多模ld（9w）电流
//    double ld27wsetcur;//多模ld（27w）设置电流
//    double ld9wsetcur;//多模ld（9w）设置电流
//
//};


//class QtGolightpump :public QSerialPort//:public QtWidgetsApplication1
//{
//public:
//    QtGolightpump();
//    void SetData();
//    void Setcurrent(const int value);
//    void CurrentQuery();
//    void TempQuery();
//    void BacklightQuery();
//    void ReInfoData(const QByteArray data);
//    void Onofflaser(const QByteArray data);
//    Status& Ampstatus() { return ampstatus; }
//    int Ampcurrent() { return repumpcurrent; }
//    int Ampsetcur() { return resetcurrent; }
//    int Amppower() { return repumppower; }
//    double Amptemp() { return repumptemp; }
//    QByteArray& Datasend() { return datasend; }
//    int setcurvalue;
//private:
//    Status ampstatus = off;
//    QByteArray datasend;
//    QByteArray datareceive;
//    QByteArray setzero;
//    int resetcurrent;
//    int repumpcurrent;
//    int repumppower;
//    double repumptemp;
//};

//class QtCotek :public QSerialPort  //:public QtWidgetsApplication1
//{
//public:
//    QtCotek();
//    void Setcurrent(QByteArray value);
//    void OutputCurrentQuery();//试试这样有问题吗,其实可以列为常量的
//    void OutputVoltageQuery();
//    void TempQuery();
//    void ReInfoData(const QByteArray data);
//    void OnOffstatusQuery();
//    Status DeviceStatusQuery();
//    Status OnOffDevice();
//    Status& Ampstatus() { return ampstatus; }
//   double ampcurrent() { return repumpcurrent; }
//    int ampsetcur() { return resetcurrent; }
//    int amptemp() { return retemp; }
//
//    QByteArray Datasend() { return datasend; }
//private:
//    Status ampstatus = off;
//    QByteArray datasend;
//    QByteArray datareceive;
//    QByteArray setzero;
//    int resetcurrent;
//    double repumpcurrent;
//    double repumpvoltage;
//    double retemp;
//};

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

