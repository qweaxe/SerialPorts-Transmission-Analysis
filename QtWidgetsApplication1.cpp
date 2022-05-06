#include "QtWidgetsApplication1.h"
//QApplication::addLibraryPath("");

//测试版本（之前sb删掉了lambda版本）
 
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
   // Processwork->moveToThread(t2);
    //理论上这些应该全放到构造函数里去
    QStringList comlist;
    QIntValidator* SMvalidator = new QIntValidator(curmin, seedcurmax, this);
    QDoubleValidator* MMvalidator = new QDoubleValidator(amp4curmin, amp4curmax, 1, this);
    QDoubleValidator* Mainvalidator = new QDoubleValidator(mainmin, mainmax, 1, this);
    redatatxt.setFileName(txtfilename);


    ui.COM0PowerEdit->setValidator(SMvalidator);
    ui.COM1PowerEdit->setValidator(SMvalidator);
    ui.COM2PowerEdit->setValidator(SMvalidator);
    ui.COM3PowerEdit->setValidator(MMvalidator);
    ui.COM4PowerEdit->setValidator(MMvalidator);
    ui.COM5PowerEdit->setValidator(MMvalidator);
    ui.COM6PowerEdit->setValidator(MMvalidator);
    ui.COM7PowerEdit->setValidator(MMvalidator);
    ui.MainpowerEdit->setValidator(Mainvalidator);

    comlist.clear();

    //这句语法不太熟悉
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui.ComComboBox->addItem(serial.portName());
            serial.close();
        }
    }
    SetData();//换个位置？使能和控制电流的命令长度不一样

    //给子线程传值
    //connect(this, &QtWidgetsApplication1::starting1, Mainwork, &MainThread::begin);
    //connect(this, &QtWidgetsApplication1::starting2, Processwork, &ProcessThread::begin);

    //接收子线程发送的数据，并更新进度条
    //connect(Mainwork, &MainThread::working, this, [=]int l);
    //connect(Processwork, &ProcessThread::working, this, [=]int l);
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
        Comstatus = on;
        //图标提示
        ui.statuslabel->setPixmap(QPixmap(":/images/on.png"));
        ui.pushButton->setText(tr("关闭串口"));
        //connect，循环访问信息
        QObject::connect(timer, &QTimer::timeout, this, &QtWidgetsApplication1::LaserStatusQuery);

        QByteArray Test = Seed.Datasend();
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
    //只能通过按钮这种蠢方法去刷新com口了
    ui.ComComboBox->clear();
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui.ComComboBox->addItem(serial.portName());
            serial.close();
        }
    }
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
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");

    else
    {
        //serial->write(SetSeedData);
        timer->stop();
        uint value = ui.COM0PowerEdit->text().toUInt();
        if (value > seedcurmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x00;
            Comandlen[1] = 0x00;
            //value = value * 109.2267;
            Seed.Setcurrent(value);
            CRC16Checksum(Seed.Datasend());
            Comandlen[1] = Seed.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Seed.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            SetAmp3Data.resize(6);
            Senddata.resize(1);
            Senddata[0] = 0x00;

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
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        uint value = ui.COM1PowerEdit->text().toUInt();

        if (value > amp1curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x01;
            Comandlen[1] = 0x00;
            Amp1.Setcurrent(value);
            CRC16Checksum(Amp1.Datasend());
            Comandlen[1] = Amp1.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp1.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            SetAmp3Data.resize(6);
            Senddata.resize(1);
            Senddata[0] = 0x00;

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
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        uint value = ui.COM2PowerEdit->text().toUInt();
        if (value > amp2curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            //SetAmp2Data.resize(8);
            
            Comandlen[0] = 0x02;
            Comandlen[1] = 0x00;
            Amp2.Setcurrent(value);
            CRC16Checksum(Amp2.Datasend());
            Comandlen[1] = Amp2.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp2.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            SetAmp3Data.resize(6);
            Senddata.resize(1);
            Senddata[0] = 0x00;
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
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        double value = ui.COM3PowerEdit->text().toDouble();
        if (value > amp4curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x03;
            Comandlen[1] = 0x00;
            Amp3.SetMMcurrent(value);
            //int a=Amp3.th->Pumpcurrent();
            CRC16Checksum(Amp3.Datasend());
            Comandlen[1] = Amp3.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp3.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            SetAmp3Data.resize(6);
            Senddata.resize(1);
            Senddata[0] = 0x00;
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
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        double value = ui.COM39WEdit->text().toDouble();
        if (value > amp4curmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x03;
            Comandlen[1] = 0x00;
            Amp3.Set9Wcurrent(value);
            CRC16Checksum(Amp3.Datasend());
            Comandlen[1] = Amp3.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp3.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            SetAmp3Data.resize(6);
            Senddata.resize(1);
            Senddata[0] = 0x00;
        }
        if (LaserQuery == on)
            timer->start();
    }
}

/**
  * @Function Name  : on_SetCOM4Button_clicked
  * @
  * @brief 发送com4的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM4Button_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        uint value = (ui.COM4PowerEdit->text().toDouble()) * 1000;
        if (value > 10000)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x04;
            Amp4.Setcurrent(value);
            Checksum(Amp4.Datasend());
            Comandlen[1] = Amp4.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp4.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            Senddata.resize(1);
            Senddata[0] = 0x00;
        }
        if (LaserQuery == on)
            timer->start();
    }
}
/**
  * @Function Name  : on_SetCOM5Button_clicked
  * @
  * @brief 发送com5的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM5Button_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        uint value = (ui.COM5PowerEdit->text().toDouble()) * 1000;
        if (value > 10000)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x05;
            Amp5.Setcurrent(value);
            Checksum(Amp5.Datasend());
            Comandlen[1] = Amp5.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp5.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            Senddata.resize(1);
            Senddata[0] = 0x00;
        }
        if (LaserQuery == on)
            timer->start();
    }
}

/**
  * @Function Name  : on_SetCOM6Button_clicked
  * @
  * @brief 发送com6的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM6Button_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        uint value = (ui.COM6PowerEdit->text().toDouble()) * 1000;
        if (value > 10000)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x06;
            Amp6.Setcurrent(value);
            Checksum(Amp6.Datasend());
            Comandlen[1] = Amp6.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp6.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            Senddata.resize(1);
            Senddata[0] = 0x00;
        }
        if (LaserQuery == on)
            timer->start();
    }
}

/**
  * @Function Name  : on_SetCOM7Button_clicked
  * @
  * @brief 发送com7的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetCOM7Button_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        uint value = (ui.COM7PowerEdit->text().toDouble()) * 1000;
        if (value > 10000)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Comandlen[0] = 0x07;
            Amp7.Setcurrent(value);
            Checksum(Amp7.Datasend());
            Comandlen[1] = Amp7.Datasend().size();
            Senddata[0] = 0x00;
            Senddata = Header + Comandlen + Amp7.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            Senddata.resize(1);
            Senddata[0] = 0x00;
        }
        if (LaserQuery == on)
            timer->start();
    }
}

/**
  * @Function Name  : on_SetMainButton_clicked
  * @
  * @brief 发送主放大（com5）的数据
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_SetMainButton_clicked()
{
    if (Comstatus == off)
        QMessageBox::warning(this, "警告⚠", "串口是关闭状态，通讯失败!");
    else
    {
        timer->stop();
        //想想怎么处理数据，发送的其实是HEX代码
        QByteArray value = ui.MainpowerEdit->text().toLatin1();
        double douvalue = ui.MainpowerEdit->text().toDouble();
        if (douvalue > mainmax)
            QMessageBox::warning(this, tr("警告⚠"), tr("超过最大范围"));
        else
        {
            Mainamp.Setcurrent(value);
            SetMainData.resize(9);//试试01.5A那边能否接受
            Comandlen[0] = 0x05;
            Comandlen[1] = Mainamp.Datasend().size();

            //SetMainData[0] = 0x53;//S
            //SetMainData[1] = 0x49;//I
            //SetMainData[2] = 0x20;//space
            //SetMainData[3] = value[0];
            //SetMainData[4] = 0x2E;//0x2E.//中文点怎么办？试试，好像不行
            //SetMainData[5] = value[2];
            //SetMainData[6] = 0x30;//不打0了，自己赋值为0
            //SetMainData[7] = 0x0D;//CR
            //SetMainData[8] = 0x0A;//LF

            Senddata[0] = 0x00;
            //Senddata =Header+Comandlen+ SetMainData + Senddata;
            Senddata = Header + Comandlen + Mainamp.Datasend() + Senddata;
            Checksum(Senddata);
            Senddata = Senddata + Ender;
            serial->write(Senddata);
            serial->waitForBytesWritten(10);
            ui.SendtextEdit->clear();
            ui.SendtextEdit->append(AddBlank(Senddata));
            Senddata.clear();

            Senddata.resize(1);
            Senddata[0] = 0x00;


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
    if (!buf.isEmpty())
    {
        //circularBuffer.append(buf);
        //emit starting2(circularBuffer.Read());
        redatatxt.write(buf.toHex(' ').toUpper());
        redatatxt.write(enter);
        AnalysisData(buf);
        DisplayData(buf);
    }
    redatatxt.close();
    buf.clear();
    //buffer.close();
}


/**
  * @Function Name  : on_GetcomButton_clicked
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
    int length = buf.size();
    double curvalue = 0;
    double tempvalue = 0;
    int powervalue = 0;
    int backcurvalue = 0;//应该要改成double类型才对
    int size = 0;
    //帧头验证
    if (buf[0] != 0xAA && buf[1] != 0x55)
        QMessageBox::warning(this, tr("警告⚠"), tr("舍弃"));
    //长度验证
    //else
    //{
    //    size = buf[3] + 5;
    //    buf.resize(size);
    //}
    else if (buf[3] != length - 7)
        QMessageBox::warning(this, "错误❌", "收到数据长度不对");
    else
    {
        //(选)获取命令字、数据长度
        buf.resize(length - 2);//把空格删掉了
        temp.resize(length - 2);
        //检验校验和
        temp = buf;
        Checksum(temp);
        if (checksum[0] != buf[length - 3])
            QMessageBox::warning(this, tr("警告⚠"), tr("校验和不正确"));//目前这样，感觉不太对
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
                ui.COM2TempLcd->display(Amp2.Pumptemp());
                ui.COM2PowerLcd->display(Amp2.Pumppower());
                ui.COM2CurLcd->display(Amp2.Pumpcurrent());
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
                ui.COM3PowerLcd->display(Amp3.Pumppower());
                ui.COM3CurLcd->display(Amp3.LD27wcur());
               
                break;
                //case[amp4]
            case 0x04:
                //QMessageBox::information(this, "收到消息", "这个是Amp4端返回的信号！");
                Amp4.ReInfoData(buf);
                switch (Amp4.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM4Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                case off:
                    ui.COM4Status->setPixmap(QPixmap(":/images/offline.png"));
                    break;
                default:
                    break;
                }
                ui.COM4PowerLcd->display(Amp4.Amppower());
                ui.COM4TempLcd->display(Amp4.Amptemp());
                ui.COM4CurLcd->display(Amp4.Ampcurrent());
                break;
                //case[main]
                //纯发字符串 
            case 0x05:// AA 55 05 0B 31 2E 32 30 41 0D 0A 3D 3E 0D 0A BA (0D 0A)（这里已经去掉了）：1.20A CR LF => CR LF
                //QMessageBox::information(this, "收到消息", "这个是Main端返回的信号！");
                //length = buf[3] + 4;
                //Mainamp.ReInfoData(buf);
                //switch (buf[length - 4])
                //{
                //case 0x21://!
                //    QMessageBox::warning(this, tr("警告⚠"), tr(" Command correct but execution error (e.g. parameters out of range)"));
                //    break;
                //case 0x3D://=

                //    switch (buf[length - 7])
                //    {
                //    case 0x30://0，电源关闭，远程关闭
                //        ui.MainStatus->setPixmap(QPixmap(":/images/online.png"));
                //        break;
                //    case 0x31://1，电源开启，远程关闭

                //        break;
                //    case 0x32://2，电源关闭，远程开启
                //        ui.MainStatus->setPixmap(QPixmap(":/images/offline.png"));//这种情况应该是关后询问
                //        break;
                //    case 0x33://3，电源开启，远程开启
                //        ui.MainStatus->setPixmap(QPixmap(":/images/online.png"));
                //        break;
                //    case 0x41://A
                //        ui.MainCurLcd->display(Mainamp.ampcurrent());
                //        break;
                //    case 0x56://V

                //        break;
                //    default:
                //        break;
                //    }
                //    break;
                //case 0x3F://?
                //    QMessageBox::warning(this, tr("警告⚠"), tr("Command error, not accepted."));
                //    break;
                //default:
                //    break;
                //}
                Amp5.ReInfoData(buf);
                switch (Amp5.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM5Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                case off:
                    ui.COM5Status->setPixmap(QPixmap(":/images/offline.png"));
                    break;
                default:
                    break;
                }
                ui.COM5PowerLcd->display(Amp5.Amppower());
                ui.COM5TempLcd->display(Amp5.Amptemp());
                ui.COM5CurLcd->display(Amp5.Ampcurrent());
                break;
               
            case 0x06:
                Amp6.ReInfoData(buf);
                switch (Amp6.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM6Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                case off:
                    ui.COM6Status->setPixmap(QPixmap(":/images/offline.png"));
                    break;
                default:
                    break;
                }
                ui.COM6PowerLcd->display(Amp6.Amppower());
                ui.COM6TempLcd->display(Amp6.Amptemp());
                ui.COM6CurLcd->display(Amp6.Ampcurrent());
                break;

            case 0x07:
                Amp7.ReInfoData(buf);
                switch (Amp7.Ampstatus())//感觉这样不太好
                {
                case on:
                    ui.COM7Status->setPixmap(QPixmap(":/images/online.png"));
                    break;

                case off:
                    ui.COM7Status->setPixmap(QPixmap(":/images/offline.png"));
                    break;
                default:
                    break;
                }
                ui.COM7PowerLcd->display(Amp7.Amppower());
                ui.COM7TempLcd->display(Amp7.Amptemp());
                ui.COM7CurLcd->display(Amp7.Ampcurrent());
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
    
    //QtGolightpump amp4;
    SetAmp4Data.resize(8);
    SetAmp4Data[0] = 0xEF;
    SetAmp4Data[1] = 0xEF;
    SetAmp4Data[2] = 0x05;
    SetAmp4Data[3] = 0xFF;
    SetAmp4Data[4] = 0x00;
    SetAmp4Data[7] = 0x00;
    Amp4.Setcurrent(0);

    Amp5.Setcurrent(0);
    Amp6.Setcurrent(0);
    Amp7.Setcurrent(0);
    //功放端不知是否是设置GSI还是先找目标再SI，先按SI设置
    //QtCotek mainamp;
    SetMainData.resize(9);
    SetMainData[0] = 0x53;//S
    SetMainData[1] = 0x49;//I
    SetMainData[2] = 0x20;//space
    SetMainData[4] = 0x2E;//.
    SetMainData[7] = 0x0D;//CR
    SetMainData[8] = 0x0A;//LF
     
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
/**
  * @Function Name  : ConverttoHex
  * @
  * @brief 转换为16进制，备用
  * @param value  要转的10进制数
  * @retval char 转换的字符串
  */
char* QtWidgetsApplication1::ConverttoHex(int value)//备用
{
    char* text;
    int hi = value / 256;
    int low = value % 256;
    /*buffer[0] = hi;
    buffer[1] = low;*/
    return text;
}
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
    Comandlen[0] = 0x00;
    Seed.OnstatusQuery();
    CRC16Checksum(Seed.Datasend());
    Comandlen[1] = Seed.Datasend().size();//长度
    Senddata.resize(1);
    Senddata[0] = 0x00;
    Senddata = Header+Comandlen+Seed.Datasend() + Senddata;
    Checksum(Senddata);
     Senddata=Senddata+Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    ui.SendtextEdit->clear();
    ui.SendtextEdit->append(AddBlank(Senddata));
    Sleep(sleeptime);
    Senddata.clear();
    Senddata.resize(1);


    //Amp1，这种就可以放到类里作为一个方法
    Comandlen[0] = 0x01;
    Amp1.OnstatusQuery();
    CRC16Checksum(Amp1.Datasend());
    Comandlen[1] = Amp1.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp1.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Sleep(sleeptime);
    Senddata.clear();
    Senddata.resize(1);

   //Amp2
    Comandlen[0] = 0x02;
    Amp2.OnstatusQuery();
    CRC16Checksum(Amp2.Datasend());
    Comandlen[1] = Amp2.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp2.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Sleep(sleeptime);
    Senddata.clear();
    Senddata.resize(1);

    //Amp3，读模块号
    Comandlen[0] = 0x03;
    Amp3.MMOnstatusQuery();
    CRC16Checksum(Amp3.Datasend());
    Comandlen[1] = Amp3.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp3.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Sleep(sleeptime);
    Senddata.clear();
    Senddata.resize(1);

    //Amp4
    Comandlen[0] = 0x04;
    Amp4.Onofflaser(temp);
    Checksum(Amp4.Datasend());
    Comandlen[1] = Amp4.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp4.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);

    //Amp5
    Comandlen[0] = 0x05;
    Amp5.Onofflaser(temp);
    Checksum(Amp5.Datasend());
    Comandlen[1] = Amp5.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp5.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);
   
    //Amp6
    Comandlen[0] = 0x06;
    Amp6.Onofflaser(temp);
    Checksum(Amp6.Datasend());
    Comandlen[1] = Amp6.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp6.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);
    
    //Amp7
    Comandlen[0] = 0x07;
    Amp7.Onofflaser(temp);
    Checksum(Amp7.Datasend());
    Comandlen[1] = Amp7.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp7.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);

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
    
    SetSeedData.resize(5);//停止获取数据
    //SetSeedData.fromHex("0x55,0x0A,0x03,0x70,0x70");
    SetSeedData[0] = 0x55;
    SetSeedData[1] = 0xAA;
    SetSeedData[2] = 0x03;
    SetSeedData[3] = 0x70;
    SetSeedData[4] = 0x70;

    //关闭命令，看返回值是否正常
    QByteArray temp;
    temp.resize(1);
    temp[0] = 0x05;
   
    SetAmp1Data.resize(6);
    SetAmp1Data[0] = 0xEF;
    SetAmp1Data[1] = 0xEF;
    SetAmp1Data[2] = 0x03;
    SetAmp1Data[3] = 0xFF;
    SetAmp1Data[4] = 0x05;

    //关闭命令，看返回值是否正常
    
    SetAmp2Data.resize(6);
    SetAmp2Data[0] = 0xEF;
    SetAmp2Data[1] = 0xEF;
    SetAmp2Data[2] = 0x03;
    SetAmp2Data[3] = 0xFF;
    SetAmp2Data[4] = 0x05;

    //设置pump电流为0算了，也不好弄，3月24日
    SetAmp3Data.resize(8);
    SetAmp3Data[0] = 0x00;
    SetAmp3Data[1] = 0x06;
    SetAmp3Data[2] = 0x55;
    SetAmp3Data[3] = 0x7E;
    SetAmp3Data[4] = 0x00;//hi
    SetAmp3Data[5] = 0x00;//low
    SetAmp3Data[6] = 0xF8;
    SetAmp3Data[7] = 0x1E;

    //关闭命令，看返回值是否正常
    Amp4.Onofflaser(temp);
    SetAmp4Data.resize(6);
    SetAmp4Data[0] = 0xEF;
    SetAmp4Data[1] = 0xEF;
    SetAmp4Data[2] = 0x05;
    SetAmp4Data[3] = 0xFF;
    SetAmp4Data[4] = 0x00;

    SetMainData.resize(9);//不知道是power还是global power,先用前者
    SetMainData[0] = 0x50;//P
    SetMainData[1] = 0x4F;//O
    SetMainData[2] = 0x57;//W
    SetMainData[3] = 0x45;//E
    SetMainData[4] = 0x52;//R
    SetMainData[5] = 0x20;//space
    SetMainData[6] = 0x30;//0
    SetMainData[7] = 0x0D;//CR
    SetMainData[8] = 0x0A;//LF

    //逐级发送数据

  //Senddata[0] = 0x00;//Seed
  //Senddata[1] = 0x00;
  //Senddata = SetSeedData + Senddata;
  //Checksum(Senddata);
  //serial->write(Senddata);
  //Senddata.clear();
  //Senddata.resize(2);
    Comandlen[0] = 0x00;
    Seed.Setcurrent(1);
    CRC16Checksum(Seed.Datasend());
    Comandlen[1] = Seed.Datasend().size();//长度
    Senddata.resize(1);
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Seed.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->clear();
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);

  //Amp1
    Comandlen[0] = 0x01;
    Amp1.Setcurrent(1);
    CRC16Checksum(Amp1.Datasend());
    Comandlen[1] = Amp1.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp1.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);

   //Amp2
    Comandlen[0] = 0x02;
    Amp2.Setcurrent(1);
    CRC16Checksum(Amp2.Datasend());
    Comandlen[1] = Amp2.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp2.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);

    //Amp3，还不知道问啥，直接电流设置为0，9W
    Comandlen[0] = 0x03;
    Amp3.Set9Wcurrent(0.001);
    CRC16Checksum(Amp3.Datasend());
    Comandlen[1] = Amp3.Datasend().size();
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp3.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));

    Senddata.clear();
    Senddata.resize(1);

    //Amp4
    Comandlen[0] = 0x04;
    Amp4.Onofflaser(temp);
    Checksum(Amp4.Datasend());
    Comandlen[1] = Amp4.Datasend().size();//长度
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp4.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);

    Comandlen[0] = 0x05;
    Amp5.Onofflaser(temp);
    Checksum(Amp5.Datasend());
    Comandlen[1] = Amp5.Datasend().size();//长度
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp5.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);
    
    Comandlen[0] = 0x06;
    Amp6.Onofflaser(temp);
    Checksum(Amp6.Datasend());
    Comandlen[1] = Amp6.Datasend().size();//长度
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp6.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);

    Comandlen[0] = 0x07;
    Amp7.Onofflaser(temp);
    Checksum(Amp7.Datasend());
    Comandlen[1] = Amp7.Datasend().size();//长度
    Senddata[0] = 0x00;
    Senddata = Header + Comandlen + Amp7.Datasend() + Senddata;
    Checksum(Senddata);
    Senddata = Senddata + Ender;
    serial->write(Senddata);
    serial->waitForBytesWritten(10);
    Sleep(sleeptime);
    ui.SendtextEdit->append(AddBlank(Senddata));
    Senddata.clear();
    Senddata.resize(1);
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
        Senddata.resize(1);
        Comandlen[0] = 0x00;
        Seed.StatusQuery();
        CRC16Checksum(Seed.Datasend());
        Comandlen[1] = Seed.Datasend().size();
        Senddata[0] = 0x00;
        Senddata = Header + Comandlen + Seed.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Comandlen[0] = 0x01;
        Amp1.StatusQuery();
        CRC16Checksum(Amp1.Datasend());
        Comandlen[1] = Amp1.Datasend().size();
        Senddata[0] = 0x00;
        Senddata = Header + Comandlen + Amp1.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Comandlen[0] = 0x02;
        Amp2.StatusQuery();
        CRC16Checksum(Amp2.Datasend());
        Comandlen[1] = Amp2.Datasend().size();
        Senddata[0] = 0x00;
        Senddata = Header + Comandlen + Amp2.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);
       
        //com3想连大电流驱动，预计要改
        Comandlen[0] = 0x03;
        Amp3.MMstatusQuery();
        CRC16Checksum(Amp3.Datasend());
        Comandlen[1] = Amp3.Datasend().size();
        Senddata[0] = 0x00;
        Senddata = Header + Comandlen + Amp3.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        //com4 
        Comandlen[0] = 0x04;
        Amp4.CurrentQuery();
        Checksum(Amp4.Datasend());
        Comandlen[1] = Amp4.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp4.Datasend());
        Senddata = Header + Comandlen + Amp4.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp4.BacklightQuery();
        Checksum(Amp4.Datasend());
        Comandlen[1] = Amp4.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp4.Datasend());
        Senddata = Header + Comandlen + Amp4.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp4.TempQuery();
        Checksum(Amp4.Datasend());
        Comandlen[1] = Amp4.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp4.Datasend());
        Senddata = Header + Comandlen + Amp4.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        // com5
        Comandlen[0] = 0x05;
        Amp5.CurrentQuery();
        Checksum(Amp5.Datasend());
        Comandlen[1] = Amp5.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp5.Datasend());
        Senddata = Header + Comandlen + Amp5.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp5.BacklightQuery();
        Checksum(Amp5.Datasend());
        Comandlen[1] = Amp5.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp5.Datasend());
        Senddata = Header + Comandlen + Amp5.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp5.TempQuery();
        Checksum(Amp5.Datasend());
        Comandlen[1] = Amp5.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp5.Datasend());
        Senddata = Header + Comandlen + Amp5.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        // com6
        Comandlen[0] = 0x06;
        Amp6.CurrentQuery();
        Checksum(Amp6.Datasend());
        Comandlen[1] = Amp6.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp6.Datasend());
        Senddata = Header + Comandlen + Amp6.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp6.BacklightQuery();
        Checksum(Amp6.Datasend());
        Comandlen[1] = Amp6.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp5.Datasend());
        Senddata = Header + Comandlen + Amp6.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp6.TempQuery();
        Checksum(Amp6.Datasend());
        Comandlen[1] = Amp6.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp6.Datasend());
        Senddata = Header + Comandlen + Amp6.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);


        // com7
        Comandlen[0] = 0x07;
        Amp7.CurrentQuery();
        Checksum(Amp7.Datasend());
        Comandlen[1] = Amp7.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp7.Datasend());
        Senddata = Header + Comandlen + Amp7.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp7.BacklightQuery();
        Checksum(Amp7.Datasend());
        Comandlen[1] = Amp7.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp7.Datasend());
        Senddata = Header + Comandlen + Amp7.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

        Amp7.TempQuery();
        Checksum(Amp7.Datasend());
        Comandlen[1] = Amp7.Datasend().size();
        Senddata[0] = 0x00;
        Checksum(Amp7.Datasend());
        Senddata = Header + Comandlen + Amp7.Datasend() + Senddata;
        Checksum(Senddata);
        Senddata = Senddata + Ender;
        serial->write(Senddata);
        serial->waitForBytesWritten(10);
        Sleep(sleeptime);
        ui.SendtextEdit->append(AddBlank(Senddata));
        Senddata.clear();
        Senddata.resize(1);

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
    switch (com)
    {
    case 0x00:
        Comandlen[0] = 0x00;
        break;
    case 0x01:
        Comandlen[0] = 0x01;
        break;
    case 0x02:
        Comandlen[0] = 0x02;
        break;

    case 0x03:
        Comandlen[0] = 0x03;
        break;
    case 0x04:
        Comandlen[0] = 0x04;
        break;

    case 0x05:
        Comandlen[0] = 0x05;
        break;

    case 0x06:
        Comandlen[0] = 0x06;
        break;

    case 0x07:
        Comandlen[0] = 0x07;
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
    if (size == -1)
    {
        QMessageBox::warning(this, tr("警告⚠"), tr("Error Occured!"));
        isSend = 0;
    }
    else
        isSend = 1;//不一定，可能发过去了那边没收到
    return isSend;
}

