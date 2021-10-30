#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"send_window.h"
#include"receive_window.h"
#include <winsock.h>
#include"CBase64.h"
#include<iostream>
#include<QDebug>
#include <QPushButton>
#include<QMessageBox>
#include<QLineEdit>
#include <iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void login();  // to log in
    bool login_success(char *bufferresv); // check Authentication
    sockaddr_in sin;
    WSADATA wsadata;
    char bufferresv[10240]; 
    //buffer space
    //change it to a proper size

private:
    Ui::MainWindow *ui;
    Send_window *SW = new Send_window();
    receive_window *RW = new receive_window();
    void to_login();
    void to_RW();
    void to_SW();
};
#endif // MAINWINDOW_H
