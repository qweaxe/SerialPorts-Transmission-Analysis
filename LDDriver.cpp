#include "LDDriver.h"

/**
  * @Function Name  : QtLambdapump
  * @
  * @brief 朗普达的构造函数
  * @param None
  */
QtLambdapump::QtLambdapump()
{
    datasend.resize(8);

    datasend[0] = 0x01;//开启时命令
    datasend[1] = 0x03;//决定询问还是设置电流
    datasend[2] = 0x55;
    datasend[3] = 0x80;
    datasend[4] = 0x00;
    datasend[5] = 0x01;
    datasend[6] = 0x94;
    datasend[7] = 0x2E;
    ampstatus = off;
    repumpcurrent = 0;
    repumppower = 0;
    repumptemp = 25.0;
    rebacktemp = 25.0;
    remodeltemp = 25.0;
    resettectemp = 0.0;
    reteccurrent = 0;
    recavtemp = 0.0;
    reld27wcur = 0.0;
    reld27wsetcur = 0.001;
    reld27wtemp = 25.0;
    reld9wcur = 0.0;
    reld9wsetcur = 0.001;
    pd1power = 0;
    pd2power = 0;
    pd3power = 0;
    pd4power = 0;
    retectemp = 25.0;
    resetcurrent = 1;
    setcurvalue = 0;
}
/**
  * @Function Name  : SetData
  * @
  * @brief 备用
  * @param None
  * @retval void
  */
void QtLambdapump::SetData()
{

}
/**
  * @Function Name  : Setcurrent
  * @
  * @brief 设置电流，小电流驱动板
  * @param value 电流值，单位mA
  * @retval void
  */
void QtLambdapump::Setcurrent(const int value)
{
    this->datasend.resize(6);
    this->datasend[0] = 0x01;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0x55;
    this->datasend[3] = 0x7E;
    this->datasend[4] = value / 256;
    this->datasend[5] = value % 256;
    //QtWidgetsApplication1::CRC16Checksum(datasend);

}
/**
  * @Function Name  : SetMMcurrent
  * @
  * @brief 设置电流，多模ld驱动板
  * @param value 电流值，单位A
  * @retval void
  */
void QtLambdapump::SetMMcurrent(const double value)
{
    uint temp = value * 1000;
    this->datasend.resize(6);
    this->datasend[0] = 0x01;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0x55;
    this->datasend[3] = 0x7E;
    this->datasend[4] = temp / 256;
    this->datasend[5] = temp % 256;
}
/**
  * @Function Name  : Set9Wcurrent
  * @
  * @brief 设置电流，多模ld的9W泵
  * @param value 电流值，单位A
  * @retval void
  */
void QtLambdapump::Set9Wcurrent(const double value)
{
    uint temp = value * 1000;
    this->datasend.resize(6);
    this->datasend[0] = 0x01;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0x55;
    this->datasend[3] = 0x7F;
    this->datasend[4] = temp / 256;
    this->datasend[5] = temp % 256;
}
/**
  * @Function Name  : StatusQuery
  * @
  * @brief 状态询问
  * @param None
  * @retval void
  */
void QtLambdapump::StatusQuery()
{
    this->datasend.resize(6);
    this->datasend[0] = 0x01;
    this->datasend[1] = 0x03;
    this->datasend[2] = 0x55;
    this->datasend[3] = 0x7E;
    this->datasend[4] = 0x00;
    this->datasend[5] = 0x06;
    //QtWidgetsApplication1::CRC16Checksum(datasend);

}
/**
  * @Function Name  : ReInfoData
  * @
  * @brief 收到的数据处理
  * @param data 收到的数据
  * @retval void
  */
