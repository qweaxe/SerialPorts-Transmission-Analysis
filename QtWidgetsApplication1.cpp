#include "QtWidgetsApplication1.h"
#include <qvalidator.h>
#include<QtWidgets/QMessageBox>
//QApplication::addLibraryPath("");

//lambda版本（测试分支）
 
/**
  * @Function Name  : QtWidgetsApplication1
  * @
  * @brief 主函数
  * @param parent  父指针
  * @retval None
  */
QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //分配线程
   // Mainwork->moveToThread(t1);
    Processwork->moveToThread(t2);
    
    //理论上这些应该全放到构造函数里去
    QStringList comlist;
    QIntValidator* SMvalidator = new QIntValidator(curmin, seedcurmax, this);
    QDoubleValidator* MMvalidator = new QDoubleValidator(amp4curmin, amp4curmax, 1, this);
    QDoubleValidator* Mainvalidator = new QDoubleValidator(mainmin, mainmax, 1, this);
    redatatxt.setFileName(txtfilename);
    reerrortxt.setFileName(errortxtfilename);

    ui.COM0PowerEdit->setValidator(SMvalidator);
    ui.COM1PowerEdit->setValidator(SMvalidator);
    ui.COM2PowerEdit->setValidator(MMvalidator);
    ui.COM3PowerEdit->setValidator(MMvalidator);
    comlist.clear();

    //这句语法不太熟悉
    //foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    //{
    //    QSerialPort serial;
    //    serial.setPort(info);
    //    if (serial.open(QIODevice::ReadWrite))
    //    {
    //        ui.ComComboBox->addItem(serial.portName());
    //        serial.close();
    //    }
    //}
    //SetData();//换个位置？使能和控制电流的命令长度不一样

    //给子线程建立连接
    //connect(this, &QtWidgetsApplication1::starting1, Mainwork, &MainThread::begin);
    connect(this, &QtWidgetsApplication1::starting2, Processwork, &ProcessThread::begin);
    

    //接收子线程发送的数据，并更新进度条
    //connect(Mainwork, &MainThread::working, this, [=]int l);
    //connect(Processwork, &ProcessThread::working, this, [=]int l);
    //connect(Processwork, &ProcessThread::processed, this, &QtWidgetsApplication1::COM0Changed);

    t2->start();
    emit QtWidgetsApplication1::starting2();
    connect(Processwork, &ProcessThread::SerialPortChanged, this, &QtWidgetsApplication1::SerialPortChanged);
    connect(this, &QtWidgetsApplication1::GetSerialPort, Processwork, &ProcessThread::SetSerialPortList);
    //connect设置串口的信息函数
    connect(this, &QtWidgetsApplication1::SetSerialPort, Processwork, &ProcessThread::SetSerialPort);
    
}
     




//QGroupBox* createFirstExclusiveGroup()
//{
//    //QGroupBox* groupBox = new QGroupBox(tr("Exclusive ZZ Buttons"));
//
//   /* QGroupBox* */
//}

/**
  * @Function Name  : on_pushButton_toggled
  * @
  * @brief 开启通信/关闭通信
  * @param checked  按钮状态
  * @retval void
  */
void QtWidgetsApplication1::on_pushButton_toggled(bool checked)
{
    if (checked)
    {

        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui.ComComboBox->currentText());
        //打开串口，读写F
        serial->open(QIODevice::ReadWrite);
        //设置波特率，后面改成可变的
        serial->setBaudRate(QSerialPort::Baud115200);
        //设置数据位数，后面改成可变的
        serial->setDataBits(QSerialPort::Data8);
        //设置校验位，后面改成可变的
        serial->setParity(QSerialPort::NoParity);
        //设置停止位，后面改成可变的
        serial->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //设置读取数据的缓存大小
        serial->setReadBufferSize(1024);
        //关闭设置菜单使能（好像不要这个功能）
        //连接信号槽，点开始通信后就开始，下位机一有数据发送过来就会相应此槽函数：connect（ob1，sig1，ob2，slot1）
        QObject::connect(serial, &QSerialPort::readyRead, this, &QtWidgetsApplication1::ReadData);
        //多线程版本，2022年10月8日，需要修改
        //QObject::connect(serial, &QSerialPort::readyRead, Processwork, &ProcessThread::Read);
        Comstatus = on;
        //图标提示
        ui.statuslabel->setPixmap(QPixmap(":/images/on.png"));
        ui.pushButton->setText(tr("关闭串口"));
        //connect，循环访问信息
        QObject::connect(timer, &QTimer::timeout, this, &QtWidgetsApplication1::LaserStatusQuery);

        //QByteArray Test = Seed.Datasend();
    }
    else
    {
        timer->stop();
        serial->clear();
        serial->close();
        serial->deleteLater();
        Comstatus = off;
        //图标提示
        ui.statuslabel->setPixmap(QPixmap(":/images/off.png"));
        ui.pushButton->setText(tr("开启通讯"));
        ui.GetLDStatusBtn->setChecked(0);//用信号机制去修改ui？
        ui.EnlaserButton->setChecked(0);
        ui.GetLDStatusBtn->setText(tr("开始获取"));
        ui.EnlaserButton->setText(tr("使能激光器"));
    }
}