///**
//  * @Function Name  : QtDhkjpump
//  * @
//  * @brief 东辉科技的构造函数
//  * @param None
//  */
//QtDhkjpump::QtDhkjpump()
//{
//    datasend.resize(5);
//    //datasend.fromHex("0x55,0xAA,0x03,0x73,0x73");
//    datasend[0] = 0x55;
//    datasend[1] = 0xAA;
//    datasend[2] = 0x03;
//    datasend[3] = 0x73;
//    datasend[4] = 0x73;
//    R7 = 0.0;
//    lnR7 = 0.0;
//    recavtemp = 25.0;
//    repumpcurrent = 0.0;
//    repumppower = 0.0;
//    repumptemp = 25.0;
//    resetcurrent = 0;
//    retempcur = 0.0;
//    r = 0;
//    setcurvalue = 0;
//    u3 = 0;
//}
///**
//  * @Function Name  :Setcurrent
//  * @
//  * @brief 设置电流
//  * @param value 设置的值
//  * @retval void
//  */
//void QtDhkjpump::Setcurrent(double value)
//{
//    this->datasend.resize(5);
//    this->datasend[0] = 0x55;
//    this->datasend[1] = 0xAA;
//    this->datasend[2] = 0x02;
//    this->datasend[3] = value / 256;
//    this->datasend[4] = (uint)value % 256;
//}
///**
//  * @Function Name  : ReInfoData
//  * @
//  * @brief 收到的数据处理
//  * @param data 数据
//  * @retval void
//  */
//void QtDhkjpump::ReInfoData(const QByteArray data)
//{
//    this->reseed->resize(4);
//    this->reseed[0] = data.sliced(4, 4);//空
//    this->reseed[1] = data.sliced(8, 4);//空
//    this->reseed[2] = data.sliced(12, 4);//空
//    this->reseed[3] = data.sliced(16, 4);//泵浦激光器温度
//    this->reseed[4] = data.sliced(20, 4);//泵浦制冷电流
//    this->reseed[5] = data.sliced(24, 4);//泵浦激光器功率
//    this->reseed[6] = data.sliced(28, 4);//泵浦激光器驱动电流
//    this->reseed[7] = data.sliced(32, 4);//激光腔温度
//    //temperature
//    this->u3 = ((reseed[3][2] & 0x03) * 256 + reseed[3][3]) * 2;
//    this->r = (10000 * u3) / (2048 - u3);
//    this->repumptemp = (37000 - r) / 600;
//    //temperature current
//    this->retempcur = ((((unsigned char)reseed[4][2] )& 0x03) * 256 + (unsigned char)reseed[4][3]) * 2 * 1.4;
//    //pump power
//    this->repumppower = ((((unsigned char)reseed[5][2]) & 0x03) * 256 + (unsigned char)reseed[5][3]) * 2 * 0.36361;
//    //pump current
//    this->repumpcurrent = ((((unsigned char)reseed[6][2])& 0x03) * 256 + (unsigned char)reseed[6][3]) * 2 * 0.4;
//    //pump temperature
//    this->R7 = (((unsigned char)reseed[7][2]) * 256 +( (unsigned char)reseed[7][3])) * 1.0 * 0.625;
//    this->lnR7 = log(R7);
//    this->recavtemp = pow(((1.141256846 * pow(10, -3)) + 2.325154750 * pow(10, -4) * lnR7 + lnR7 * lnR7 * lnR7 * 0.9126356973 * pow(10, -7)), -1) - 273.15;
//}
///**
//  * @Function Name  : QtLambdapump
//  * @
//  * @brief 朗普达的构造函数
//  * @param None
//  */
//QtLambdapump::QtLambdapump()
//{
//    datasend.resize(8);
//    
//    datasend[0] = 0x01;//开启时命令
//    datasend[1] = 0x03;//决定询问还是设置电流
//    datasend[2] = 0x55;
//    datasend[3] = 0x80;
//    datasend[4] = 0x00;
//    datasend[5] = 0x01;
//    datasend[6] = 0x94;
//    datasend[7] = 0x2E;
//    ampstatus = off;
//    repumpcurrent = 0;
//    repumppower = 0;
//    repumptemp = 25.0;
//    backtemp = 25.0;
//    remodeltemp = 25.0;
//    resettectemp = 0.0;
//    reteccurrent = 0;
//    cavtemp = 0.0;
//    ld27wcur = 0.0;
//    ld27wsetcur = 0.001;
//    ld27wtemp = 25.0;
//    ld9wcur = 0.0;
//    ld9wsetcur = 0.001;
//    pd1power = 0;
//    pd2power = 0;
//    pd3power = 0;
//    pd4power = 0;
//    tectemp = 25.0;
//    resetcurrent = 1;
//    setcurvalue = 0;
//}
///**
//  * @Function Name  : SetData
//  * @
//  * @brief 备用
//  * @param None
//  * @retval void
//  */
//void QtLambdapump::SetData()
//{
//
//}
///**
//  * @Function Name  : Setcurrent
//  * @
//  * @brief 设置电流，小电流驱动板
//  * @param value 电流值，单位mA
//  * @retval void
//  */
//void QtLambdapump::Setcurrent(const int value)
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0x01;
//    this->datasend[1] = 0x06;
//    this->datasend[2] = 0x55;
//    this->datasend[3] = 0x7E;
//    this->datasend[4] = value / 256;
//    this->datasend[5] = value % 256;
//    //QtWidgetsApplication1::CRC16Checksum(datasend);
//
//}
///**
//  * @Function Name  : SetMMcurrent
//  * @
//  * @brief 设置电流，多模ld驱动板
//  * @param value 电流值，单位A
//  * @retval void
//  */
//void QtLambdapump::SetMMcurrent(const double value)
//{
//    uint temp = value * 1000;
//    this->datasend.resize(6);
//    this->datasend[0] = 0x01;
//    this->datasend[1] = 0x06;
//    this->datasend[2] = 0x55;
//    this->datasend[3] = 0x7E;
//    this->datasend[4] = temp / 256;
//    this->datasend[5] = temp % 256;
//}
///**
//  * @Function Name  : Set9Wcurrent
//  * @
//  * @brief 设置电流，多模ld的9W泵
//  * @param value 电流值，单位A
//  * @retval void
//  */
//void QtLambdapump::Set9Wcurrent(const double value)
//{
//    uint temp = value * 1000;
//    this->datasend.resize(6);
//    this->datasend[0] = 0x01;
//    this->datasend[1] = 0x06;
//    this->datasend[2] = 0x55;
//    this->datasend[3] = 0x7F;
//    this->datasend[4] = temp / 256;
//    this->datasend[5] = temp % 256;
//}
///**
//  * @Function Name  : StatusQuery
//  * @
//  * @brief 状态询问
//  * @param None
//  * @retval void
//  */
//void QtLambdapump::StatusQuery()
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0x01;
//    this->datasend[1] = 0x03;
//    this->datasend[2] = 0x55;
//    this->datasend[3] = 0x7E;
//    this->datasend[4] = 0x00;
//    this->datasend[5] = 0x06;
//    //QtWidgetsApplication1::CRC16Checksum(datasend);
//
//}
///**
//  * @Function Name  : ReInfoData
//  * @
//  * @brief 收到的数据处理
//  * @param data 收到的数据
//  * @retval void
//  */
//void QtLambdapump::ReInfoData(const QByteArray data)
//{  
//    
//    if ((unsigned char)data[5] != 0x99)
//        qDebug() << "输出结果是：" << data[5] << " 和 " << 0x99;
//    
//    switch ((unsigned char)data[5])
//    {
//    case 0x03:
//        switch (data[6])
//        {
//        case 0x02:
//            switch (data[8])
//            {
//            case 0x00:
//                this->ampstatus = on;
//                break;
//            case 0x05:
//                this->ampstatus = on;
//                break;
//            case 0x0A://坏了
//                //QMessageBox::warning(QtWidgetsApplication1, tr("警告⚠"), tr("MCU损坏"));
//                this->ampstatus = broken;
//                break;
//            }
//            break;
//        case 0x0A://小电流板连续读取数据
//            this->remodeltemp = (((unsigned char)data[7]) * 256 + (unsigned char)data[8]) / 100.0;
//            this->repumptemp = (((unsigned char)data[9]) * 256 + (unsigned char)data[10]) / 100.0;
//            this->reteccurrent = (((unsigned char)data[11]) * 256 + (unsigned char)data[12]);
//            this->repumppower = (((unsigned char)data[13]) * 256 + (unsigned char)data[14]);
//            this->repumpcurrent = (((unsigned char)data[15]) * 256 + (unsigned char)data[16]);
//            break;
//        case 0x0C://一次回数据
//            this->repumpcurrent = ((unsigned char)data[13]) * 256 + (unsigned char)data[14];
//
//            this->repumppower = ((unsigned char)data[15]) * 256 + (unsigned char)data[16];
//            this->repumptemp = (((unsigned char)data[17]) * 256 + (unsigned char)data[18]) / 100.0;//不知道要不要this
//            break;
//        case 0x16://大电流板返回数据，一次+连续
//            this->pd1power = (((unsigned char)data[7]) * 256 + (unsigned char)data[8]);
//            this->backtemp = (((unsigned char)data[9]) * 256 + (unsigned char)data[10]) / 100.0;
//            this->pd2power = (((unsigned char)data[11]) * 256 + (unsigned char)data[12]);
//            this->pd3power = (((unsigned char)data[13]) * 256 + (unsigned char)data[14]);
//            this->pd4power = (((unsigned char)data[15]) * 256 + (unsigned char)data[16]);
//            this->ld27wtemp = (((unsigned char)data[17]) * 256 + (unsigned char)data[18]) / 100.0;
//            this->ld27wcur = (((unsigned char)data[19]) * 256 + (unsigned char)data[20])/1000.0;
//            this->ld9wcur = (((unsigned char)data[21]) * 256 + (unsigned char)data[22])/1000.0;
//            this->ld27wsetcur = (((unsigned char)data[25]) * 256 + (unsigned char)data[26])/1000.0;
//            this->ld9wsetcur = (((unsigned char)data[27]) * 256 + (unsigned char)data[28])/1000.0;
//            break;
//        default:
//            break;
//        }
//        break;
//    case 0x06://设置电流
//        if (data[7]==0x7E)
//        {
//            this->resetcurrent = ((unsigned char)data[8]) * 256 + (unsigned char)data[9];
//            this->ld27wsetcur = (((unsigned char)data[8]) * 256 + (unsigned char)data[9]) / 1000.0;
//        }
//        else if(data[7]==0x7F)
//            this->ld9wsetcur = (((unsigned char)data[8]) * 256 + (unsigned char)data[9]) / 1000.0;
//        if (this->resetcurrent == 1||(this->ld9wsetcur==0.001&&this->ld27wsetcur==0.0))//新方法，判断是否开启
//        {
//            if (this->ampstatus == on)
//                this->ampstatus = off;//要this
//            else if (this->ampstatus == off)
//                this->ampstatus = on;
//        }
//        break;
//    case 0x99://大电流驱动模块号
//        if (this->ampstatus == off)
//            this->ampstatus = on;
//        break;
//    default:
//        break;
//    }
//}
///**
//  * @Function Name  : OnstatusQuery
//  * @
//  * @brief 开启状态询问
//  * @param None
//  * @retval void
//  */
//void QtLambdapump::OnstatusQuery()
//{
//    this->datasend.resize(6);
//    //this->datasend = QByteArray::fromHex("010355800001");//这可读性不是更差吗……
//    this->datasend[0] = 0x01;
//    this->datasend[1] = 0x03;
//    this->datasend[2] = 0x55;
//    this->datasend[3] = 0x80;
//    this->datasend[4] = 0x00;
//    this->datasend[5] = 0x01;
//}
///**
//  * @Function Name  : MMOnstatusQuery
//  * @
//  * @brief 多模ld驱动板的开启状态询问
//  * @param None
//  * @retval void
//  */
//void QtLambdapump::MMOnstatusQuery()
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0xF8;
//    this->datasend[1] = 0x99;
//    this->datasend[2] = 0x55;
//    this->datasend[3] = 0x02;
//    this->datasend[4] = 0x77;
//    this->datasend[5] = 0x69;
//}
///**
//  * @Function Name  : MMstatusQuery
//  * @
//  * @brief 多模ld驱动板的状态询问
//  * @param None
//  * @retval void
//  */
//void QtLambdapump::MMstatusQuery()
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0x01;
//    this->datasend[1] = 0x03;
//    this->datasend[2] = 0x55;
//    this->datasend[3] = 0x7E;
//    this->datasend[4] = 0x00;
//    this->datasend[5] = 0x0B;
//
//}


