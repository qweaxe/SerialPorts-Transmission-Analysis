/********************************************************************************
** Form generated from reading UI file 'QtWidgetsApplication1jjbbnH.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef QTWIDGETSAPPLICATION1JJBBNH_H
#define QTWIDGETSAPPLICATION1JJBBNH_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsApplication1Class
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox2;
    QLabel *COM1Status;
    QLineEdit *COM1PowerEdit;
    QPushButton *SetCOM1Button;
    QLCDNumber *COM1CurLcd;
    QLCDNumber *COM1TempLcd;
    QLCDNumber *COM1PowerLcd;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_25;
    QLabel *label_32;
    QLabel *label_36;
    QLabel *label_37;
    QGroupBox *groupBox3;
    QLabel *COM2Status;
    QLineEdit *COM2PowerEdit;
    QPushButton *SetCOM2Button;
    QLCDNumber *COM227WCurLcd;
    QLCDNumber *COM2TempLcd;
    QLCDNumber *COM227WLDTempLcd;
    QLabel *label_16;
    QLabel *label_15;
    QLabel *label_14;
    QLabel *label_26;
    QLabel *label_33;
    QLabel *label_38;
    QLabel *label_39;
    QGroupBox *groupBox0;
    QPushButton *pushButton;
    QComboBox *ComComboBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *statuslabel;
    QPushButton *GetcomButton;
    QPushButton *EnlaserButton;
    QPushButton *GetLDStatusBtn;
    QGroupBox *groupBox1;
    QLCDNumber *COM0CurLcd;
    QPushButton *SetCOM0Button;
    QLineEdit *COM0PowerEdit;
    QLCDNumber *COM0PowerLcd;
    QLCDNumber *COM0TempLcd;
    QLabel *COM0Status;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_24;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_35;
    QGroupBox *groupBox;
    QLabel *label_4;
    QLabel *label_5;
    QTextEdit *SendtextEdit;
    QTextEdit *ReceivetextEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtWidgetsApplication1Class)
    {
        if (QtWidgetsApplication1Class->objectName().isEmpty())
            QtWidgetsApplication1Class->setObjectName(QString::fromUtf8("QtWidgetsApplication1Class"));
        QtWidgetsApplication1Class->resize(603, 720);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtWidgetsApplication1Class->sizePolicy().hasHeightForWidth());
        QtWidgetsApplication1Class->setSizePolicy(sizePolicy);
        QtWidgetsApplication1Class->setMinimumSize(QSize(603, 720));
        QtWidgetsApplication1Class->setMaximumSize(QSize(603, 720));
        QtWidgetsApplication1Class->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/ufologo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        QtWidgetsApplication1Class->setWindowIcon(icon);
        centralWidget = new QWidget(QtWidgetsApplication1Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QFont font;
        font.setPointSize(11);
        centralWidget->setFont(font);
        groupBox2 = new QGroupBox(centralWidget);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        groupBox2->setGeometry(QRect(10, 250, 581, 131));
        groupBox2->setMinimumSize(QSize(581, 131));
        groupBox2->setMaximumSize(QSize(581, 131));
        COM1Status = new QLabel(groupBox2);
        COM1Status->setObjectName(QString::fromUtf8("COM1Status"));
        COM1Status->setGeometry(QRect(220, 50, 31, 31));
        COM1Status->setPixmap(QPixmap(QString::fromUtf8(":/images/offline.png")));
        COM1Status->setScaledContents(true);
        COM1PowerEdit = new QLineEdit(groupBox2);
        COM1PowerEdit->setObjectName(QString::fromUtf8("COM1PowerEdit"));
        COM1PowerEdit->setGeometry(QRect(20, 60, 71, 20));
        COM1PowerEdit->setMaxLength(4);
        SetCOM1Button = new QPushButton(groupBox2);
        SetCOM1Button->setObjectName(QString::fromUtf8("SetCOM1Button"));
        SetCOM1Button->setGeometry(QRect(140, 60, 80, 21));
        COM1CurLcd = new QLCDNumber(groupBox2);
        COM1CurLcd->setObjectName(QString::fromUtf8("COM1CurLcd"));
        COM1CurLcd->setGeometry(QRect(330, 20, 64, 23));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(false);
        COM1CurLcd->setFont(font1);
        COM1CurLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM1CurLcd->setSmallDecimalPoint(false);
        COM1CurLcd->setDigitCount(5);
        COM1CurLcd->setSegmentStyle(QLCDNumber::Flat);
        COM1CurLcd->setProperty("value", QVariant(0.000000000000000));
        COM1TempLcd = new QLCDNumber(groupBox2);
        COM1TempLcd->setObjectName(QString::fromUtf8("COM1TempLcd"));
        COM1TempLcd->setGeometry(QRect(330, 100, 64, 23));
        COM1TempLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM1TempLcd->setSegmentStyle(QLCDNumber::Flat);
        COM1PowerLcd = new QLCDNumber(groupBox2);
        COM1PowerLcd->setObjectName(QString::fromUtf8("COM1PowerLcd"));
        COM1PowerLcd->setGeometry(QRect(330, 60, 64, 23));
        COM1PowerLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM1PowerLcd->setDigitCount(5);
        COM1PowerLcd->setSegmentStyle(QLCDNumber::Flat);
        label_11 = new QLabel(groupBox2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(270, 20, 54, 21));
        label_12 = new QLabel(groupBox2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(270, 100, 54, 21));
        label_13 = new QLabel(groupBox2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(270, 60, 81, 21));
        label_25 = new QLabel(groupBox2);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(100, 60, 41, 21));
        label_32 = new QLabel(groupBox2);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(400, 10, 51, 31));
        QFont font2;
        font2.setPointSize(14);
        label_32->setFont(font2);
        label_32->setScaledContents(false);
        label_36 = new QLabel(groupBox2);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(400, 90, 51, 41));
        label_36->setFont(font2);
        label_36->setScaledContents(false);
        label_37 = new QLabel(groupBox2);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(400, 60, 51, 21));
        label_37->setFont(font2);
        label_37->setScaledContents(false);
        COM1PowerEdit->raise();
        SetCOM1Button->raise();
        COM1CurLcd->raise();
        COM1TempLcd->raise();
        COM1PowerLcd->raise();
        COM1Status->raise();
        label_11->raise();
        label_12->raise();
        label_13->raise();
        label_25->raise();
        label_32->raise();
        label_36->raise();
        label_37->raise();
        groupBox3 = new QGroupBox(centralWidget);
        groupBox3->setObjectName(QString::fromUtf8("groupBox3"));
        groupBox3->setGeometry(QRect(10, 380, 581, 121));
        groupBox3->setMinimumSize(QSize(581, 81));
        groupBox3->setMaximumSize(QSize(581, 121));
        COM2Status = new QLabel(groupBox3);
        COM2Status->setObjectName(QString::fromUtf8("COM2Status"));
        COM2Status->setGeometry(QRect(220, 50, 31, 31));
        COM2Status->setPixmap(QPixmap(QString::fromUtf8(":/images/offline.png")));
        COM2Status->setScaledContents(true);
        COM2PowerEdit = new QLineEdit(groupBox3);
        COM2PowerEdit->setObjectName(QString::fromUtf8("COM2PowerEdit"));
        COM2PowerEdit->setGeometry(QRect(20, 60, 71, 20));
        COM2PowerEdit->setMaxLength(4);
        SetCOM2Button = new QPushButton(groupBox3);
        SetCOM2Button->setObjectName(QString::fromUtf8("SetCOM2Button"));
        SetCOM2Button->setGeometry(QRect(140, 60, 80, 24));
        COM227WCurLcd = new QLCDNumber(groupBox3);
        COM227WCurLcd->setObjectName(QString::fromUtf8("COM227WCurLcd"));
        COM227WCurLcd->setGeometry(QRect(330, 20, 64, 23));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setUnderline(false);
        COM227WCurLcd->setFont(font3);
        COM227WCurLcd->setContextMenuPolicy(Qt::DefaultContextMenu);
        COM227WCurLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM227WCurLcd->setSegmentStyle(QLCDNumber::Flat);
        COM2TempLcd = new QLCDNumber(groupBox3);
        COM2TempLcd->setObjectName(QString::fromUtf8("COM2TempLcd"));
        COM2TempLcd->setGeometry(QRect(330, 50, 64, 23));
        COM2TempLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM2TempLcd->setSegmentStyle(QLCDNumber::Flat);
        COM227WLDTempLcd = new QLCDNumber(groupBox3);
        COM227WLDTempLcd->setObjectName(QString::fromUtf8("COM227WLDTempLcd"));
        COM227WLDTempLcd->setGeometry(QRect(330, 90, 64, 23));
        COM227WLDTempLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM227WLDTempLcd->setSegmentStyle(QLCDNumber::Flat);
        label_16 = new QLabel(groupBox3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(263, 50, 71, 21));
        label_16->setFont(font);
        label_15 = new QLabel(groupBox3);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(240, 20, 91, 20));
        label_14 = new QLabel(groupBox3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(270, 90, 91, 16));
        label_26 = new QLabel(groupBox3);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(100, 60, 41, 21));
        label_33 = new QLabel(groupBox3);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(400, 10, 51, 41));
        label_33->setFont(font2);
        label_33->setScaledContents(false);
        label_38 = new QLabel(groupBox3);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(400, 40, 51, 41));
        label_38->setFont(font2);
        label_38->setScaledContents(false);
        label_39 = new QLabel(groupBox3);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(400, 80, 51, 41));
        label_39->setFont(font2);
        label_39->setScaledContents(false);
        COM2PowerEdit->raise();
        SetCOM2Button->raise();
        COM227WCurLcd->raise();
        COM2TempLcd->raise();
        COM227WLDTempLcd->raise();
        COM2Status->raise();
        label_16->raise();
        label_15->raise();
        label_14->raise();
        label_26->raise();
        label_33->raise();
        label_38->raise();
        label_39->raise();
        groupBox0 = new QGroupBox(centralWidget);
        groupBox0->setObjectName(QString::fromUtf8("groupBox0"));
        groupBox0->setGeometry(QRect(10, 0, 581, 91));
        groupBox0->setMinimumSize(QSize(581, 91));
        groupBox0->setMaximumSize(QSize(581, 91));
        groupBox0->setFlat(false);
        groupBox0->setCheckable(false);
        pushButton = new QPushButton(groupBox0);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(330, 20, 81, 21));
        pushButton->setCheckable(true);
        ComComboBox = new QComboBox(groupBox0);
        ComComboBox->setObjectName(QString::fromUtf8("ComComboBox"));
        ComComboBox->setGeometry(QRect(80, 60, 81, 21));
        ComComboBox->setMaxVisibleItems(256);
        label = new QLabel(groupBox0);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 40, 54, 16));
        label_2 = new QLabel(groupBox0);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(430, 40, 71, 16));
        statuslabel = new QLabel(groupBox0);
        statuslabel->setObjectName(QString::fromUtf8("statuslabel"));
        statuslabel->setGeometry(QRect(500, 30, 41, 41));
        statuslabel->setPixmap(QPixmap(QString::fromUtf8(":/images/off.png")));
        statuslabel->setScaledContents(true);
        GetcomButton = new QPushButton(groupBox0);
        GetcomButton->setObjectName(QString::fromUtf8("GetcomButton"));
        GetcomButton->setGeometry(QRect(80, 20, 81, 21));
        EnlaserButton = new QPushButton(groupBox0);
        EnlaserButton->setObjectName(QString::fromUtf8("EnlaserButton"));
        EnlaserButton->setGeometry(QRect(330, 60, 81, 21));
        QFont font4;
        font4.setPointSize(10);
        EnlaserButton->setFont(font4);
        EnlaserButton->setCheckable(true);
        EnlaserButton->setChecked(false);
        GetLDStatusBtn = new QPushButton(groupBox0);
        GetLDStatusBtn->setObjectName(QString::fromUtf8("GetLDStatusBtn"));
        GetLDStatusBtn->setGeometry(QRect(220, 20, 81, 21));
        GetLDStatusBtn->setCheckable(true);
        groupBox1 = new QGroupBox(centralWidget);
        groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
        groupBox1->setGeometry(QRect(10, 90, 581, 161));
        groupBox1->setMinimumSize(QSize(581, 161));
        groupBox1->setMaximumSize(QSize(581, 161));
        COM0CurLcd = new QLCDNumber(groupBox1);
        COM0CurLcd->setObjectName(QString::fromUtf8("COM0CurLcd"));
        COM0CurLcd->setGeometry(QRect(330, 20, 64, 23));
        COM0CurLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM0CurLcd->setSmallDecimalPoint(false);
        COM0CurLcd->setSegmentStyle(QLCDNumber::Flat);
        SetCOM0Button = new QPushButton(groupBox1);
        SetCOM0Button->setObjectName(QString::fromUtf8("SetCOM0Button"));
        SetCOM0Button->setGeometry(QRect(140, 70, 80, 21));
        COM0PowerEdit = new QLineEdit(groupBox1);
        COM0PowerEdit->setObjectName(QString::fromUtf8("COM0PowerEdit"));
        COM0PowerEdit->setGeometry(QRect(20, 70, 71, 20));
        COM0PowerEdit->setMaxLength(4);
        COM0PowerLcd = new QLCDNumber(groupBox1);
        COM0PowerLcd->setObjectName(QString::fromUtf8("COM0PowerLcd"));
        COM0PowerLcd->setGeometry(QRect(330, 70, 64, 23));
        COM0PowerLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM0PowerLcd->setSegmentStyle(QLCDNumber::Flat);
        COM0TempLcd = new QLCDNumber(groupBox1);
        COM0TempLcd->setObjectName(QString::fromUtf8("COM0TempLcd"));
        COM0TempLcd->setGeometry(QRect(330, 120, 64, 23));
        COM0TempLcd->setStyleSheet(QString::fromUtf8("QLCDNumber\n"
"{\n"
"background:black;\n"
"color: rgb(255, 255, 255);\n"
"}"));
        COM0TempLcd->setSegmentStyle(QLCDNumber::Flat);
        COM0Status = new QLabel(groupBox1);
        COM0Status->setObjectName(QString::fromUtf8("COM0Status"));
        COM0Status->setGeometry(QRect(230, 70, 31, 31));
        COM0Status->setPixmap(QPixmap(QString::fromUtf8(":/images/offline.png")));
        COM0Status->setScaledContents(true);
        label_6 = new QLabel(groupBox1);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(270, 120, 54, 21));
        label_8 = new QLabel(groupBox1);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(270, 70, 54, 21));
        label_9 = new QLabel(groupBox1);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(270, 20, 71, 21));
        label_24 = new QLabel(groupBox1);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(100, 70, 41, 21));
        label_24->setScaledContents(false);
        label_30 = new QLabel(groupBox1);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(400, 10, 51, 41));
        label_30->setFont(font2);
        label_30->setScaledContents(false);
        label_31 = new QLabel(groupBox1);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(400, 60, 51, 41));
        label_31->setFont(font2);
        label_31->setScaledContents(false);
        label_35 = new QLabel(groupBox1);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(400, 110, 51, 41));
        label_35->setFont(font2);
        label_35->setScaledContents(false);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 510, 581, 151));
        groupBox->setMinimumSize(QSize(581, 151));
        groupBox->setMaximumSize(QSize(581, 151));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 30, 54, 16));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(320, 30, 54, 16));
        SendtextEdit = new QTextEdit(groupBox);
        SendtextEdit->setObjectName(QString::fromUtf8("SendtextEdit"));
        SendtextEdit->setGeometry(QRect(10, 50, 241, 91));
        SendtextEdit->setMinimumSize(QSize(241, 91));
        SendtextEdit->setMaximumSize(QSize(241, 91));
        SendtextEdit->setReadOnly(true);
        ReceivetextEdit = new QTextEdit(groupBox);
        ReceivetextEdit->setObjectName(QString::fromUtf8("ReceivetextEdit"));
        ReceivetextEdit->setGeometry(QRect(320, 50, 241, 91));
        ReceivetextEdit->setMinimumSize(QSize(241, 91));
        ReceivetextEdit->setMaximumSize(QSize(241, 91));
        ReceivetextEdit->setReadOnly(true);
        QtWidgetsApplication1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtWidgetsApplication1Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 603, 22));
        QtWidgetsApplication1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtWidgetsApplication1Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtWidgetsApplication1Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtWidgetsApplication1Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtWidgetsApplication1Class->setStatusBar(statusBar);

        retranslateUi(QtWidgetsApplication1Class);
        QObject::connect(pushButton, &QPushButton::toggled, statuslabel, qOverload<>(&QLabel::update));
        QObject::connect(GetcomButton, &QPushButton::clicked, ComComboBox, qOverload<>(&QComboBox::update));
        QObject::connect(SetCOM0Button, &QPushButton::clicked, COM0PowerEdit, qOverload<>(&QLineEdit::copy));

        QMetaObject::connectSlotsByName(QtWidgetsApplication1Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtWidgetsApplication1Class)
    {
        QtWidgetsApplication1Class->setWindowTitle(QCoreApplication::translate("QtWidgetsApplication1Class", "UFO_Laser Control", nullptr));
        groupBox2->setTitle(QCoreApplication::translate("QtWidgetsApplication1Class", "Amp1", nullptr));
        COM1Status->setText(QString());
        SetCOM1Button->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\350\256\276\347\275\256\347\224\265\346\265\201", nullptr));
        label_11->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "LD\347\224\265\346\265\201\357\274\232", nullptr));
        label_12->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "LD\346\270\251\345\272\246\357\274\232", nullptr));
        label_13->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "LD\345\212\237\347\216\207\357\274\232", nullptr));
        label_25->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "mA", nullptr));
        label_32->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "mA", nullptr));
        label_36->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\302\260C", nullptr));
        label_37->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "mW", nullptr));
        groupBox3->setTitle(QCoreApplication::translate("QtWidgetsApplication1Class", "Amp2", nullptr));
        COM2Status->setText(QString());
        SetCOM2Button->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\350\256\276\347\275\256\347\224\265\346\265\201", nullptr));
        label_16->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\350\203\214\346\235\277\346\270\251\345\272\246\357\274\232", nullptr));
        label_15->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "27WLD\347\224\265\346\265\201\357\274\232", nullptr));
        label_14->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "LD\346\270\251\345\272\246\357\274\232", nullptr));
        label_26->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "A", nullptr));
        label_33->setText(QCoreApplication::translate("QtWidgetsApplication1Class", " A", nullptr));
        label_38->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\302\260C", nullptr));
        label_39->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\302\260C", nullptr));
#if QT_CONFIG(tooltip)
        groupBox0->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        groupBox0->setTitle(QCoreApplication::translate("QtWidgetsApplication1Class", "\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
        pushButton->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\345\274\200\345\220\257\351\200\232\350\256\257", nullptr));
        label->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "PC\347\253\257\345\217\243:", nullptr));
        label_2->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\351\200\232\350\256\257\347\212\266\346\200\201:", nullptr));
        GetcomButton->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\350\216\267\345\217\226\347\253\257\345\217\243", nullptr));
        EnlaserButton->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\344\275\277\350\203\275\346\277\200\345\205\211\345\231\250", nullptr));
        GetLDStatusBtn->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\350\216\267\345\217\226LD\347\212\266\346\200\201", nullptr));
        groupBox1->setTitle(QCoreApplication::translate("QtWidgetsApplication1Class", "Seed", nullptr));
        SetCOM0Button->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\350\256\276\347\275\256\347\224\265\346\265\201", nullptr));
        COM0PowerEdit->setText(QString());
        COM0Status->setText(QString());
        label_6->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "LD\346\270\251\345\272\246\357\274\232", nullptr));
        label_8->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "LD\345\212\237\347\216\207\357\274\232", nullptr));
        label_9->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "LD\347\224\265\346\265\201\357\274\232", nullptr));
        label_24->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "mA", nullptr));
        label_30->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "mA", nullptr));
        label_31->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "mW", nullptr));
        label_35->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\302\260C", nullptr));
        groupBox->setTitle(QCoreApplication::translate("QtWidgetsApplication1Class", "\346\224\266\345\217\221\346\225\260\346\215\256", nullptr));
        label_4->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\345\217\221\351\200\201\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("QtWidgetsApplication1Class", "\346\216\245\346\224\266:", nullptr));
        SendtextEdit->setDocumentTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsApplication1Class: public Ui_QtWidgetsApplication1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // QTWIDGETSAPPLICATION1JJBBNH_H