void QtLambdapump::ReInfoData(const QByteArray data)
{

        //if ((unsigned char)data[5] != 0x99)
        //qDebug() << "输出结果是：" << data[5] << " 和 " << 0x99;

    switch ((unsigned char)data[5])
    {
    case 0x03:
        switch (data[6])
        {
        case 0x02:
            switch (data[8])
            {
            case 0x00:
                this->ampstatus = on;
                break;
            case 0x05:
                this->ampstatus = on;
                break;
            case 0x0A://坏了
                //QMessageBox::warning(QtWidgetsApplication1, tr("警告⚠"), tr("MCU损坏"));
                this->ampstatus = broken;
                break;
            }
            break;
        case 0x0A://小电流板连续读取数据
            this->remodeltemp = (((unsigned char)data[7]) * 256 + (unsigned char)data[8]) / 100.0;
            this->repumptemp = (((unsigned char)data[9]) * 256 + (unsigned char)data[10]) / 100.0;
            this->reteccurrent = (((unsigned char)data[11]) * 256 + (unsigned char)data[12]);
            this->repumppower = (((unsigned char)data[13]) * 256 + (unsigned char)data[14]);
            this->repumpcurrent = (((unsigned char)data[15]) * 256 + (unsigned char)data[16]);
            break;
        case 0x0C://一次回数据
            this->repumpcurrent = ((unsigned char)data[13]) * 256 + (unsigned char)data[14];

            this->repumppower = ((unsigned char)data[15]) * 256 + (unsigned char)data[16];
            this->repumptemp = (((unsigned char)data[17]) * 256 + (unsigned char)data[18]) / 100.0;//不知道要不要this
            break;
        case 0x0E://双温控返回全部数据
            this->reteccurrent = ((unsigned char)data[7] * 256 + (unsigned char)data[8]);
            this->recavtemp = ((unsigned char)data[9] * 256 + (unsigned char)data[10]) / 100.0;
            this->repumpcurrent = ((unsigned char)data[11] * 256 + (unsigned char)data[12]);
            this->repumppower = ((unsigned char)data[13] * 256 + (unsigned char)data[14]);
            this->repumptemp = ((unsigned char)data[15] * 256 + (unsigned char)data[16]) / 100.0;
            this->rebacktemp = ((unsigned char)data[19] * 256 + (unsigned char)data[20]) / 100.0;
            break;
        case 0x16://大电流板返回数据，一次+连续
            this->pd1power = (((unsigned char)data[7]) * 256 + (unsigned char)data[8]);
            this->rebacktemp = (((unsigned char)data[9]) * 256 + (unsigned char)data[10]) / 100.0;
            this->pd2power = (((unsigned char)data[11]) * 256 + (unsigned char)data[12]);
            this->pd3power = (((unsigned char)data[13]) * 256 + (unsigned char)data[14]);
            this->pd4power = (((unsigned char)data[15]) * 256 + (unsigned char)data[16]);
            this->reld27wtemp = (((unsigned char)data[17]) * 256 + (unsigned char)data[18]) / 100.0;
            this->reld27wcur = (((unsigned char)data[19]) * 256 + (unsigned char)data[20]) / 1000.0;
            this->reld9wcur = (((unsigned char)data[21]) * 256 + (unsigned char)data[22]) / 1000.0;
            this->reld27wsetcur = (((unsigned char)data[25]) * 256 + (unsigned char)data[26]) / 1000.0;
            this->reld9wsetcur = (((unsigned char)data[27]) * 256 + (unsigned char)data[28]) / 1000.0;
            break;
        default:
            break;
        }
        break;
    case 0x06://设置电流
        if (data[7] == 0x7E)
        {
            this->resetcurrent = ((unsigned char)data[8]) * 256 + (unsigned char)data[9];
            this->reld27wsetcur = (((unsigned char)data[8]) * 256 + (unsigned char)data[9]) / 1000.0;
        }
        else if (data[7] == 0x7F)
            this->reld9wsetcur = (((unsigned char)data[8]) * 256 + (unsigned char)data[9]) / 1000.0;
        if (this->resetcurrent == 1 || (this->reld9wsetcur == 0.001 && this->reld27wsetcur == 0.0))//新方法，判断是否开启
        {
            if (this->ampstatus == on)
                this->ampstatus = off;//要this
            else if (this->ampstatus == off)
                this->ampstatus = on;
        }
        break;
    case 0x99://大电流驱动模块号
        if (this->ampstatus == off)
            this->ampstatus = on;
        break;
    default:
        break;
    }
}
/**
  * @Function Name  : OnstatusQuery
  * @
  * @brief 开启状态询问
  * @param None
  * @retval void
  */
void QtLambdapump::OnstatusQuery()
{
    this->datasend.resize(6);
    //this->datasend = QByteArray::fromHex("010355800001");//这可读性不是更差吗……
    this->datasend[0] = 0x01;
    this->datasend[1] = 0x03;
    this->datasend[2] = 0x55;
    this->datasend[3] = 0x80;
    this->datasend[4] = 0x00;
    this->datasend[5] = 0x01;
}
/**
  * @Function Name  : MMOnstatusQuery
  * @
  * @brief 多模ld驱动板的开启状态询问
  * @param None
  * @retval void
  */
