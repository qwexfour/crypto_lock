#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void myexit();
    void SignUp();
    //void ComeIn(const QBluetoothServiceInfo &remoteService);
    void ComeIn();
    void delete_account();

signals:
    void messageReceived(const QString &sender, const QString &message);
    //void disconnected();

private slots:
    void readTCPSocket1();
    void readTCPSocket();
    void connectedTCP();
    void connectedTCP1();
    void disconnected();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpsocket;
    //QBluetoothSocket *btsocket;
    //QBluetoothServiceInfo* service;
    QTcpSocket *tcpsocket1;
};

#endif // MAINWINDOW_H
