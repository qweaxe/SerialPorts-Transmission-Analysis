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
void QtLambdapump::Set27Wcurrent(const double value)
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
            this->reld27wtemp = (data[17]* 256 + (unsigned char) data[18]) / 100.0;
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
        if (this->resetcurrent == 1 || ((abs(this->reld9wsetcur -0.001)<0.001) && abs(this->reld27wsetcur -0.001)<0.001))//新方法，判断是否开启
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
    setpump1cur = 0.0;
    setpump2cur = 0.0;
    resetpump1cur = 0.0;
    resetpump2cur = 0.0;
    repump1cur = 0.0;
    repump2cur = 0.0;
}
/**
  * @Function Name  : Onoffenginmode
  * @
  * @brief 东辉科技的工程模式开关
  * @param s 开关的指令
  */
void QtDhkjpump::Onoffenginmode(bool s)
{
    if (s == on)
    {
        this->datasend.resize(12);
        this->datasend[0] = 0x5A;
        this->datasend[1] = 0x46;
        this->datasend[2] = 0x00;
        this->datasend[3] = 0xEC;
        this->datasend[4] = 0x00;
        this->datasend[5] = 0x00;
        this->datasend[6] = 0x00;
        this->datasend[7] = 0x00;
        this->datasend[8] = 0x00;
        this->datasend[9] = 0x00;//校验和
        this->datasend[10] = 0x00;//校验和
        this->datasend[11] = 0xA5;
    }
    else if (s == off)
    {
        this->datasend.resize(12);
        this->datasend[0] = 0x5A;
        this->datasend[1] = 0x46;
        this->datasend[2] = 0x00;
        this->datasend[3] = 0xCE;
        this->datasend[4] = 0x00;
        this->datasend[5] = 0x00;
        this->datasend[6] = 0x00;
        this->datasend[7] = 0x00;
        this->datasend[8] = 0x00;
        this->datasend[9] = 0x00;//校验和
        this->datasend[10] = 0x00;//校验和
        this->datasend[11] = 0xA5;
    }
}
/**
  * @Function Name  : Onofflaser
  * @
  * @brief 东辉科技的激光器开关
  * @param s 开关的指令
  */
void QtDhkjpump::Onofflaser(bool s)
{
    if (s == on)
    {
        this->datasend.resize(12);
        this->datasend[0] = 0x5A;
        this->datasend[1] = 0x46;
        this->datasend[2] = 0xAA;
        this->datasend[3] = 0x00;
        this->datasend[4] = 0x40;
        this->datasend[5] = 0x00;
        this->datasend[6] = 0x01;
        this->datasend[7] = 0x00;
        this->datasend[8] = 0x00;
        this->datasend[9] = 0x00;//校验和
        this->datasend[10] = 0x00;//校验和
        this->datasend[11] = 0xA5;

    }
    else if (s == off)
    {
        this->datasend.resize(12);
        this->datasend[0] = 0x5A;
        this->datasend[1] = 0x46;
        this->datasend[2] = 0xAA;
        this->datasend[3] = 0x00;
        this->datasend[4] = 0x40;
        this->datasend[5] = 0x00;
        this->datasend[6] = 0x00;
        this->datasend[7] = 0x00;
        this->datasend[8] = 0x00;
        this->datasend[9] = 0x00;//校验和
        this->datasend[10] = 0x00;//校验和
        this->datasend[11] = 0xA5;
    }
}
/**
  * @Function Name  : OnoffstatusQuery
  * @
  * @brief 东辉科技的激光器开关状态查询
  * @param 
  */
void QtDhkjpump::OnoffstatusQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x00;
    this->datasend[4] = 0x40;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  :SetLD1current
  * @
  * @brief 设置电流
  * @param value 设置的值
  * @retval void
  */
void QtDhkjpump::SetLD1current(float value)
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x46;
    this->datasend[2] = 0xAA;
    this->datasend[3] = 0x11;
    this->datasend[4] = 0xF4;
    floatToIEEE754(value);
    this->datasend[9] = 0x00;//校验和高位
    this->datasend[10] = 0x00;//校验和低位
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  :SetLD1current
  * @
  * @brief 设置电流
  * @param value 设置的值
  * @retval void
  */