///**
//  * @Function Name  : Golightpump
//  * @
//  * @brief Golight的构造函数
//  */
//QtGolightpump::QtGolightpump()
//{
//    datasend.resize(6);
//    datasend[0] = 0xEF;//开启使能
//    datasend[1] = 0xEF;
//    datasend[2] = 0x03;//决定是询问还是设置电流
//    datasend[3] = 0xFF;
//    datasend[4] = 0x04;//决定具体的行为，[2]为05时00就是设置电流
//    datasend[5] = 0x00;//checksum
//    ampstatus = off;
//    repumpcurrent = 0;
//    repumppower = 0;
//    repumptemp = 25.0;
//    resetcurrent = 0;
//    setcurvalue = 0;
//}
//
///**
//  * @Function Name  : SetData
//  * @
//  * @brief 备用
//  * @param None
//  * @retval void
//  */
//void QtGolightpump::SetData()
//{
//
//}
//
///**
//  * @Function Name  : Setcurrent
//  * @
//  * @brief 设置电流，小电流版和大电流版通用
//  * @param value 电流值，单位为mA/A
//  * @retval void
//  */
//void QtGolightpump::Setcurrent(const int value)
//{
//    int temp = value/100;
//    this->datasend.resize(8);
//    this->datasend[0] = 0xEF;//开启使能
//    this->datasend[1] = 0xEF;
//    this->datasend[2] = 0x05;//决定是询问还是设置电流
//    this->datasend[3] = 0xFF;
//    this->datasend[4] = 0x00;//决定具体的行为，[2]为05时00就是设置电流
//    this->datasend[5] = temp / 256;//hi
//    this->datasend[6] = temp % 256;//low
//    //QtWidgetsApplication1::Checksum(datasend);
//}
//
///**
//  * @Function Name  : CurrentQuery
//  * @
//  * @brief 电流询问
//  * @param None
//  * @retval void
//  */
//void QtGolightpump::CurrentQuery()
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0xEF;//
//    this->datasend[1] = 0xEF;
//    this->datasend[2] = 0x03;//决定是询问还是设置电流
//    this->datasend[3] = 0xFF;
//    this->datasend[4] = 0x01;//决定具体的行为
//    //QtWidgetsApplication1::Checksum(datasend);
//}
//
///**
//  * @Function Name  : TempQuery
//  * @
//  * @brief 温度询问
//  * @param None
//  * @retval void
//  */
//void QtGolightpump::TempQuery()
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0xEF;//
//    this->datasend[1] = 0xEF;
//    this->datasend[2] = 0x03;//决定是询问还是设置电流
//    this->datasend[3] = 0xFF;
//    this->datasend[4] = 0x02;//决定具体的行为
//    //QtWidgetsApplication1::Checksum(datasend);
//}
//
///**
//  * @Function Name  : BacklightQuery
//  * @
//  * @brief 背光电流询问
//  * @param None
//  * @retval void
//  */
//void QtGolightpump::BacklightQuery()
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0xEF;//
//    this->datasend[1] = 0xEF;
//    this->datasend[2] = 0x03;//决定是询问还是设置电流
//    this->datasend[3] = 0xFF;
//    this->datasend[4] = 0x03;//决定具体的行为
//   // QtWidgetsApplication1::Checksum(datasend);
//}
///**
//  * @Function Name  : ReInfoData
//  * @
//  * @brief 返回的数据处理
//  * @param data 返回的数据
//  * @retval void
//  */
//void QtGolightpump::ReInfoData(const QByteArray data)
//{
//    switch (data[5])
//    {            //想想怎么设置，return？
//    case 0x00://设置激光器驱动电流目标值
//        if (data[6] == 0x01)
//            //QMessageBox::warning(this, tr("警告⚠"), tr("超过最大值，Amp1已设置为允许最大电流值"));
//            this->repumpcurrent = 1500;//应该换成常量
//            break;
//    case 0x01://获取激光器驱动电流实际值
//        this->repumpcurrent = (((unsigned char)data[6]) * 256 + ((unsigned char)data[7]) )/10;
//        //ui.Amp1CurLcd->display(curvalue);
//
//        break;
//    case 0x02://获取激光器热敏电阻温度值
//        this->repumptemp = (((unsigned char)data[6]) * 256 + ((unsigned char)data[7]) ) / 10.0;
//        //ui.Amp1TempLcd->display(tempvalue);
//        break;
//
//    case 0x03://获取激光器背光电流值
//        this->repumptemp = (((unsigned char)data[6] ) * 256 + ((unsigned char)data[7]));
//        //ui.Amp1BackcurLcd->display(backcurvalue);
//        break;
//    case 0x04://使能激光器输出
//        if (data[6] == 0x01)
//            this->ampstatus = on;
//        //else
//            //QMessageBox::warning(this, tr("警告⚠"), tr("Amp1未能正确开启"));
//        if (this->ampstatus == on)//要不用信号与槽去实现这个功能？应该要多线程吧
//            //ui.Amp1Status->setPixmap(QPixmap("Resources/online.png"));//路径的问题……
//        //else
//            //QMessageBox::warning(this, tr("警告⚠"), tr("Amp1数据不对"));
//            
//            break;
//    case 0x05://关闭激光器输出
//        if (data[6] == 0x00)
//            this->ampstatus = off;
//        else
//            //QMessageBox::warning(this, tr("警告"), tr("Amp1未能正确关闭"));
//        //ui.Amp1Status->setPixmap(QPixmap("Resources/offline.png"));
//            break;
//    default:
//        //QMessageBox::warning(this, tr("警告⚠"), tr("一级存在问题"));
//        break;
//    }
//}
//
///**
//  * @Function Name  : Onofflaser
//  * @
//  * @brief 开启激光器/关闭激光器
//  * @param data 决定开启还是关闭
//  * @retval void
//  */
//void QtGolightpump::Onofflaser(const QByteArray data)
//{
//    this->datasend.resize(6);
//    this->datasend[0] = 0xEF;
//    this->datasend[1] = 0xEF;
//    this->datasend[2] = 0x03;
//    this->datasend[3] = 0xFF;
//    this->datasend[4] = data[0];
//    this->datasend[5] = 0x00;
//
//}
//
//
///**
//  * @Function Name  : QtCotek
//  * @
//  * @brief Cotek的构造函数
//  */
//QtCotek::QtCotek()
//{
//    datasend.resize(9);
//    datasend[0] = 0x50;//P
//    datasend[1] = 0x4F;//O
//    datasend[2] = 0x57;//W
//    datasend[3] = 0x45;//E
//    datasend[4] = 0x52;//R
//    datasend[5] = 0x20;//space
//    datasend[6] = 0x32;//2
//    datasend[7] = 0x0D;//CR
//    datasend[8] = 0x0A;//LF
//
//    ampstatus = off;
//    repumpcurrent = 0;
//    resetcurrent = 0;
//    retemp = 25.0;
//    repumpvoltage = 0.0;
//}
//
//
///**
//  * @Function Name  : Setcurrent
//  * @
//  * @brief 设置电流
//  * @param value，电流值，单位为A
//  * @retval void
//  */
//void QtCotek::Setcurrent(QByteArray value)
//{
//    this->datasend.resize(9);//试试01.5A那边能否接受
//   /* Comandlen[0] = 0x05;
//    Comandlen[1] = SetMainData.size();*/
//
//    //SetMainData[0] = 0x53;//S
//    //SetMainData[1] = 0x49;//I
//    //SetMainData[2] = 0x20;//space
//    //SetMainData[3] = value[0];
//    //SetMainData[4] = 0x2E;//0x2E.//中文点怎么办？试试，好像不行
//    //SetMainData[5] = value[2];
//    //SetMainData[6] = 0x30;//不打0了，自己赋值为0
//    //SetMainData[7] = 0x0D;//CR
//    //SetMainData[8] = 0x0A;//LF
//    this->datasend[0] = 0x53;//S
//    this->datasend[1] = 0x49;//I
//    this->datasend[2] = 0x20;//space
//    this->datasend[3] = value[0];
//    this->datasend[4] = 0x2E; //0x2E;//0x2E.//中文点怎么办？试试，好像不行
//    this->datasend[5] = value[2];
//    this->datasend[6] = 0x30;//不打0了，自己赋值为0
//    this->datasend[7] = 0x0D;//CR
//    this->datasend[8] = 0x0A;//LF
//
//}
//
//
///**
//  * @Function Name  : OutputCurrentQuery
//  * @
//  * @brief 输出电流询问
//  * @param None
//  * @retval void
//  */
//void QtCotek::OutputCurrentQuery()
//{
//    this->datasend.resize(5);
//    this->datasend[0] = 0x52;//R
//    this->datasend[1] = 0x49;//I
//    this->datasend[2] = 0x3F;//?
//    this->datasend[3] = 0x0D;//CR
//    this->datasend[4] = 0x0A;//LF
//}
//
//
///**
//  * @Function Name  : OutputVoltageQuery
//  * @
//  * @brief 输出电压询问
//  * @param None
//  * @retval void
//  */
//void QtCotek::OutputVoltageQuery()
//{
//    this->datasend.resize(5);
//    this->datasend[0] = 0x52;//R
//    this->datasend[1] = 0x56;//V
//    this->datasend[2] = 0x3F;//?
//    this->datasend[3] = 0x0D;//CR
//    this->datasend[4] = 0x0A;//LF
//}
//
//
///**
//  * @Function Name  : ReInfoData
//  * @
//  * @brief 返回的数据处理
//  * @param data 返回的数据
//  */
//void QtCotek::ReInfoData(const QByteArray data)
//{
//    int length = data.size() - 2;
//    switch (data[length - 8])
//    {
//    case 0x30://0，电源关闭，远程关闭
//        this->ampstatus = on;
//        break;
//    case 0x31://1，电源开启，远程关闭
//
//        break;
//    case 0x32://2，电源关闭，远程开启
//        this->ampstatus = off;//这时候一般是关后询问
//        break;
//    case 0x33://3，电源开启，远程开启
//        this->ampstatus = on;
//        
//        break;
//    case 0x41://A
//        this->repumpcurrent = data.sliced(4, 4).toDouble();
//        break;
//    case 0x56://V
//        
//        break;
//
//
//    default:
//        break;
//    }
//}
//
///**
//  * @Function Name  : OnOffstatusQuery
//  * @
//  * @brief 开启关闭状态询问
//  * @param None
//  * @retval void
//  */
//void QtCotek::OnOffstatusQuery()
//{
//    this->datasend.resize(9);
//    this->datasend[0] = 0x50;//P
//    this->datasend[1] = 0x4F;//O
//    this->datasend[2] = 0x57;//W
//    this->datasend[3] = 0x45;//E
//    this->datasend[4] = 0x52;//R
//    this->datasend[5] = 0x20;//space
//    this->datasend[6] = 0x32;//2
//    this->datasend[7] = 0x0D;//CR
//    this->datasend[8] = 0x0A;//LF
//    //this->ampstatus = on;
//}
//
///**
//  * @Function Name  : DeviceStatusQuery
//  * @
//  * @brief 设备状态询问
//  * @param None
//  * @retval Status 返回状态，这样实时性够吗？
//  */
//Status QtCotek::DeviceStatusQuery()
//{
//    this->datasend.resize(8);
//    datasend[0] = 0x53;//S
//    datasend[1] = 0x54;//T
//    datasend[2] = 0x55;//U
//    datasend[3] = 0x53;//S
//    datasend[4] = 0x20;//space
//    datasend[5] = 0x31;//0，只问部分状态，电源
//    datasend[6] = 0x0D;//CR
//    datasend[7] = 0x0A;//LF
//    return this->ampstatus;
//}
//
///**
//  * @Function Name  : OnOffDevice
//  * @
//  * @brief 开启电源/关闭电源
//  * @param None
//  * @retval Status 开启和关闭的状态
//  */
//Status QtCotek::OnOffDevice()
//{
//    if (this->ampstatus == off)
//    {
//        this->datasend.resize(9);
//        this->datasend[0] = 0x50;//P
//        this->datasend[1] = 0x4F;//O
//        this->datasend[2] = 0x57;//W
//        this->datasend[3] = 0x45;//E
//        this->datasend[4] = 0x52;//R
//        this->datasend[5] = 0x20;//space
//        this->datasend[6] = 0x31;//1
//        this->datasend[7] = 0x0D;//CR
//        this->datasend[8] = 0x0A;//LF
//        this->ampstatus = on;
//    }
//    else if (this->ampstatus == on)
//    {
//        this->datasend.resize(9);
//        this->datasend[0] = 0x50;//P
//        this->datasend[1] = 0x4F;//O
//        this->datasend[2] = 0x57;//W
//        this->datasend[3] = 0x45;//E
//        this->datasend[4] = 0x52;//R
//        this->datasend[5] = 0x20;//space
//        this->datasend[6] = 0x30;//0
//        this->datasend[7] = 0x0D;//CR
//        this->datasend[8] = 0x0A;//LF
//        this->ampstatus = off;
//    }
//    else
//    {
//        return this->ampstatus;
//    }
//
//    return this->ampstatus;
//}



