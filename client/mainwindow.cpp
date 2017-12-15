#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>
#include <QMessageBox>
#include <iostream>
#include <stdlib.h>
#include <QDate>
#include <QTime>

#define PORT 8030
#define IP_SERVER "192.168.137.17"
#define AD_BTOOTH "A0:E9:DB:06:58:58"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpsocket(nullptr)
    , btsocket(nullptr)
{
    ui->setupUi(this);
    //QBluetoothServiceInfo* service = new QBluetoothServiceInfo();

   // QBluetoothServiceInfo remote = service;
    //QBluetoothServiceInfo & remoteService = service;
    qDebug() << "Init";
    QMessageBox::information(NULL,QObject::tr("MainWindow ctor"),tr("Не все так плохо..."));

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
    //tcpsocket->connectToHost("192.168.31.140", 5039);
    //    qDebug() << "Connect to host";

    /*if(!tcpsocket->waitForConnected(3000))
        qDebug() << "Error string";*/
    QMessageBox::information(NULL,QObject::tr("connectedTCP"), tr("Connected to TCP"));
    QString s = "r#"
            + ui->lineEdit->text() + '#'
            + ui->lineEdit_2->text() + '#'
            + ui->lineEdit_3->text() + "#key1#key2";
    QByteArray data = s.toUtf8() + '\n';
    QMessageBox::information(NULL,QObject::tr("connectedTCP"), s);
    tcpsocket->write(data);
}

void MainWindow::connectedBT()
{
    QMessageBox::information(NULL,QObject::tr("connectedBT"),tr("Connected to BT"));

    QDate date;
    QTime time;

    QString date_time = date.toString("d.M.yy") + '#' + time.toString("hh:mm:ss");
    QString s = ui->lineEdit->text() + '#' + ui->lineEdit_2->text() + '#' + ui->lineEdit_3->text() + '#' + date_time;
    QByteArray data = s.toUtf8() + "\n";
    // data.append(s); // or  QByteArray data = s.toUtf8() + '\n';

    QMessageBox::information(NULL,QObject::tr("connectedBT"),"sending: " + s);
    qDebug() << "Send data to lock";

    btsocket->write(data);
}

void MainWindow::disconnected()
{
//    tcpsocket->write("Action: Logoff\n");
//    tcpsocket->write("\n");

//    tcpsocket->close();
    qDebug() << "Socket closed!";
}
/*
void MainWindow::login()
{
    sock->write("Action: Login\nUsername: abills_admin\nSecret: Password\n");
    sock->write("\n");
}

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
void MainWindow::readBTSocket()
{
    if (!btsocket)
        return;

    while (btsocket->canReadLine()) {
        QByteArray line = btsocket->readLine();
        QMessageBox::information(this, QString("readBTSocket"), QString(line));
        emit messageReceived(btsocket->peerName(),
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
    }
}
//void MainWindow::ComeIn(const QBluetoothServiceInfo &remoteService)
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
    //btsocket->connectToService(*service); //variant with service
    /*btsocket.connectToService(address, QBluetoothUuid(QBluetoothUuid::SerialPort)); //variant with address*/

    qDebug() << "ConnectToService done";


}

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

    tcpsocket->connectToHost(IP_SERVER, PORT);
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