void QtLambdapump::MMOnstatusQuery()
{
    this->datasend.resize(6);
    this->datasend[0] = 0xF8;
    this->datasend[1] = 0x99;
    this->datasend[2] = 0x55;
    this->datasend[3] = 0x02;
    this->datasend[4] = 0x77;
    this->datasend[5] = 0x69;
}
/**
  * @Function Name  : MMstatusQuery
  * @
  * @brief 多模ld驱动板的状态询问
  * @param None
  * @retval void
  */
void QtLambdapump::MMstatusQuery()
{
    this->datasend.resize(6);
    this->datasend[0] = 0x01;
    this->datasend[1] = 0x03;
    this->datasend[2] = 0x55;
    this->datasend[3] = 0x7E;
    this->datasend[4] = 0x00;
    this->datasend[5] = 0x0B;

}
/**
  * @Function Name  : SetStatus
  * @
  * @brief 接收别的线程处理后的值
  * @param Amp 另外的线程处理后的pump驱动类的指针
  * @retval void
  */
void QtLambdapump::SetStatus(QtLambdapump* Amp)
{
    this->repumpcurrent = Amp->Pumpcurrent();
    this->repumppower = Amp->Pumppower();
    this->repumptemp = Amp->Pumptemp();
}
/**
  * @Function Name  : QtDhkjpump
  * @
  * @brief 东辉科技的构造函数
  * @param None
  */
QtDhkjpump::QtDhkjpump()
{
    datasend.resize(5);
    //datasend.fromHex("0x55,0xAA,0x03,0x73,0x73");
    datasend[0] = 0x55;
    datasend[1] = 0xAA;
    datasend[2] = 0x03;
    datasend[3] = 0x73;
    datasend[4] = 0x73;
    R7 = 0.0;
    lnR7 = 0.0;
    recavtemp = 25.0;
    repumpcurrent = 0.0;
    repumppower = 0.0;
    repumptemp = 25.0;
    resetcurrent = 0;
    retempcur = 0.0;
    r = 0;
    setcurvalue = 0;
    u3 = 0;
}
/**
  * @Function Name  :Setcurrent
  * @
  * @brief 设置电流
  * @param value 设置的值
  * @retval void
  */
void QtDhkjpump::Setcurrent(double value)
{
    this->datasend.resize(5);
    this->datasend[0] = 0x55;
    this->datasend[1] = 0xAA;
    this->datasend[2] = 0x02;
    this->datasend[3] = value / 256;
    this->datasend[4] = (uint)value % 256;
}
/**
  * @Function Name  : ReInfoData
  * @
  * @brief 收到的数据处理
  * @param data 数据
  * @retval void
  */
void QtDhkjpump::ReInfoData(const QByteArray data)
{
    this->reseed->resize(4);
    this->reseed[0] = data.sliced(4, 4);//空
    this->reseed[1] = data.sliced(8, 4);//空
    this->reseed[2] = data.sliced(12, 4);//空
    this->reseed[3] = data.sliced(16, 4);//泵浦激光器温度
    this->reseed[4] = data.sliced(20, 4);//泵浦制冷电流
    this->reseed[5] = data.sliced(24, 4);//泵浦激光器功率
    this->reseed[6] = data.sliced(28, 4);//泵浦激光器驱动电流
    this->reseed[7] = data.sliced(32, 4);//激光腔温度
    //temperature
    this->u3 = ((reseed[3][2] & 0x03) * 256 + reseed[3][3]) * 2;
    this->r = (10000 * u3) / (2048 - u3);
    this->repumptemp = (37000 - r) / 600;
    //temperature current
    this->retempcur = ((((unsigned char)reseed[4][2]) & 0x03) * 256 + (unsigned char)reseed[4][3]) * 2 * 1.4;
    //pump power
    this->repumppower = ((((unsigned char)reseed[5][2]) & 0x03) * 256 + (unsigned char)reseed[5][3]) * 2 * 0.36361;
    //pump current
    this->repumpcurrent = ((((unsigned char)reseed[6][2]) & 0x03) * 256 + (unsigned char)reseed[6][3]) * 2 * 0.4;
    //pump temperature
    this->R7 = (((unsigned char)reseed[7][2]) * 256 + ((unsigned char)reseed[7][3])) * 1.0 * 0.625;
    this->lnR7 = log(R7);
    this->recavtemp = pow(((1.141256846 * pow(10, -3)) + 2.325154750 * pow(10, -4) * lnR7 + lnR7 * lnR7 * lnR7 * 0.9126356973 * pow(10, -7)), -1) - 273.15;
}