//void QtWidgetsApplication1::sigLabelChange()
//{
//
//}

void QtWidgetsApplication1::on_ComComboBox_currentIndexChanged(int index)
{

}


/**
  * @Function Name  : on_GetcomButton_clicked
  * @
  * @brief 获取可用的通信串口
  * @param None  
  * @retval void
  */
void QtWidgetsApplication1::on_GetcomButton_clicked()
{

    emit GetSerialPort();
    ui.ComComboBox->clear();
    //只能通过按钮这种蠢方法去刷新com口了
    //ui.ComComboBox->clear();
    //foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    //{
    //    QSerialPort serial;
    //    serial.setPort(info);
    //    if (serial.open(QIODevice::ReadWrite))
    //    {
    //        ui.ComComboBox->addItem(serial.portName());
    //        serial.close();
    //    }
    //}
    //ui.GetcomButton->setText(tr("发送数据"));
    //serial->write("00ff ffff");

    //ui.ComComboBox->update();//update和repaint都没有用，不能刷新内容
    //ui.ComComboBox->currentIndexChanged();
}




/**
  * @Function Name  : on_SetCOM0Button_clicked
  * @
  * @brief 发送com0的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM0Button_clicked()
{
    //串口是否开启
    if (Comstatus == off)
        QMessageBox::warning(this, tr("警告⚠"), tr("串口是关闭状态，通讯失败!"));

    else
    {
        //serial->write(SetSeedData);
        timer->stop();
        uint value = ui.COM0PowerEdit->text().toUInt();
        if (value > seedcurmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            //value = value * 109.2267;
            Seed.Setcurrent(value);
            SendDatabyte(0, Seed.Datasend());
        }
        if (LaserQuery == on)
            timer->start();
    }
}
/**
  * @Function Name  : on_SetCOM1Button_clicked
  * @
  * @brief 发送com1的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM1Button_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, tr("警告⚠"), tr("串口是关闭状态，通讯失败!"));
    else
    {
        timer->stop();
        uint value = ui.COM1PowerEdit->text().toUInt();

        if (value > amp1curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {

            Amp1.Setcurrent(value);
            SendDatabyte(1, Amp1.Datasend());
        }
        if (LaserQuery == on)
            timer->start();
    }
}
/**
  * @Function Name  : on_SetCOM2Button_clicked
  * @
  * @brief 发送com2的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM2Button_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, tr("警告⚠"), tr("串口是关闭状态，通讯失败!"));
    else
    {
        timer->stop();
        double value = ui.COM2PowerEdit->text().toDouble();
        if (value > amp2curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            //SetAmp2Data.resize(8);
            
            Amp2.Set9Wcurrent(value);
            SendDatabyte(2, Amp2.Datasend());
        }
        if (LaserQuery == on)
            timer->start();
    }
}
/**
  * @Function Name  : on_SetCOM3Button_clicked
  * @
  * @brief 发送com3的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM3Button_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, tr("警告⚠"), tr("串口是关闭状态，通讯失败!"));
    else if (Amp3.LD9wsetcur()<0.9)
    {
        QMessageBox::warning(this, tr("警告"), tr("前一级尚未正确开启！"));
    }
    else
    {
        timer->stop();
        double value = ui.COM3PowerEdit->text().toDouble();
        if (value > amp3curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
        
            Amp3.SetMMcurrent(value);
            SendDatabyte(3, Amp3.Datasend());
        }
        if (LaserQuery == on)
            timer->start();
    }
}
/**
  * @Function Name  : on_SetCOM39WButton_clicked
  * @
  * @brief 发送com3，9w泵的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_setCOM39WBtn_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, tr("警告⚠"), tr("串口是关闭状态，通讯失败!"));
    else if (Amp2.LD9wsetcur() < 0.9)
    {
        QMessageBox::warning(this, tr("警告"), tr("前一级尚未正确开启！"));
    }
    else
    {
        timer->stop();
        double value = ui.COM39WEdit->text().toDouble();
        if (value > amp4curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {       
            Amp3.Set9Wcurrent(value);
            SendDatabyte(3, Amp3.Datasend());
        }
        if (LaserQuery == on)
            timer->start();
    }
}


/**
  * @Function Name  : Readdata
  * @
  * @brief 读取串口返回的数据，好像可以在这处理数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::ReadData()
{
    //读取
    QByteArray buf = serial->readAll();//2022年4月15日：测试版本先用readline()；大电流驱动板对应不了！还是用readAll()+判断
    QByteArray enter("\n");
    //enter.resize(1);
    //enter[0] = '\n';
   
    redatatxt.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    reerrortxt.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    if (!buf.isEmpty())
    {
        //circularBuffer.append(buf);
        //emit starting2(buf);
        redatatxt.write(buf.toHex(' ').toUpper());
        redatatxt.write(enter);
        //emit starting2(buf);
        AnalysisData(buf);
        DisplayData(buf);
    }
    redatatxt.close();
    reerrortxt.close();
    buf.clear();
    //buffer.close();
}


/**
  * @Function Name  : on_SeedPowerEdit_returnPressed
  * @
  * @brief 回车也能发出信号
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SeedPowerEdit_returnPressed()
{
    if (Comstatus == off)
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        //int value = ui.SeedPowerEdit->text().toInt();
       // SetSeedData[3] = value / 125;
       // SetSeedData[4] = value % 125;
        Senddata += SetSeedData;
        serial->write(Senddata);
        ui.SendtextEdit->clear();
        ui.SendtextEdit->setText(Senddata.toHex().toUpper());
    }

}


/**
  * @Function Name  : DisplayData
  * @
  * @brief 显示收到的字符串
  * @param buf  返回的数据
  * @retval void
  */
