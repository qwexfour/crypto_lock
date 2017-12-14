#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>
#include <iostream>
#include <stdlib.h>
#include <QDate>
#include <QTime>

#define PORT 8030
#define IP_SERVER "192.168.137.236"
#define AD_BTOOTH "A0:E9:DB:06:58:58"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QBluetoothServiceInfo* service = new QBluetoothServiceInfo();

   // QBluetoothServiceInfo remote = service;
    //QBluetoothServiceInfo & remoteService = service;

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
/*
void MainWindow::connect_Sock()
{
    sock->connectToHost("192.168.0.120", 5039);
        qDebug() << "Connect to host";
    if(!sock->waitForConnected(3000))
        qDebug() << "Error string";
}

void MainWindow::disconnect_Sock()
{
    sock->write("Action: Logoff\n");
    sock->write("\n");

    sock->close();
    qDebug() << "Socket closed!";
}

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
        emit messageReceived(btsocket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}
void MainWindow::readTCPSocket()
{
    if (!tcpsocket)
        return;

    while (tcpsocket->canReadLine()) {
        QByteArray line = tcpsocket->readLine();
        emit messageReceived(tcpsocket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}
//void MainWindow::ComeIn(const QBluetoothServiceInfo &remoteService)
void MainWindow::ComeIn()
{
    if (btsocket)
        return;

    // Connect to bluetoothservice - lock

    /*
    QBluetoothAddress address(AD_BTOOTH); //variant with address
    btsocket = new QBluetoothSocket(address); //variant with address
    */
    btsocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);//variant with service
    service = new QBluetoothServiceInfo();//variant with service
    service->setServiceName("HTC Desire Z A7272");//variant with service

    qDebug() << "Create socket";

    btsocket->connectToService(*service); //variant with service
    /*btsocket.connectToService(address, QBluetoothUuid(QBluetoothUuid::SerialPort)); //variant with address*/

    qDebug() << "ConnectToService done";

    QDate date;
    QTime time;
    QString date_time = date.toString("d.M.yy") + '#' + time.toString("hh:mm:ss");
    QString s = ui->lineEdit->text() + '#' + ui->lineEdit_2->text() + '#' + ui->lineEdit_3->text() + '#' + date_time;
    QByteArray data;
    data.append(s); // or  QByteArray data = s.toUtf8() + '\n';
    btsocket->write(data);
    btsocket->write("\n");
    qDebug() << "Send data to lock";

    connect(btsocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(btsocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(btsocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
}

void MainWindow::SignUp()
{   if (tcpsocket)
        return;

    // Connect to server
    tcpsocket = new QTcpSocket(this);
    qDebug() << "Create socket";
    tcpsocket->connectToHost(IP_SERVER, PORT);
    if (!tcpsocket->waitForConnected(5000))
        qDebug()<<"Error";
    else qDebug() << "ConnectToServer done";
    QDate date;
    QTime time;
    QString date_time = date.toString("d.M.yy") + '#' + time.toString("hh:mm:ss");
    QString s = ui->lineEdit->text() + '#' + ui->lineEdit_2->text() + '#' + ui->lineEdit_3->text();
    QByteArray data;
    data.append(s); // or  QByteArray data = s.toUtf8() + '\n';
    tcpsocket->write(data);
    tcpsocket->write("\n");
    qDebug() << "Send data to server";

    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(tcpsocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpsocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

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
    //ui->textEdit->setText(s);
    //ui->label->setText(s);
}