/**
  * @Function Name  : Golightpump
  * @
  * @brief Golight的构造函数
  */
QtGolightpump::QtGolightpump()
{
    datasend.resize(6);
    datasend[0] = 0xEF;//开启使能
    datasend[1] = 0xEF;
    datasend[2] = 0x03;//决定是询问还是设置电流
    datasend[3] = 0xFF;
    datasend[4] = 0x04;//决定具体的行为，[2]为05时00就是设置电流
    datasend[5] = 0x00;//checksum
    ampstatus = off;
    repumpcurrent = 0;
    repumppower = 0;
    repumptemp = 25.0;
    resetcurrent = 0;
    setcurvalue = 0;
}

/**
  * @Function Name  : SetData
  * @
  * @brief 备用
  * @param None
  * @retval void
  */
void QtGolightpump::SetData()
{

}

/**
  * @Function Name  : Setcurrent
  * @
  * @brief 设置电流，小电流版和大电流版通用
  * @param value 电流值，单位为mA/A
  * @retval void
  */
void QtGolightpump::Setcurrent(const int value)
{
    int temp = value / 100;
    this->datasend.resize(8);
    this->datasend[0] = 0xEF;//开启使能
    this->datasend[1] = 0xEF;
    this->datasend[2] = 0x05;//决定是询问还是设置电流
    this->datasend[3] = 0xFF;
    this->datasend[4] = 0x00;//决定具体的行为，[2]为05时00就是设置电流
    this->datasend[5] = temp / 256;//hi
    this->datasend[6] = temp % 256;//low
    //QtWidgetsApplication1::Checksum(datasend);
}

/**
  * @Function Name  : CurrentQuery
  * @
  * @brief 电流询问
  * @param None
  * @retval void
  */
void QtGolightpump::CurrentQuery()
{
    this->datasend.resize(6);
    this->datasend[0] = 0xEF;//
    this->datasend[1] = 0xEF;
    this->datasend[2] = 0x03;//决定是询问还是设置电流
    this->datasend[3] = 0xFF;
    this->datasend[4] = 0x01;//决定具体的行为
    //QtWidgetsApplication1::Checksum(datasend);
}

/**
  * @Function Name  : TempQuery
  * @
  * @brief 温度询问
  * @param None
  * @retval void
  */
void QtGolightpump::TempQuery()
{
    this->datasend.resize(6);
    this->datasend[0] = 0xEF;//
    this->datasend[1] = 0xEF;
    this->datasend[2] = 0x03;//决定是询问还是设置电流
    this->datasend[3] = 0xFF;
    this->datasend[4] = 0x02;//决定具体的行为
    //QtWidgetsApplication1::Checksum(datasend);
}

/**
  * @Function Name  : BacklightQuery
  * @
  * @brief 背光电流询问
  * @param None
  * @retval void
  */
void QtGolightpump::BacklightQuery()
{
    this->datasend.resize(6);
    this->datasend[0] = 0xEF;//
    this->datasend[1] = 0xEF;
    this->datasend[2] = 0x03;//决定是询问还是设置电流
    this->datasend[3] = 0xFF;
    this->datasend[4] = 0x03;//决定具体的行为
   // QtWidgetsApplication1::Checksum(datasend);
}
/**
  * @Function Name  : ReInfoData
  * @
  * @brief 返回的数据处理
  * @param data 返回的数据
  * @retval void
  */