//QtWidgetsApplication1::QtWidgetsApplication1()
//{
//
//}

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


/**
  * @Function Name  : on_COMDelayEdit_returnPressed
  * @
  * @brief 通信延时设置，回车触发
  * @param None
  * @retval void
  */
void QtWidgetsApplication1::on_COMDelayEdit_returnPressed()
{
    sleeptime = ui.COMDelayEdit->text().toInt();
}

/* ============================================================
    函数名称 :    ReadPointeratPosition
    功能描述 :    获取环形缓冲区指定位置的指针
    返回值   :    读取的指针
    参数     :    length  输出这个指定位置到缓冲区结尾的长度
    用法     :
   ============================================================*/
char* CircularBuffer::ReadPointeratPosition(qint64 pos, qint64& length)
{
    if (pos >= 0)
    {
        pos += begin;
        for (int i = 0; i < buffers.size(); i++)
        {
            length = (i == endBuffer ? end : buffers[i].size());
            if (length>pos)
            {
                length -= pos;
                return buffers[i].data() + pos;
            }
            pos -= length;
        }
    }
    length = 0;
    return 0;
}

/* ============================================================
    函数名称 :    free
    功能描述 :    释放空间
    返回值   :    Void
    参数     :    bytes  多少字节的内存
    用法     :
   ============================================================*/