void QtWidgetsApplication1::DisplayData(const QByteArray buf)
{
    QByteArray buf1 = buf;
    QString str = AddBlank(buf1);
    ui.ReceivetextEdit->clear();
    ui.ReceivetextEdit->append(str);
    
}

//处理收到的数据
//格式： AA + 55 + com（1）+ length（1） + info（length） + checksum（1）+ CR + LF e.g.：AA 55 01 05 EF 01 00 64 EF 48 0D 0A
/**
  * @Function Name  : AnalysisData
  * @
  * @brief 分析返回的数据
  * @param buf  返回的数据
  * @retval void
  */
void QtWidgetsApplication1::AnalysisData(QByteArray buf)//const
{
    //转换
    uint length = buf.size();
    double curvalue = 0;
    double tempvalue = 0;
    int powervalue = 0;
    int backcurvalue = 0;//应该要改成double类型才对
    int size = 0;
    QByteArray enter("\n");
    //如果要写中文应该用QString
    QByteArray byteframe("error type：frame  Data：");
    QByteArray bytelength("error type: length  Data：");
    QByteArray bytechecksum("error type: checksum  Data：");
    
    //帧头验证
    if (buf[0] != 0xAA && buf[1] != 0x55)
    {
        //QMessageBox::warning(this, tr("警告⚠"), tr("舍弃"));
        reerrortxt.write(byteframe);
        reerrortxt.write(enter);
        reerrortxt.write(buf.toHex(' ').toUpper());
        reerrortxt.write(enter);
        reerrortxt.close();

    }
      
    //长度验证
    //else
    //{
    //    size = buf[3] + 5;
    //    buf.resize(size);
    //}
    else if ((unsigned char)buf[3] != length - 7)
    {
        //QMessageBox::warning(this, "错误❌", "收到数据长度不对");
        reerrortxt.write(bytelength);
        reerrortxt.write(enter);
        reerrortxt.write(buf.toHex(' ').toUpper());
        reerrortxt.write(enter);
        reerrortxt.close();
    }
    else
    {
        //(选)获取命令字、数据长度
        buf.resize(length - 2);//把空格删掉了
        temp.resize(length - 2);
        //检验校验和
        temp = buf;
        Checksum(temp);
        if (checksum[0] != buf[length - 3])
        {
            //QMessageBox::warning(this, tr("警告⚠"), tr("校验和不正确"));
            reerrortxt.write(bytechecksum);
            reerrortxt.write(enter);
            reerrortxt.write(buf.toHex(' ').toUpper());
            reerrortxt.write(enter);
            reerrortxt.close();
        }
            //目前这样，感觉不太对
        else
        {
            switch (buf[2])
            {

            case 0x00://case[seed]
                //QMessageBox::information(this, "收到消息", "这个是Seed端返回的信号！");

                Seed.ReInfoData(buf);
              /*  if (Seed.Ampstatus() == on)
                    ui.COM0Status->setPixmap(QPixmap(":/images/online.png"));*/
                switch (Seed.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM0Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                 case off:
                     ui.COM0Status->setPixmap(QPixmap(":/images/offline.png"));
                     break;
                default:
                    break;
                }
                ui.COM0TempLcd->display(Seed.Pumptemp());
                //ui.COM0CoolcurLcd->display(Seed.Tempcurrent());
                ui.COM0PowerLcd->display(Seed.Pumppower());
                ui.COM0CurLcd->display(Seed.Pumpcurrent());
                //ui.COM0CavtempLcd->display(Seed.Cavtemp());
                break;

            case 0x01: //case[amp1]
                //QMessageBox::information(this, "收到消息", "这个是Amp1端返回的信号！");
                Amp1.ReInfoData(buf);
                switch (Amp1.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM1Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                case off:
                    ui.COM1Status->setPixmap(QPixmap(":/images/offline.png"));
                    break;
                default:
                    break;
                }
                ui.COM1TempLcd->display(Amp1.Pumptemp());
                ui.COM1PowerLcd->display(Amp1.Pumppower());
                ui.COM1CurLcd->display(Amp1.Pumpcurrent());
                break;

            case 0x02: //case[amp2]
                //QMessageBox::information(this, "收到消息", "这个是Amp2端返回的信号！");
                Amp2.ReInfoData(buf);
                switch (Amp2.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM2Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                case off:
                    ui.COM2Status->setPixmap(QPixmap(":/images/offline.png"));
                    break;
                default:
                    break;
                }
                //com2仅连了一个9W泵
                ui.COM2TempLcd->display(Amp2.Modeltemp());
                ui.COM227WLDTempLcd->display(Amp2.LD27wtemp());
                ui.COM29WCurLcd->display(Amp2.LD9wcur());
                break;

            case 0x03://case[amp3]
                //QMessageBox::information(this, "收到消息", "这个是Amp3端返回的信号！");
                Amp3.ReInfoData(buf);
                switch (Amp3.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM3Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                case off:
                    ui.COM3Status->setPixmap(QPixmap(":/images/offline.png"));
                    break;
                default:
                    break;
                }
                ui.COM3TempLcd->display(Amp3.Modeltemp());
                ui.COM327WTempLcd->display(Amp3.LD27wtemp());
                ui.COM327WCurLcd->display(Amp3.LD27wcur());
                ui.COM39WCurLcd->display(Amp3.LD9wcur());
                break;
          
            default:
                break;
            }
        }

    }
    //检验数据
    //读取数据，分发
}
/**
  * @Function Name  : on_EnlaserButton_toggled
  * @
  * @brief 使能激光器按键
  * @param checked  按钮状态
  * @retval void
  */
