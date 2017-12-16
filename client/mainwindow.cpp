#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>
#include <QMessageBox>
//#include <iostream>
//#include <сstdlib>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QFile>
#include "lib/consts.h"
#include "lib/msglib.h"
#include "lib/RSA.h"

#define PORT_SERVER 8030
#define PORT_LOCK 8031
#define IP_SERVER "192.168.137.17"
#define IP_LOCK "192.168.137.31"
//#define AD_BTOOTH "A0:E9:DB:06:58:58"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpsocket(nullptr)
    , tcpsocket1(nullptr)
{
    ui->setupUi(this);
    //QBluetoothServiceInfo* service = new QBluetoothServiceInfo();

   // QBluetoothServiceInfo remote = service;
    //QBluetoothServiceInfo & remoteService = service;
    qDebug() << "Init";
    QMessageBox::information(NULL,QObject::tr("MainWindow ctor"),tr("Здравствуйте!"));

    //connect(ui->actionExit, &QAction::triggered, this, &MainWindow::myexit); //exit to myexit
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::SignUp);//push button to SignUp
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::ComeIn);//push button to SignIn
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::myexit);//push button to myexit
    //delete service;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectedTCP()
{
    ui->pushButton->setEnabled(false);
    //tcpsocket->connectToHost("192.168.31.140", 5039);
    //    qDebug() << "Connect to host";

    /*if(!tcpsocket->waitForConnected(3000))
        qDebug() << "Error string";*/
    QMessageBox::information(NULL,QObject::tr("connectedTCP"), tr("Connected to TCP"));
    QFile file("Keys.txt");
    if(file.exists())
    {
        QMessageBox::information(NULL,QObject::tr("Registration"), tr("You are already registered!"));
        return;
    }
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(NULL,QObject::tr("Registration"), tr("Error!"));
        return;
    }
    number open_key_e1[LENGTH_2BYTES];
    number open_key_n1[LENGTH_2BYTES];
    number close_key_d1[LENGTH_2BYTES];
    GenKeys(open_key_e1, open_key_n1, close_key_d1);
    char* key_e;
    char* key_n;
    char* key_d;
    NumberToStr(open_key_e1, key_e);
    NumberToStr(open_key_n1, key_n);
    NumberToStr(close_key_d1, key_d);

    QTextStream out(&file);
    out << ui->lineEdit->text() << "\n" << ui->lineEdit_2->text() << "\n" << ui->lineEdit_3->text() << "\n" << key_e << "\n" << key_n << "\n" << key_d << "\n";

    int *length;
    char* regmsg = makeRegMsg(ui->lineEdit->text().toLocal8Bit().constData(),
                              ui->lineEdit_2->text().toLocal8Bit().constData(),
                              ui->lineEdit_3->text().toLocal8Bit().constData(), key_e, key_n, length);

    /*QString s = "r#"
            + ui->lineEdit->text() + '#'
            + ui->lineEdit_2->text() + '#'
            + ui->lineEdit_3->text() + "#key1#key2";
    QByteArray data = s.toUtf8() + '\n';*/
    QString s(regmsg);
    QByteArray data = s.toUtf8();
    QMessageBox::information(NULL,QObject::tr("connectedTCP"), s);
    tcpsocket->write(data);
}