void CircularBuffer::free(qint64 bytes)
{
    Q_ASSERT(bytes <= buffersize);
    while (bytes>0)
    {
        const qint64 blockSize = buffers.first().size() - begin;
        if (endBuffer==0||blockSize>bytes)
        {
            if (buffersize<=bytes)
            {
                if (buffers.first().size() <= basicBlockSize)
                {
                    buffersize = 0;
                    begin = end = 0;
                }
                else clear();
            }
            else
        {
            Q_ASSERT(bytes < MaxByteArraySize);
            begin += int(bytes);
            buffersize -= bytes;
        }
            return;
        }
        buffersize -= blockSize;
        bytes -= blockSize;
        buffers.removeFirst();
        --endBuffer;
        begin = 0;
    }
   
}
/* ============================================================
    函数名称 :    Reserve
    功能描述 :    申请空间，从尾开始
    返回值   :    新空间的指针
    参数     :    bytes  多少字节的内存
    用法     :
   ============================================================*/
char* CircularBuffer::Reserve(qint64 bytes)
{
    if (bytes <= 0 || bytes >= MaxByteArraySize)
        return 0;
    if (buffers.isEmpty())
    {
        buffers.append(QByteArray());
        buffers.first().resize(qMax(basicBlockSize, int(bytes)));
    }
    else
    {
        const qint64 newSize = bytes + end;
        //若超过最后一个buffer所含数据的大小，则最后一个buffer需要从新分配
        if (newSize>buffers.last().size())
        {
            //满足以下条件时，将最后一个buffer的容积缩小到其当前所含数据的大小
            //再开辟一个新buffer，并将该buffer数据的结尾位置end设置为0
            if (newSize>buffers.last().capacity()&&(end>=basicBlockSize||newSize>=MaxByteArraySize))
            {
                buffers.last().resize(end);
                buffers.append(QByteArray());
                ++endBuffer;
                end = 0;
            }
            //将最后一个buffer进行扩容
            buffers.last().resize(qMax(basicBlockSize, end + int(bytes)));
        }
    }
    char* writePtr = buffers.last().data() + end;
    buffersize += bytes;
    Q_ASSERT(bytes < MaxByteArraySize);
    end += int(bytes);
    return writePtr;
}

