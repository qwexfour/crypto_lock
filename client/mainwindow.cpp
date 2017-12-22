#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>
#include <QMessageBox>
//#include <iostream>
#include <stdlib.h>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QFile>
#include <string.h>
extern "C" {
#include "lib/consts.h"
#include "lib/msglib.h"
#include "lib/RSA.h"
}

//#define PORT_SERVER 8030
//#define PORT_LOCK 8031
//#define IP_SERVER "192.168.137.17"
//#define IP_LOCK "192.168.137.31"
//#define AD_BTOOTH "A0:E9:DB:06:58:58"
#define BUFF_SIZE 255

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
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::delete_account);
    //delete service;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::delete_account()
{
    QFile("Keys.txt").remove();
    QMessageBox::information(NULL,QObject::tr("Delete account"), tr("Account is removed"));
}

void MainWindow::connectedTCP()
{
    //ui->pushButton->setEnabled(false);
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
    //QMessageBox::information(NULL,QObject::tr("Registration"), tr("Start generation"));
    srand(time(NULL));
    GenKeys(open_key_e1, open_key_n1, close_key_d1);


    char key_e[BUFF_SIZE];// = "1579 5005 5a33 8c4f 3773 9757 0dc0 d31f 972b cba0 906f 7aff a74e dc10 a729 b50b";
    char key_n[BUFF_SIZE];// = "20f6 db30 dbea 9878 211b aa05 6c93 3124 d697 942a 0a0c 27fc 5158 4ac9 342c bf3b";
    char key_d[BUFF_SIZE];// = "0917 909b 26cf bb75 cdbb d2be 71cd eb78 4bbc 9435 8d2d 54a8 88ae d684 18de bf03";

    //QMessageBox::information(NULL,QObject::tr("Registration"), tr("Finished generation"));

    NumberToStr(open_key_e1, key_e);
    NumberToStr(open_key_n1, key_n);
    NumberToStr(close_key_d1, key_d);


    int length;
    char* regmsg = makeRegMsg(ui->lineEdit->text().toLocal8Bit().constData(),
                              ui->lineEdit_2->text().toLocal8Bit().constData(),
                              ui->lineEdit_3->text().toLocal8Bit().constData(), key_e, key_n, &length);

    /*QString s = "r#"
            + ui->lineEdit->text() + '#'
            + ui->lineEdit_2->text() + '#'
            + ui->lineEdit_3->text() + "#key1#key2";
    QByteArray data = s.toUtf8() + '\n';*/
    QString s (regmsg);
    QByteArray data = s.toUtf8();
    /*QMessageBox::information(NULL,QObject::tr("connectedTCP"), s);*/
    tcpsocket->write(data);

    QTextStream out(&file);
    out << ui->lineEdit->text() << "\n"
        << ui->lineEdit_2->text() << "\n"
        << ui->lineEdit_3->text() << "\n"
        << key_e << "\n" << key_n << "\n"
        << key_d << "\n";
    file.close();

}

void MainWindow::connectedTCP1()
{
    //ui->pushButton_2->setEnabled(false);
    QMessageBox::information(NULL,QObject::tr("connectedTCP1 (Lock)"),tr("Connected to TCP1 (Lock)"));
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
    char key_d[BUFF_SIZE];
    char key_n[BUFF_SIZE];
    strcpy(key_d, close_key_d.toLocal8Bit().constData());
    strcpy(key_n, open_key_n.toLocal8Bit().constData());
    number num_key_d[LENGTH_2BYTES];
    number num_key_n[LENGTH_2BYTES];
    StrToNumber(key_d, num_key_d);
    StrToNumber(key_n, num_key_n);

    number res[LENGTH_2BYTES];
    number result[LENGTH_2BYTES];
    /*int length_of_str = strlen(str_msg);
    QString makeopenstr(str_msg);
    QMessageBox::information(NULL,QObject::tr("makeOpenText"), makeopenstr);*/
    HashFunction(str_msg, res);
    //QString str_key_d(key_d);
    //QMessageBox::information(NULL,QObject::tr("Key_d"), str_key_d);


    /*char s2[BUFF_SIZE];
    NumberToStr(res,s2);
    char tmp_str[BUFF_SIZE];
    sprintf( tmp_str, "%d", length_of_str);
    QString length_str(tmp_str);
    QMessageBox::information(NULL,QObject::tr("Hashfunc"), "length of str_msg " + length_str + " " + s2);*/


    SignatureRSA(res, num_key_d, num_key_n, result);
    char sign_message[BUFF_SIZE];
    NumberToStr(result, sign_message);

    char* message = makeOpenMsg(ui->lineEdit->text().toLocal8Bit().constData(),
                                ui->lineEdit_2->text().toLocal8Bit().constData(),
                                ui->lineEdit_3->text().toLocal8Bit().constData(),
                                date_time.toLocal8Bit().constData(), sign_message, NULL);

    QString s(message);
    QByteArray data = s.toUtf8();
    // data.append(s); // or  QByteArray data = s.toUtf8() + '\n';

    /*QMessageBox::information(NULL,QObject::tr("connectedTCP1"),"sending: " + s);*/
    qDebug() << "Send data to lock";

    tcpsocket1->write(data);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    file.close();
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
    QMessageBox::information(NULL,QObject::tr("Exit"), tr("Goodbye!"));
    close();
    //exit(0);
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
{

    if (tcpsocket)
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Socket is created and press Delete account and Exit to SignUp again!"));
        return;
    }

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
    //tcpsocket->close();
    //delete tcpsocket;
}

void MainWindow::ComeIn()
{
    if (tcpsocket1)
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Socket1 is created! Press Exit to SignIn again!"));
        return;
    }
    // Connect to server
    tcpsocket1 = new QTcpSocket(this);
    qDebug() << "Create socket1";

    QMessageBox::information(NULL,QObject::tr("Информация"),tr("ComeIn"));
    connect(tcpsocket1, SIGNAL(readyRead()), this, SLOT(readTCPSocket1()));
    connect(tcpsocket1, &QTcpSocket::connected, this, &MainWindow::connectedTCP1);
    connect(tcpsocket1, &QTcpSocket::disconnected, this, &MainWindow::disconnected);

//    using err_t = void (QAbstractSocket::*)(QAbstractSocket::SocketState);
//    connect(tcpsocket1, &QTcpSocket::error, static_cast<err_t>(&QAbstractSocket::error), [=](QAbstractSocket::SocketError err) {
//        QMessageBox::information(NULL,QObject::tr("Error"), "Error: " + int(err));
//    });
    connect(tcpsocket1, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
          [=](QAbstractSocket::SocketError err){
                QMessageBox::information(NULL,QObject::tr("Error"), "Error: " + tcpsocket1->errorString());
    });


    tcpsocket1->connectToHost(IP_LOCK, PORT_LOCK);
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
    //tcpsocket1->close();
    //delete tcpsocket1;
}