void QtGolightpump::ReInfoData(const QByteArray data)
{
    switch (data[5])
    {            //想想怎么设置，return？
    case 0x00://设置激光器驱动电流目标值
        if (data[6] == 0x01)
            //QMessageBox::warning(this, tr("警告⚠"), tr("超过最大值，Amp1已设置为允许最大电流值"));
            this->repumpcurrent = 1500;//应该换成常量
        break;
    case 0x01://获取激光器驱动电流实际值
        this->repumpcurrent = (((unsigned char)data[6]) * 256 + ((unsigned char)data[7])) / 10;
        //ui.Amp1CurLcd->display(curvalue);

        break;
    case 0x02://获取激光器热敏电阻温度值
        this->repumptemp = (((unsigned char)data[6]) * 256 + ((unsigned char)data[7])) / 10.0;
        //ui.Amp1TempLcd->display(tempvalue);
        break;

    case 0x03://获取激光器背光电流值
        this->repumptemp = (((unsigned char)data[6]) * 256 + ((unsigned char)data[7]));
        //ui.Amp1BackcurLcd->display(backcurvalue);
        break;
    case 0x04://使能激光器输出
        if (data[6] == 0x01)
            this->ampstatus = on;
        //else
            //QMessageBox::warning(this, tr("警告⚠"), tr("Amp1未能正确开启"));
        if (this->ampstatus == on)//要不用信号与槽去实现这个功能？应该要多线程吧
            //ui.Amp1Status->setPixmap(QPixmap("Resources/online.png"));//路径的问题……
        //else
            //QMessageBox::warning(this, tr("警告⚠"), tr("Amp1数据不对"));

            break;
    case 0x05://关闭激光器输出
        if (data[6] == 0x00)
            this->ampstatus = off;
        else
            //QMessageBox::warning(this, tr("警告"), tr("Amp1未能正确关闭"));
        //ui.Amp1Status->setPixmap(QPixmap("Resources/offline.png"));
            break;
    default:
        //QMessageBox::warning(this, tr("警告⚠"), tr("一级存在问题"));
        break;
    }
}

/**
  * @Function Name  : Onofflaser
  * @
  * @brief 开启激光器/关闭激光器
  * @param data 决定开启还是关闭
  * @retval void
  */
void QtGolightpump::Onofflaser(const QByteArray data)
{
    this->datasend.resize(6);
    this->datasend[0] = 0xEF;
    this->datasend[1] = 0xEF;
    this->datasend[2] = 0x03;
    this->datasend[3] = 0xFF;
    this->datasend[4] = data[0];
    this->datasend[5] = 0x00;

}


/**
  * @Function Name  : QtCotek
  * @
  * @brief Cotek的构造函数
  */
QtCotek::QtCotek()
{
    datasend.resize(9);
    datasend[0] = 0x50;//P
    datasend[1] = 0x4F;//O
    datasend[2] = 0x57;//W
    datasend[3] = 0x45;//E
    datasend[4] = 0x52;//R
    datasend[5] = 0x20;//space
    datasend[6] = 0x32;//2
    datasend[7] = 0x0D;//CR
    datasend[8] = 0x0A;//LF

    ampstatus = off;
    repumpcurrent = 0;
    resetcurrent = 0;
    retemp = 25.0;
    repumpvoltage = 0.0;
}


/**
  * @Function Name  : Setcurrent
  * @
  * @brief 设置电流
  * @param value，电流值，单位为A
  * @retval void
  */
void QtCotek::Setcurrent(QByteArray value)
{
    this->datasend.resize(9);//试试01.5A那边能否接受
   /* Comandlen[0] = 0x05;
    Comandlen[1] = SetMainData.size();*/

    //SetMainData[0] = 0x53;//S
    //SetMainData[1] = 0x49;//I
    //SetMainData[2] = 0x20;//space
    //SetMainData[3] = value[0];
    //SetMainData[4] = 0x2E;//0x2E.//中文点怎么办？试试，好像不行
    //SetMainData[5] = value[2];
    //SetMainData[6] = 0x30;//不打0了，自己赋值为0
    //SetMainData[7] = 0x0D;//CR
    //SetMainData[8] = 0x0A;//LF
    this->datasend[0] = 0x53;//S
    this->datasend[1] = 0x49;//I
    this->datasend[2] = 0x20;//space
    this->datasend[3] = value[0];
    this->datasend[4] = 0x2E; //0x2E;//0x2E.//中文点怎么办？试试，好像不行
    this->datasend[5] = value[2];
    this->datasend[6] = 0x30;//不打0了，自己赋值为0
    this->datasend[7] = 0x0D;//CR
    this->datasend[8] = 0x0A;//LF

}


/**
  * @Function Name  : OutputCurrentQuery
  * @
  * @brief 输出电流询问
  * @param None
  * @retval void
  */