/* ============================================================
    函数名称 :    ReserveFront
    功能描述 :    申请空间，从头开始
    返回值   :    新空间的指针
    参数     :    bytes  多少字节的内存
    用法     :
   ============================================================*/
char* CircularBuffer::ReserveFront(qint64 bytes)
{
    if (bytes <= 0 || bytes >= MaxByteArraySize)
        return 0;

    if (begin<bytes)
    {
        if (buffers.isEmpty())
            buffers.append(QByteArray());
        else
        {
            buffers.first().remove(0, begin);
            if (endBuffer == 0)
                end -= begin;
        }

        begin = qMax(basicBlockSize, int(bytes));
        if (buffersize==0)
        {
            end = begin;
        }
        else
        {
            buffers.prepend(QByteArray());
            ++endBuffer;
        }
        buffers.first().resize(begin);
    }
    begin -= int(bytes);
    buffersize += bytes;
    return buffers.first().data()+begin;
}



/* ============================================================
    函数名称 :    chop
    功能描述 :   从尾释放空间，需配合reserve使用
    返回值   :    Void
    参数     :    length  释放空间的长度
    用法     :
   ============================================================*/
void CircularBuffer::chop(qint64 length)
{
    Q_ASSERT(length <= buffersize);
    while (length>0)
    {
        if (endBuffer==0||end>length)
        {
            if (buffersize <= length)
            {
                if (buffers.first().size() <= basicBlockSize)
                {
                    buffersize = 0;
                    begin = end = 0;
                }
                else clear();
            }
            else
            {
                Q_ASSERT(length < MaxByteArraySize);
                end -= int(length);
                buffersize -= length;
            }
            return;
        }
        //直接把最后一个砍掉？
        buffersize -= end;
        length -= end;
        buffers.removeLast();
        --endBuffer;
        end = buffers.last().size();
    }
}