void QtWidgetsApplication1::on_EnlaserButton_toggled(bool checked)
{
    if (Comstatus == off)
    {
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
        ui.EnlaserButton->setChecked(0);
    }

    else
    {
        if (checked)//开启
        {

            ui.EnlaserButton->setText(tr("关闭激光器"));
            ////只能用这种蠢方法一个个赋值吗？测试数据
            //QByteArray data;
            //data.resize(6);
            //data[0] = 0xAA;
            //data[1] = 0x55;
            //data[2] = 0x01;
            //data[3] = 0xD3;
            //data[4] = 0x00;
            //data[5] = 0x00;
            //serial->write(data);//直接写输过去的内容？
            //ui.SendtextEdit->setText(data.toHex());

            Laserstatus = Enlaser();
        }
        else//关闭，seed发送停止获取数据，1、2、4发送关闭命令，3设置电流为0？
        {
            //QByteArray data;
            //data.resize(6);
            //data[0] = 0xAA;
            //data[1] = 0x55;
            //data[2] = 0x02;
            //data[3] = 0xD3;
            //serial->write(data);
            //ui.SendtextEdit->setText(data.toHex());
            Laserstatus = Dislaser();
            ui.EnlaserButton->setText(tr("使能激光器"));
            ui.GetLDStatusBtn->setChecked(0);
            ui.GetLDStatusBtn->setText(tr("开始获取"));

        }
    }
}
/**
  * @Function Name  : SetData
  * @
  * @brief 初始化各级的初始数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::SetData()//
{
    //这些应当放到主类的构造函数中去
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

    //QtDhkjpump seed;
    SetSeedData.resize(5);
    SetSeedData[0] = 0x55;
    SetSeedData[1] = 0xAA;
    SetSeedData[2] = 0x02;
    SetSeedData[3] = 0x00;
    SetSeedData[4] = 0x00;
    Seed.Setcurrent(0);
    //EF EF 05 FF 00 VAL(Hi) VAL(Low) Checksum

    //QtGolightpump amp1;

    SetAmp1Data.resize(8);
    SetAmp1Data[0] = 0xEF;
    SetAmp1Data[1] = 0xEF;
    SetAmp1Data[2] = 0x05;
    SetAmp1Data[3] = 0xFF;
    SetAmp1Data[4] = 0x00;
    SetAmp1Data[7] = 0x00;
    Amp1.Setcurrent(0);
    //QtGolightpump amp2;
    SetAmp2Data.resize(8);
    SetAmp2Data[0] = 0xEF;
    SetAmp2Data[1] = 0xEF;
    SetAmp2Data[2] = 0x05;
    SetAmp2Data[3] = 0xFF;
    SetAmp2Data[4] = 0x00;
    SetAmp2Data[7] = 0x00;
    Amp2.Setcurrent(0);
    //QtGolightpump amp3;
    SetAmp3Data.resize(6);
    SetAmp3Data[0] = 0x01;
    SetAmp3Data[1] = 0x06;
    SetAmp3Data[2] = 0x55;
    SetAmp3Data[3] = 0x7E;
    SetAmp3Data[4] = 0x00;//high
    SetAmp3Data[5] = 0x00;//low
    Amp3.Setcurrent(0);
    //SetAmp3Data[6] = 0x00;//crc16low
    //SetAmp3Data[7] = 0x00;//crc16hi
    
     
    //紧急时刻直接设置的值
    SetSeedzero.resize(5);
    SetSeedzero[0] = 0x55;
    SetSeedzero[1] = 0xAA;
    SetSeedzero[2] = 0x02;
    SetSeedzero[3] = 0x00;
    SetSeedzero[4] = 0x00;

    SetAmp1zero.resize(8);
    SetAmp1zero[0] = 0xEF;
    SetAmp1zero[1] = 0xEF;
    SetAmp1zero[2] = 0x05;
    SetAmp1zero[3] = 0xFF;
    SetAmp1zero[4] = 0x00;
    SetAmp1zero[5] = 0x00;
    SetAmp1zero[6] = 0x00;
    SetAmp1zero[7] = 0xE2;

    SetAmp3zero.resize(8);
    SetAmp3zero[0] = 0x01;
    SetAmp3zero[1] = 0x06;
    SetAmp3zero[2] = 0x55;
    SetAmp3zero[3] = 0x7E;
    SetAmp3zero[4] = 0x00;
    SetAmp3zero[5] = 0x00;
    SetAmp3zero[6] = 0xF8;
    SetAmp3zero[7] = 0x1E;

}
/**
  * @Function Name  : QStringtoHex
  * @
  * @brief QString转换为hex，备用
  * @param str  字符串
  * @param senddata 转换后的结果
  * @retval void
  */