void QtCotek::OutputCurrentQuery()
{
    this->datasend.resize(5);
    this->datasend[0] = 0x52;//R
    this->datasend[1] = 0x49;//I
    this->datasend[2] = 0x3F;//?
    this->datasend[3] = 0x0D;//CR
    this->datasend[4] = 0x0A;//LF
}


/**
  * @Function Name  : OutputVoltageQuery
  * @
  * @brief 输出电压询问
  * @param None
  * @retval void
  */
void QtCotek::OutputVoltageQuery()
{
    this->datasend.resize(5);
    this->datasend[0] = 0x52;//R
    this->datasend[1] = 0x56;//V
    this->datasend[2] = 0x3F;//?
    this->datasend[3] = 0x0D;//CR
    this->datasend[4] = 0x0A;//LF
}


/**
  * @Function Name  : ReInfoData
  * @
  * @brief 返回的数据处理
  * @param data 返回的数据
  */
void QtCotek::ReInfoData(const QByteArray data)
{
    int length = data.size() - 2;
    switch (data[length - 8])
    {
    case 0x30://0，电源关闭，远程关闭
        this->ampstatus = on;
        break;
    case 0x31://1，电源开启，远程关闭

        break;
    case 0x32://2，电源关闭，远程开启
        this->ampstatus = off;//这时候一般是关后询问
        break;
    case 0x33://3，电源开启，远程开启
        this->ampstatus = on;

        break;
    case 0x41://A
        this->repumpcurrent = data.sliced(4, 4).toDouble();
        break;
    case 0x56://V

        break;


    default:
        break;
    }
}

/**
  * @Function Name  : OnOffstatusQuery
  * @
  * @brief 开启关闭状态询问
  * @param None
  * @retval void
  */
void QtCotek::OnOffstatusQuery()
{
    this->datasend.resize(9);
    this->datasend[0] = 0x50;//P
    this->datasend[1] = 0x4F;//O
    this->datasend[2] = 0x57;//W
    this->datasend[3] = 0x45;//E
    this->datasend[4] = 0x52;//R
    this->datasend[5] = 0x20;//space
    this->datasend[6] = 0x32;//2
    this->datasend[7] = 0x0D;//CR
    this->datasend[8] = 0x0A;//LF
    //this->ampstatus = on;
}

/**
  * @Function Name  : DeviceStatusQuery
  * @
  * @brief 设备状态询问
  * @param None
  * @retval Status 返回状态，这样实时性够吗？
  */
Status QtCotek::DeviceStatusQuery()
{
    this->datasend.resize(8);
    datasend[0] = 0x53;//S
    datasend[1] = 0x54;//T
    datasend[2] = 0x55;//U
    datasend[3] = 0x53;//S
    datasend[4] = 0x20;//space
    datasend[5] = 0x31;//0，只问部分状态，电源
    datasend[6] = 0x0D;//CR
    datasend[7] = 0x0A;//LF
    return this->ampstatus;
}

/**
  * @Function Name  : OnOffDevice
  * @
  * @brief 开启电源/关闭电源
  * @param None
  * @retval Status 开启和关闭的状态
  */
Status QtCotek::OnOffDevice()
{
    if (this->ampstatus == off)
    {
        this->datasend.resize(9);
        this->datasend[0] = 0x50;//P
        this->datasend[1] = 0x4F;//O
        this->datasend[2] = 0x57;//W
        this->datasend[3] = 0x45;//E
        this->datasend[4] = 0x52;//R
        this->datasend[5] = 0x20;//space
        this->datasend[6] = 0x31;//1
        this->datasend[7] = 0x0D;//CR
        this->datasend[8] = 0x0A;//LF
        this->ampstatus = on;
    }
    else if (this->ampstatus == on)
    {
        this->datasend.resize(9);
        this->datasend[0] = 0x50;//P
        this->datasend[1] = 0x4F;//O
        this->datasend[2] = 0x57;//W
        this->datasend[3] = 0x45;//E
        this->datasend[4] = 0x52;//R
        this->datasend[5] = 0x20;//space
        this->datasend[6] = 0x30;//0
        this->datasend[7] = 0x0D;//CR
        this->datasend[8] = 0x0A;//LF
        this->ampstatus = off;
    }
    else
    {
        return this->ampstatus;
    }

    return this->ampstatus;
}