/* ============================================================
    函数名称 :    clear
    功能描述 :    清空缓存区
    返回值   :    Void
    参数     :     NULL
    用法     :
   ============================================================*/
void CircularBuffer::clear()
{
    if (buffers.isEmpty()) return;
    //为什么要这样分两步处理？
    buffers.erase(buffers.begin() + 1, buffers.end());
    buffers.first().clear();

    begin = end = 0;
    endBuffer = 0;
    buffersize = 0;
}

/* ============================================================
    函数名称 :    Indexof
    功能描述 :    搜寻内部内容
    返回值   :    qint64
    参数     :     char c  搜寻字符，maxlength  最大长度，pos  位置
    用法     :
   ============================================================*/

qint64 CircularBuffer::Indexof(char c, qint64 maxlength, qint64 pos)
{
    if (maxlength <= 0 || pos < 0)
        return -1;
    
    qint64 index = -(pos + begin);
    for (int i = 0; i < buffers.size(); i++)
    {
        qint64 nextBlockIndex = qMin(index + (i == endBuffer ? end : buffers[i].size()), maxlength);

        if (nextBlockIndex>0)
        {
            const char* ptr = buffers[i].data();
            if (index<0)
            {
                ptr -= index;
                index = 0;
            }
            const char* findPtr = reinterpret_cast<const char*>(memchr(ptr, c, nextBlockIndex - index));
            if (findPtr) return qint64(findPtr - ptr) + index + pos;
            if (nextBlockIndex == maxlength) return -1;
        }
        index = nextBlockIndex;
    }
    return -1;
}
/* ============================================================
    函数名称 :    Read
    功能描述 :    读取maxlength长度数据到data中，若buffer<maxlength就读取全部
    返回值   :    qint64 长度
    参数     :     char* data  数据，maxlength  最大长度
    用法     :
   ============================================================*/