void QtWidgetsApplication1::QStringtoHex(QString str, QByteArray& senddata)//备用
{
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len / 2);
    char lstr, hstr;
    for (int i = 0; i < len; )
    {
        hstr = str[i].toLatin1();
        if (hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if (i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if ((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata * 16 + lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}
/**
  * @Function Name  : ConvertHexChar
  * @
  * @brief hex转char显示，备用
  * @param ch  要转的char
  * @retval char 转换的结果
  */
char QtWidgetsApplication1::ConvertHexChar(char ch)//备用
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - 0x30;
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch - 'A' + 10;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    else return ch - ch;//不在此范围内，这样是否合理？
}
///**
//  * @Function Name  : ConverttoHex
//  * @
//  * @brief 转换为16进制，备用
//  * @param value  要转的10进制数
//  * @retval char 转换的字符串
//  */
//char* QtWidgetsApplication1::ConverttoHex(int value)//备用
//{
//    char* text;
//    int hi = value / 256;
//    int low = value % 256;
//    /*buffer[0] = hi;
//    buffer[1] = low;*/
//    *text = hi + low;
//    return text;
//}

/**
  * @Function Name  : AddBlank
  * @
  * @brief 给数据添加空格，转换大写和hex显示
  * @param arr  待转换的字符阵列
  * @retval QString 转换完的字符串
  */
QString QtWidgetsApplication1::AddBlank(QByteArray& arr)
{
    QString str = arr.toHex().toUpper();
    QString disdata;
    for (int i = 0; i < str.length() / 2; i++)
    {
        disdata += str.mid((i * 2), 2) + " ";//不是取中间，是第一个参数pos指定截取字符串起始位置，第二个参数n指定截取字符串长度
    }
    return disdata;
}

/**
  * @Function Name  : Checksum
  * @
  * @brief 求累加校验和
  * @param data 待求校验和的数据
  * @retval void
  */
void QtWidgetsApplication1::Checksum(QByteArray& data)//校验和部分
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

/**
  * @Function Name  : CRC16Checksum
  * @
  * @brief 求CRC16（modbus）累加校验和
  * @param data 待求校验和的数据
  * @retval void
  */
void QtWidgetsApplication1::CRC16Checksum(QByteArray& data)
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

/**
  * @Function Name  : Enlaser
  * @
  * @brief 使能激光器
  * @param None
  * @retval Status 开启状态
  */
Status QtWidgetsApplication1::Enlaser()
{
    bool status = 0;
    //timer->stop();
    //初始化数据
    SetSeedData[2] = 0x03;//开始获取数据
    SetSeedData[3] = 0x73;
    SetSeedData[4] = 0x73;
    
    //开启命令，看返回值是否正常来判断通信
   
   // 设置Golight开启关闭状态的控制位;
    QByteArray temp;
    temp.resize(1);
    temp[0] = 0x04;


    //逐级发送数据
    //Seed
    Seed.OnstatusQuery();
    SendDatabyte(0, Seed.Datasend());

    //Amp1，这种就可以放到类里作为一个方法
    Amp1.OnstatusQuery();
    SendDatabyte(1, Amp1.Datasend());

   //Amp2
    Amp2.MMOnstatusQuery();
    SendDatabyte(2, Amp2.Datasend());

    //Amp3，读模块号
    Amp3.MMOnstatusQuery();
    SendDatabyte(3, Amp3.Datasend());

    ////Amp4
    //Amp4.Onofflaser(temp);
    //SendDatabyte(4, Amp4.Datasend());

    ////Amp5
    //Amp5.Onofflaser(temp);
    //SendDatabyte(5, Amp5.Datasend());
   
    ////Amp6
    //Amp6.Onofflaser(temp);
    //SendDatabyte(6, Amp6.Datasend());
    //
    ////Amp7
    //Amp7.Onofflaser(temp);
    //SendDatabyte(7, Amp7.Datasend());

    ////Main
    //Mainamp.OnOffstatusQuery();
    //Comandlen[0] = 0x05;
    //Comandlen[1] = Mainamp.Datasend().size();
    //Senddata[0] = 0x00;
    //Senddata = Header + Comandlen + Mainamp.Datasend() + Senddata;
    //Checksum(Senddata);
    //Senddata = Senddata + Ender;
    //serial->write(Senddata);
    //serial->waitForBytesWritten(10);
    //Sleep(10);
    //ui.SendtextEdit->append(AddBlank(Senddata));
    //Senddata.clear();
    //Senddata.resize(1);
    ////再开启，只有main需要
    //Mainamp.OnOffDevice();
    //Comandlen[0] = 0x05;
    //Comandlen[1] = Mainamp.Datasend().size();
    //Senddata[0] = 0x00;
    //Senddata = Header + Comandlen + Mainamp.Datasend() + Senddata;
    //Checksum(Senddata);
    //Senddata = Senddata + Ender;
    //serial->write(Senddata);
    //serial->waitForBytesWritten(10);
    //Sleep(10);
    //ui.SendtextEdit->append(AddBlank(Senddata));
    //Senddata.clear();
    //Senddata.resize(1);
    ////timer->start();
    //Sleep(10);//不知道有没有用
    return on;
}

/**
  * @Function Name  : Dislaser
  * @
  * @brief 关闭激光器
  * @param None
  * @retval Status 开启状态
  */
Status QtWidgetsApplication1::Dislaser()
{
    timer->stop();
    Senddata.resize(1);
    
    //SetSeedData.resize(5);//停止获取数据
    ////SetSeedData.fromHex("0x55,0x0A,0x03,0x70,0x70");
    //SetSeedData[0] = 0x55;
    //SetSeedData[1] = 0xAA;
    //SetSeedData[2] = 0x03;
    //SetSeedData[3] = 0x70;
    //SetSeedData[4] = 0x70;

    ////关闭命令，看返回值是否正常
    //QByteArray temp;
    //temp.resize(1);
    //temp[0] = 0x05;
   
    //SetAmp1Data.resize(6);
    //SetAmp1Data[0] = 0xEF;
    //SetAmp1Data[1] = 0xEF;
    //SetAmp1Data[2] = 0x03;
    //SetAmp1Data[3] = 0xFF;
    //SetAmp1Data[4] = 0x05;

    ////关闭命令，看返回值是否正常
    //
    //SetAmp2Data.resize(6);
    //SetAmp2Data[0] = 0xEF;
    //SetAmp2Data[1] = 0xEF;
    //SetAmp2Data[2] = 0x03;
    //SetAmp2Data[3] = 0xFF;
    //SetAmp2Data[4] = 0x05;

    ////设置pump电流为0算了，也不好弄，3月24日
    //SetAmp3Data.resize(8);
    //SetAmp3Data[0] = 0x00;
    //SetAmp3Data[1] = 0x06;
    //SetAmp3Data[2] = 0x55;
    //SetAmp3Data[3] = 0x7E;
    //SetAmp3Data[4] = 0x00;//hi
    //SetAmp3Data[5] = 0x00;//low
    //SetAmp3Data[6] = 0xF8;
    //SetAmp3Data[7] = 0x1E;

    ////关闭命令，看返回值是否正常
    //Amp4.Onofflaser(temp);
    //SetAmp4Data.resize(6);
    //SetAmp4Data[0] = 0xEF;
    //SetAmp4Data[1] = 0xEF;
    //SetAmp4Data[2] = 0x05;
    //SetAmp4Data[3] = 0xFF;
    //SetAmp4Data[4] = 0x00;

    //SetMainData.resize(9);//不知道是power还是global power,先用前者
    //SetMainData[0] = 0x50;//P
    //SetMainData[1] = 0x4F;//O
    //SetMainData[2] = 0x57;//W
    //SetMainData[3] = 0x45;//E
    //SetMainData[4] = 0x52;//R
    //SetMainData[5] = 0x20;//space
    //SetMainData[6] = 0x30;//0
    //SetMainData[7] = 0x0D;//CR
    //SetMainData[8] = 0x0A;//LF

    //逐级发送数据

  //Senddata[0] = 0x00;//Seed
  //Senddata[1] = 0x00;
  //Senddata = SetSeedData + Senddata;
  //Checksum(Senddata);
  //serial->write(Senddata);
  //Senddata.clear();
  //Senddata.resize(2);
    //Comandlen[0] = 0x00;
   
    //Seed
    Seed.Setcurrent(1);
    SendDatabyte(0, Seed.Datasend());

  //Amp1
    Amp1.Setcurrent(1);
    SendDatabyte(1, Amp1.Datasend());

   //Amp2
    Amp2.Set9Wcurrent(0.001);
    SendDatabyte(2, Amp2.Datasend());

    //Amp3，还不知道问啥，直接电流设置为0，9W
    Amp3.Set9Wcurrent(0.001);
    SendDatabyte(3, Amp3.Datasend());

    ////Amp4
    //Comandlen[0] = 0x04;
    //Amp4.Onofflaser(temp);
    //SendDatabyte(4, Amp4.Datasend());

    ////Amp5
    //Comandlen[0] = 0x05;
    //Amp5.Onofflaser(temp);
    //SendDatabyte(5, Amp5.Datasend());

    ////Amp6
    //Comandlen[0] = 0x06;
    //Amp6.Onofflaser(temp);
    //SendDatabyte(6, Amp6.Datasend());
    ////Amp7
    //Comandlen[0] = 0x07;
    //Amp7.Onofflaser(temp);
    //SendDatabyte(7, Amp7.Datasend());
    ////Main
    //Mainamp.OnOffDevice();
    //Comandlen[0] = 0x05;
    //Comandlen[1] = Mainamp.Datasend().size();
    //Senddata[0] = 0x00;
    //Senddata = Header + Comandlen + Mainamp.Datasend() + Senddata;
    //Checksum(Senddata);
    //Senddata = Senddata + Ender;
    //serial->write(Senddata);
    //serial->waitForBytesWritten(10);
    //Sleep(sleeptime);
    //ui.SendtextEdit->append(AddBlank(Senddata));
    //Senddata.clear();
    //Senddata.resize(1);
    ////timer->start();
    //Sleep(sleeptime);
    
    return off;
}

/**
  * @Function Name  : LaserStatusQuery
  * @
  * @brief laser的状态询问，这样要过至少550ms！
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::LaserStatusQuery()
{
    if (Comstatus == off || Laserstatus == off)
    {
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
        timer->stop();
        ui.GetLDStatusBtn->setChecked(0);
        ui.GetLDStatusBtn->setText(tr("开始获取"));
    }
    else
    {
        Seed.StatusQuery();
        SendDatabyte(0, Seed.Datasend());

        Amp1.StatusQuery();
        SendDatabyte(1, Amp1.Datasend());

        Amp2.MMstatusQuery();
        SendDatabyte(2, Amp2.Datasend());
       
        //com3想连大电流驱动，预计要改
        Amp3.MMstatusQuery();
        SendDatabyte(3, Amp3.Datasend());

        ////com4 
        //Amp4.CurrentQuery();
        //SendDatabyte(4, Amp4.Datasend());

        //Amp4.BacklightQuery();
        //SendDatabyte(4, Amp4.Datasend());

        //Amp4.TempQuery();
        //SendDatabyte(4, Amp4.Datasend());

        //// com5
        //Amp5.CurrentQuery();
        //SendDatabyte(5, Amp5.Datasend());

        //Amp5.BacklightQuery();
        //SendDatabyte(5, Amp5.Datasend());

        //Amp5.TempQuery();
        //SendDatabyte(5, Amp5.Datasend());

        //// com6
        //Comandlen[0] = 0x06;
        //Amp6.CurrentQuery();
        //SendDatabyte(6, Amp6.Datasend());

        //Amp6.BacklightQuery();
        //SendDatabyte(6, Amp6.Datasend());

        //Amp6.TempQuery();
        //SendDatabyte(6, Amp6.Datasend());


        //// com7
        //Amp7.CurrentQuery();
        //SendDatabyte(7, Amp7.Datasend());

        //Amp7.BacklightQuery();
        //SendDatabyte(7, Amp7.Datasend());

        //Amp7.TempQuery();
        //SendDatabyte(7, Amp7.Datasend());

        //////功放的
        ////Comandlen[0] = 0x05;
        ////Mainamp.OutputCurrentQuery();
        ////Comandlen[1] = Mainamp.Datasend().size();
        ////Senddata[0] = 0x00;
        //Senddata = Header + Comandlen + Mainamp.Datasend() + Senddata;
        //Checksum(Senddata);
        //Senddata = Senddata + Ender;
        //serial->write(Senddata);
        //serial->waitForBytesWritten(10);
        //Sleep(sleeptime);

        //ui.SendtextEdit->append(AddBlank(Senddata));

        //Senddata.clear();
        //Senddata.resize(1);
        // ui.SendtextEdit->clear();

    }

}

/**
  * @Function Name  : on_GetLDStatusBtn_toggled
  * @
  * @brief 获取ld状态的按钮
  * @param checked  按钮状态
  * @retval void
  */
void QtWidgetsApplication1::on_GetLDStatusBtn_toggled(bool checked)
{
    if (Comstatus == off)
    {
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
        ui.GetLDStatusBtn->setChecked(0);
    }
    else
    {
        if (checked)
        {
            ui.GetLDStatusBtn->setText(tr("停止获取"));
            LaserQuery = on;
            ui.SendtextEdit->clear();
            timer->setInterval(1000);//先设1s
            sendtime->setInterval(50);
            timer->start();
        }
        else
        {
            timer->stop();
            LaserQuery = off;
            ui.SendtextEdit->clear();
            ui.GetLDStatusBtn->setText(tr("开始获取"));
        }
    }

}

/**
  * @Function Name  : Changeui
  * @
  * @brief 备用
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::Changeui()//备用
{
    /* QPalette lcdpat = ui.SeedLdcurLCD->palette();
     lcdpat.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
     ui.SeedLdcurLCD->setStyleSheet("background-color: black");
     ui.SeedLdcurLCD->setPalette(lcdpat);*/
}

/**
  * @Function Name  : SendDatabyte
  * @
  * @brief 发送数据，待完成
  * @param com 哪个com口
  * @param senddata 待发的数据
  * @retval bool 发送情况
  */
bool QtWidgetsApplication1::SendDatabyte(const int com,  QByteArray senddata)
{
    Senddata.resize(1);
    Senddata[0] = 0x00;
    Comandlen[1] = 0x00;
    switch (com)
    {
    case 0x00:
        Comandlen[0] = 0x00;
        CRC16Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;
    case 0x01:
        Comandlen[0] = 0x01;
        CRC16Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;
    case 0x02:
        Comandlen[0] = 0x02;
        CRC16Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;

    case 0x03:
        Comandlen[0] = 0x03;
        CRC16Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;
    case 0x04:
        Comandlen[0] = 0x04;
        Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;

    case 0x05:
        Comandlen[0] = 0x05;
        Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;

    case 0x06:
        Comandlen[0] = 0x06;
        Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;

    case 0x07:
        Comandlen[0] = 0x07;
        Checksum(senddata);
        Comandlen[1] = senddata.size();
        senddata = Header + Comandlen + senddata + Senddata;
        break;
    default:
        QMessageBox::warning(this, tr("警告⚠"), tr("Error Occured!"));
        break;
    }
    Checksum(senddata);
    senddata = senddata + Ender;
    qint64 size = serial->write(senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->clear();
    ui.SendtextEdit->append(AddBlank(senddata));
    if (size == -1)
    {
        QMessageBox::warning(this, tr("警告⚠"), tr("Error Occured!"));
        isSend = 0;
    }
    else
        isSend = 1;//不一定，可能发过去了那边没收到

    return isSend;
}


/**
  * @Function Name  : change_ampstatus
  * @
  * @brief 备用
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::change_ampstatus()
{
}

void QtWidgetsApplication1::COM0Changed(int n, QtLambdapump* Amp0)
{
    switch (n)
    {
    case 0:
        Seed.SetStatus(Amp0);
        ui.COM0CurLcd->display(Seed.Pumpcurrent());
        ui.COM0PowerLcd->display(Seed.Pumppower());
        ui.COM0TempLcd->display(Seed.Pumptemp());
        break;
    case 1:
        Amp1.SetStatus(Amp0);
        ui.COM0CurLcd->display(Amp1.Pumpcurrent());
        break;
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    case 5:

        break;
    case 6:

        break;
    case 7:

        break;
    default:
        break;
    }

}

void QtWidgetsApplication1::SerialPortChanged(QString info)
{
    //ui.ComComboBox->clear();
    ui.ComComboBox->addItem(info);
}

///**
//  * @Function Name  : on_COMDelayEdit_returnPressed
//  * @
//  * @brief 通信延时设置，回车触发
//  * @param None
//  * @retval void
//  */
//void QtWidgetsApplication1::on_COMDelayEdit_returnPressed()
//{
//    sleeptime = ui.COMDelayEdit->text().toInt();
//}