void MainWindow::connectedTCP1()
{
    ui->pushButton_2->setEnabled(false);
    QMessageBox::information(NULL,QObject::tr("connectedTCP1"),tr("Connected to TCP1"));
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    QString date_time = date.toString("d M yy") + ' ' + time.toString("hh mm ss");
    //QString s = ui->lineEdit->text().toLocal8Bit().constData() + '#' + ui->lineEdit_2->text() + '#' + ui->lineEdit_3->text() + '#' + date_time;
    char* str_msg = makeOpenText(ui->lineEdit->text().toLocal8Bit().constData(),
                             ui->lineEdit_2->text().toLocal8Bit().constData(),
                             ui->lineEdit_3->text().toLocal8Bit().constData(),
                             date_time.toLocal8Bit().constData(), NULL);
    QFile file("Keys.txt");
    if(!file.exists())
    {
        QMessageBox::information(NULL,QObject::tr("Authorisation"), tr("Your account is lost!"));
        return;
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL,QObject::tr("Authorisation"), tr("Error!"));
        return;
    }
    QTextStream in(&file);
    QString surname = in.readLine();
    QString name = in.readLine();
    QString patronymic = in.readLine();
    QString open_key_e = in.readLine();
    QString open_key_n = in.readLine();
    QString close_key_d = in.readLine();
    char* key_d = close_key_d.toLocal8Bit().data();
    char* key_n = open_key_n.toLocal8Bit().data();
    number num_key_d[LENGTH_2BYTES];
    number num_key_n[LENGTH_2BYTES];
    StrToNumber(key_d, num_key_d);
    StrToNumber(key_n, num_key_n);

    number res[LENGTH_2BYTES];
    number result[LENGTH_2BYTES];
    HashFunction(str_msg, res);
    SignatureRSA(res, num_key_d, num_key_n, result);
    char* sign_message;
    NumberToStr(result, sign_message);

    char* message = makeOpenMsg(ui->lineEdit->text().toLocal8Bit().constData(),
                                ui->lineEdit_2->text().toLocal8Bit().constData(),
                                ui->lineEdit_3->text().toLocal8Bit().constData(),
                                date_time.toLocal8Bit().constData(), sign_message, NULL);

    QString s(message);
    QByteArray data = s.toUtf8();
    // data.append(s); // or  QByteArray data = s.toUtf8() + '\n';

    QMessageBox::information(NULL,QObject::tr("connectedTCP1"),"sending: " + s);
    qDebug() << "Send data to lock";

    tcpsocket1->write(data);
}