qint64 CircularBuffer::Read(char* data, qint64 maxlength)
{
    const qint64 bytesToRead = qMin(buffersize, maxlength);
    qint64 readSoFar = 0;
    while (readSoFar<bytesToRead)
    {
        const qint64 bytesToReadFromThisBlock = qMin(bytesToRead - readSoFar, nextDataBlockSize());
        if (data)
            memcpy(data + readSoFar, readPointer(), bytesToReadFromThisBlock);
        readSoFar += bytesToReadFromThisBlock;
        free(bytesToReadFromThisBlock);
      
    }
    return readSoFar;
}
/* ============================================================
    函数名称 :    Read
    功能描述 :    读取buffers中的第一个buffer
    返回值   :    QByteArray 数据
    参数     :     NULL
    用法     :
   ============================================================*/
QByteArray CircularBuffer::Read()
{
    if(buffersize==0)
        return QByteArray();

    QByteArray qba(buffers.takeFirst());

    //避免调整大小时不必要的内存分配，使QByteArray更高效
    qba.reserve(0);
    if (endBuffer==0)
    {
        qba.resize(end);
        end = 0;
    }
    else
    {
        --endBuffer;
    }
    qba.remove(0, begin);
    begin = 0;
    buffersize -= qba.size();
    return qba;
}
/* ============================================================
    函数名称 :    Peek
    功能描述 :    从指定位置pos拷贝maxlength长度的数据到data中
    返回值   :    qint64 实际截取的长度
    参数     :     data 复制到的地方 maxlength 长度 pos 位置
    用法     :
   ============================================================*/
qint64 CircularBuffer::Peek(char* data, qint64 maxlength, qint64 pos)
{
    qint64 readSoFar = 0;
    if (pos>=0)
    {
        pos += begin;
        for (int i = 0; readSoFar<maxlength&&i<buffers.size(); i++)
        {
            qint64 blockLength = (i == endBuffer ? end : buffers[i].size());

            if (pos < blockLength)
            {
                blockLength = qMin(blockLength - pos, maxlength - readSoFar);
                memcpy(data + readSoFar, buffers[i].data() + pos, blockLength);
                readSoFar += blockLength;
                pos = 0;
            }
            else pos -= blockLength;
        }
    }
    return readSoFar;
}
/* ============================================================
    函数名称 :    append
    功能描述 :    扩展最后一个buffer
    返回值   :    Void
    参数     :     data 数据 size 大小
    用法     :
   ============================================================*/
void CircularBuffer::append(const char* data, qint64 size)
{
    char* writePointer = Reserve(size);
    if (size == 1)
        *writePointer = *data;
    else if (size)
        ::memcpy(writePointer, data, size);
}
/* ============================================================
    函数名称 :    append
    功能描述 :    在最后添加一个新buffer
    返回值   :    Void
    参数     :     qba 新的buffer
    用法     :
   ============================================================*/
void CircularBuffer::append(const QByteArray& qba)
{
    if (end==0)
    {
        if (buffers.isEmpty())
            buffers.append(qba);
        else
            buffers.last() = qba;
    }
    else
    {
        buffers.last().resize(end);
        buffers.append(qba);
        ++endBuffer;
    }
    end = qba.size();
    buffersize += end;
}
/* ============================================================
    函数名称 :   Readline
    功能描述 :    读取一行，包括该行的结束标志'\n'
    返回值   :    Void
    参数     :     data 数据 maxlength 最大长度
    用法     :
   ============================================================*/
qint64 CircularBuffer::Readline(char* data, qint64 maxlength)
{
    if (!data || --maxlength < 0)
        return -1;

    qint64 i = Indexof('\n', maxlength);
    i = Read(data, i >= 0 ? (i + 1) : maxlength);

    data[i] = '\0';
    return i;
}