void QtDhkjpump::SetLD2current(float value)
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x46;
    this->datasend[2] = 0xAA;
    this->datasend[3] = 0x11;
    this->datasend[4] = 0xF5;
    floatToIEEE754(value);
    this->datasend[9] = 0x00;//校验和高位
    this->datasend[10] = 0x00;//校验和低位
    this->datasend[11] = 0xA5;
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
    switch ((unsigned char)data[2])
    {
    case 0x00:
        if ((unsigned char)data[3] == 0xEC)
        {
            this->enginmodes = on;
        }
        else if ((unsigned char)data[3] == 0xCE)
        {
            this->enginmodes = off;
        }
        else
            qDebug() << "Status Error ! Command: "<<data;
        break;
    case 0xAA:
        switch ((unsigned char)data[03])
        {
            case 0x00:
                if ((unsigned char)data[06] == 0x00)
                    this->ampstatus = off;
                else if ((unsigned char)data[06] == 0x01)
                    this->ampstatus = on;
                break;
            case 0x11://pump电流设置
                if ((unsigned char)data[04] == 0xF4)
                {
                    myfloat.buf[0] = (unsigned char)data[8];
                    myfloat.buf[1] = (unsigned char)data[7];
                    myfloat.buf[2] = (unsigned char)data[6];
                    myfloat.buf[3] = (unsigned char)data[5];
                    this->resetpump1cur = myfloat.f;
                }
                else if ((unsigned char)data[04] == 0xF5)
                {
                    myfloat.buf[0] = (unsigned char)data[8];
                    myfloat.buf[1] = (unsigned char)data[7];
                    myfloat.buf[2] = (unsigned char)data[6];
                    myfloat.buf[3] = (unsigned char)data[5];
                    this->resetpump2cur = myfloat.f;
                }

                break;
        }
        break;

    case 0xBB://BB表示读取
        switch ((unsigned char)data[3])
        {
            case 0x00:
                if ((unsigned char)data[6] == 0x01)
                    this->ampstatus = on;
                else if ((unsigned char)data[6] == 0x00)
                    this->ampstatus = off;
            break;
            case 0x10://实时电流
                if ((unsigned char)data[04] == 0xF4)
                {
                    myfloat.buf[0] = (unsigned char)data[8];
                    myfloat.buf[1] = (unsigned char)data[7];
                    myfloat.buf[2] = (unsigned char)data[6];
                    myfloat.buf[3] = (unsigned char)data[5];
                    this->repump1cur = myfloat.f;
                }
                else if ((unsigned char)data[04] == 0xF5)
                {
                    myfloat.buf[0] = (unsigned char)data[8];
                    myfloat.buf[1] = (unsigned char)data[7];
                    myfloat.buf[2] = (unsigned char)data[6];
                    myfloat.buf[3] = (unsigned char)data[5];
                    this->repump2cur = myfloat.f;
                }
             break;
            case 0x11://设置电流
                if ((unsigned char)data[04] == 0xF4)
                {
                    myfloat.buf[0] = (unsigned char)data[8];
                    myfloat.buf[1] = (unsigned char)data[7];
                    myfloat.buf[2] = (unsigned char)data[6];
                    myfloat.buf[3] = (unsigned char)data[5];
                    this->resetpump1cur = myfloat.f;
                }
                else if ((unsigned char)data[04] == 0xF5)
                {
                    myfloat.buf[0] = (unsigned char)data[8];
                    myfloat.buf[1] = (unsigned char)data[7];
                    myfloat.buf[2] = (unsigned char)data[6];
                    myfloat.buf[3] = (unsigned char)data[5];
                    this->resetpump2cur = myfloat.f;
                }
             break;
            case 0x14://最大设置电流

             break;
        }
        break;
    default:

        break;
    }
}
/**
  * @Function Name  : LD1curQuery
  * @
  * @brief 东辉科技的LD1电流询问指令
  * @param
  */
void QtDhkjpump::LD1curQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x10;
    this->datasend[4] = 0xF4;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  : LD2curQuery
  * @
  * @brief 东辉科技的LD2电流询问指令
  * @param 
  */
void QtDhkjpump::LD2curQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x10;
    this->datasend[4] = 0xF5;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  : LD1SetcurQuery
  * @
  * @brief 东辉科技的LD1设置电流询问指令
  * @param
  */
void QtDhkjpump::LD1SetcurQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x11;
    this->datasend[4] = 0xF4;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  : LD2SetcurQuery
  * @
  * @brief 东辉科技的LD2设置电流询问指令
  * @param
  */
void QtDhkjpump::LD2SetcurQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x14;
    this->datasend[4] = 0xF5;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  : LD1maxcurQuery
  * @
  * @brief 东辉科技的LD1最大设置电流询问指令
  * @param
  */
void QtDhkjpump::LD1maxcurQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x14;
    this->datasend[4] = 0xF4;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  : LD2maxcurQuery
  * @
  * @brief 东辉科技的LD2最大设置电流询问指令
  * @param
  */
void QtDhkjpump::LD2maxcurQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x06;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x14;
    this->datasend[4] = 0xF5;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}
/**
  * @Function Name  : FloatToIEEE754
  * @
  * @brief 将单精度浮点数转换为4个32位
  * @param value 待转换数值
  * @retval 对应的字符串
  */
void QtDhkjpump::floatToIEEE754(float value)//顺序可能要改一下
{
    //unsigned int intValue = *reinterpret_cast<unsigned int*>(&value);
    //std::bitset<sizeof(float) * 8> bits(intValue);
   
    myfloat.f = value;
    this->datasend[5] = myfloat.buf[0];
    this->datasend[6] = myfloat.buf[1];
    this->datasend[7] = myfloat.buf[2];
    this->datasend[8] = myfloat.buf[3];
}