void MainWindow::disconnected()
{
//    tcpsocket->write("Action: Logoff\n");
//    tcpsocket->write("\n");

//    tcpsocket->close();
    qDebug() << "Socket closed!";
}
/*
void MainWindow::read_Data()
{
    qDebug() << "NEW DATA";
    QByteArray data;
    QString data_str;
    data.append(sock->readAll());
    data_str.append(data);
}
*/
void MainWindow::myexit()
{
    exit(0);
}
void MainWindow::readTCPSocket1()
{
    if (!tcpsocket1)
        return;

    while (tcpsocket1->canReadLine()) {
        QByteArray line = tcpsocket1->readLine();
        QMessageBox::information(this, QString("readTCPSocket1"), QString(line));
        emit messageReceived(tcpsocket1->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}
void MainWindow::readTCPSocket()
{
    if (!tcpsocket)
        return;

    qDebug() << "got to reading tcp answer";
    while (tcpsocket->canReadLine()) {
        QByteArray line = tcpsocket->readLine();
        QMessageBox::information(this, QString("readTCPSocket"), QString(line));
        emit messageReceived(tcpsocket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
        /*if (line != "Ok") {
            MainWindow MainWindow1 = this->MainWindow();
            MainWindow1().show();
        }*/
    }

}
//void MainWindow::ComeIn(const QBluetoothServiceInfo &remoteService)
/*
void MainWindow::ComeIn()
{
    if (!btsocket) {

        btsocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);//variant with service
    //    service = new QBluetoothServiceInfo();//variant with service
    //    service->setServiceName("HTC Desire Z A7272");//variant with service

        QBluetoothSocket *sock = btsocket;
        connect(btsocket, SIGNAL(readyRead()), this, SLOT(readBTSocket()));
        connect(btsocket, &QBluetoothSocket::connected, this, &MainWindow::connectedBT);
        connect(btsocket, &QBluetoothSocket::disconnected, this, &MainWindow::disconnected);
    //    connect(btsocket, &QAbstractSocket::error, this, [sock, this](QBluetoothSocket::SocketError) {
    //        QMessageBox::information(this, "ERROR", "Something went wrong!");
    //    });
        connect(btsocket, &QBluetoothSocket::stateChanged, this, [sock, this](QBluetoothSocket::SocketState){
        QMessageBox::information(this, "STATE CHANGE", "state changed");
        });
        qDebug() << "Create socket";
    } else {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("BT connection closed to be reoppended"));
        btsocket->close();
    }
    QMessageBox::information(NULL,QObject::tr("Информация"),tr("connectToService called"));
    btsocket->connectToService(QBluetoothAddress(AD_BTOOTH), 1);
    //btsocket->connectToService(*service); //variant with service*/
    /*btsocket.connectToService(address, QBluetoothUuid(QBluetoothUuid::SerialPort)); //variant with address*/
/*
    qDebug() << "ConnectToService done";


}*/

    
void MainWindow::SignUp()
{   if (tcpsocket)
        return;

    // Connect to server
    tcpsocket = new QTcpSocket(this);
    qDebug() << "Create socket";

    QMessageBox::information(NULL,QObject::tr("Информация"),tr("SignUp"));
    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readTCPSocket()));
    connect(tcpsocket, &QTcpSocket::connected, this, &MainWindow::connectedTCP);
    connect(tcpsocket, &QTcpSocket::disconnected, this, &MainWindow::disconnected);

    tcpsocket->connectToHost(IP_SERVER, PORT_SERVER);
//    if (!tcpsocket->waitForConnected(5000))
//        qDebug()<<"Error";
//    else
//        qDebug() << "ConnectToServer done";

    /*
     tcpsocket->waitForReadyRead(3000);
     QString ok;
     QByteArray okay;
     QTime time;
     okay.append(tcpsocket->readAll());
     ok.append(okay);
     QMessageBox::information(NULL,QObject::tr("Информация"),tr("Не все так плохо..."));

        QDate date;
        //QTime time;
        QString date_time = date.toString("d.M.yy") + '#' + time.toString("hh:mm:ss");
        QByteArray data;
        data.append(s); // or  QByteArray data = s.toUtf8() + '\n';
            //QByteArray arrBlock;
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_1);
            out << quint16(0) << QTime::currentTime() << data;
            out.device()->seek(0);
            out << quint16(data.size() - sizeof(quint16));

            tcpsocket->flush();


        //ui->browser_dbg->setText(time.currentTime().toString() + " " + ok);


    //QDate date;
    //QTime time;
   // QString date_time = date.toString("d.M.yy") + '#' + time.toString("hh:mm:ss");
   // QString s = ui->lineEdit->text() + '#' + ui->lineEdit_2->text() + '#' + ui->lineEdit_3->text();
    //QByteArray data;
    data.append(s); // or  QByteArray data = s.toUtf8() + '\n';
    tcpsocket->write(data);
    tcpsocket->write("\n");
    qDebug() << "Send data to server";*/
//     tcpsocket->write("r#aaa#bbbb#cccc#dddd#eeee\n");
//        tcpsocket->write("\n");
    // tcpsocket->waitForBytesWritten(3000);
    /*msocket = new QTcpSocket(this);
        msocket->connectToHost("192.168.1.2", 80);

        if (!msocket->waitForConnected(5000))
            qDebug()<<"Error";

        msocket->write("client:1:md5:key:");

        connect(msocket, SIGNAL(readyRead()), this, SLOT(on_recievedData()));*/

/*
    sock = new QTcpSocket(this);

    connect(ui->btn_connect, SIGNAL(clicked()), this, SLOT(connect_Sock()));
    connect(ui->btn_disconnect, SIGNAL(clicked()), this, SLOT(disconnect_Sock()));
    connect(ui->btn_login, SIGNAL(clicked()), this, SLOT(login()));
    connect(sock, SIGNAL(readyRead()), this, SLOT(read_Data()));
*/
//    ui->textEdit->setText(s);
//    ui->label->setText(s);
}

void MainWindow::ComeIn()
{   if (tcpsocket1)
        return;

    // Connect to server
    tcpsocket1 = new QTcpSocket(this);
    qDebug() << "Create socket";

    QMessageBox::information(NULL,QObject::tr("Информация"),tr("ComeIn"));
    connect(tcpsocket1, SIGNAL(readyRead()), this, SLOT(readTCPSocket1()));
    connect(tcpsocket1, &QTcpSocket::connected, this, &MainWindow::connectedTCP1);
    connect(tcpsocket1, &QTcpSocket::disconnected, this, &MainWindow::disconnected);

    tcpsocket->connectToHost(IP_LOCK, PORT_LOCK);
/*
     QMessageBox::information(NULL,QObject::tr("Информация"),tr("Не все так плохо..."));

        QDate date;
        //QTime time;
        QString date_time = date.toString("d.M.yy") + '#' + time.toString("hh:mm:ss");
        QByteArray data;
        data.append(s); // or  QByteArray data = s.toUtf8() + '\n';


    //QDate date;
    //QTime time;
   // QString date_time = date.toString("d.M.yy") + '#' + time.toString("hh:mm:ss");
   // QString s = ui->lineEdit->text() + '#' + ui->lineEdit_2->text() + '#' + ui->lineEdit_3->text();
    //QByteArray data;
    data.append(s); // or  QByteArray data = s.toUtf8() + '\n';
    tcpsocket->write(data);
    tcpsocket->write("\n");
    qDebug() << "Send data to server";*/
//     tcpsocket->write("r#aaa#bbbb#cccc#dddd#eeee\n");
//        tcpsocket->write("\n");

}