void QtDhkjpump::SetStatus(QtDhkjpump* Amp)
{
    this->repump1cur = Amp->repump1cur;
    this->repump2cur = Amp->repump2cur;
    this->resetpump1cur = Amp->resetpump1cur;
    this->resetpump2cur = Amp->resetpump2cur;

}
/**
  * @Function Name  : QtdhkjMMpump
  * @
  * @brief 东辉科技多模泵的构造函数
  * @param
  */
QtDhkjMMpump::QtDhkjMMpump()
{
    resetpumpcur = 0.0;
    repumpcur = 0.0;
}

/**
  * @Function Name  : Onofflaser
  * @
  * @brief 东辉科技多模泵的开关
  * @param s 开关状态指令
  */
void QtDhkjMMpump::Onofflaser(bool s)
{
    if (s == on)
    {
        this->datasend.resize(12);
        this->datasend[0] = 0x5A;
        this->datasend[1] = 0x08;
        this->datasend[2] = 0x77;
        this->datasend[3] = 0x00;
        this->datasend[4] = 0x00;
        this->datasend[5] = 0x00;
        this->datasend[6] = 0x00;
        this->datasend[7] = 0x00;
        this->datasend[8] = 0x00;
        this->datasend[9] = 0x00;//校验和
        this->datasend[10] = 0x00;//校验和
        this->datasend[11] = 0xA5;

    }
    else if (s == off)
    {
        this->datasend.resize(12);
        this->datasend[0] = 0x5A;
        this->datasend[1] = 0x08;
        this->datasend[2] = 0x55;
        this->datasend[3] = 0x00;
        this->datasend[4] = 0x00;
        this->datasend[5] = 0x00;
        this->datasend[6] = 0x00;
        this->datasend[7] = 0x00;
        this->datasend[8] = 0x00;
        this->datasend[9] = 0x00;//校验和
        this->datasend[10] = 0x00;//校验和
        this->datasend[11] = 0xA5;
    }
}

/**
  * @Function Name  : SetMMLDcurrent
  * @
  * @brief 东辉科技多模泵的设置电流
  * @param
  */
void QtDhkjMMpump::SetMMLDcurrent(float value)
{
    myfloat.f = value;
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x48;
    this->datasend[2] = 0xAA;
    this->datasend[3] = 0x11;
    this->datasend[4] = 0xF5;
    floatToIEEE754(value);
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}

/**
  * @Function Name  : ReInfoData
  * @
  * @brief 东辉科技多模泵的返回指令解析
  * @param data 返回的指令
  */
void QtDhkjMMpump::ReInfoData(const QByteArray data)
{
    switch ((unsigned char)data[1])
    {
    case 0x88:
        if ((unsigned char)data[7] == 0x00 && (unsigned char)data[8] == 0x01)
            this->ampstatus = on;
        else if ((unsigned char)data[7] == 0x00 && (unsigned char)data[8] == 0x00)
            this->ampstatus = off;
        break;
    case 0xC8:
        if ((unsigned char)data[3] == 0x10)
        {
            myfloat.buf[0] = (unsigned char)data[8];
            myfloat.buf[1] = (unsigned char)data[7];
            myfloat.buf[2] = (unsigned char)data[6];
            myfloat.buf[3] = (unsigned char)data[5];
            this->repumpcur = myfloat.f;
        }
        else if ((unsigned char)data[3] == 0x11)
        {
            myfloat.buf[0] = (unsigned char)data[8];
            myfloat.buf[1] = (unsigned char)data[7];
            myfloat.buf[2] = (unsigned char)data[6];
            myfloat.buf[3] = (unsigned char)data[5];
            this->resetpumpcur  = myfloat.f;
        }
        break;

    default:

        break;
   }
}

/**
  * @Function Name  : MMLDcurQuery
  * @
  * @brief 东辉科技多模泵的电流查询
  * @param data 返回的指令
  */
void QtDhkjMMpump::MMLDcurQuery()
{
    this->datasend.resize(12);
    this->datasend[0] = 0x5A;
    this->datasend[1] = 0x48;
    this->datasend[2] = 0xBB;
    this->datasend[3] = 0x10;
    this->datasend[4] = 0xF5;
    this->datasend[5] = 0x00;
    this->datasend[6] = 0x00;
    this->datasend[7] = 0x00;
    this->datasend[8] = 0x00;
    this->datasend[9] = 0x00;//校验和
    this->datasend[10] = 0x00;//校验和
    this->datasend[11] = 0xA5;
}

/**
  * @Function Name  : FloatToIEEE754
  * @
  * @brief 将单精度浮点数转换为4个32位
  * @param value 待转换数值
  * @retval 对应的字符串
  */
void QtDhkjMMpump::floatToIEEE754(float value)
{
        myfloat.f = value;
        this->datasend[5] = myfloat.buf[0];
        this->datasend[6] = myfloat.buf[1];
        this->datasend[7] = myfloat.buf[2];
        this->datasend[8] = myfloat.buf[3];
}

void QtDhkjMMpump::SetStatus(QtDhkjMMpump* Amp)
{
    this->repumpcur = Amp->repumpcur;
    this->resetpumpcur = Amp->resetpumpcur;
    

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

