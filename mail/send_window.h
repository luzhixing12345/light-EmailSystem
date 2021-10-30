#ifndef SEND_WINDOW_H
#define SEND_WINDOW_H

#include <QMainWindow>
#include <winsock.h>
#include<iostream>
#include<string>
#include<stdio.h>
#include<string.h>
#include<CBase64.h>
#include<QMessageBox>
using namespace std;
namespace Ui {
class Send_window;
}

class Send_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Send_window(QWidget *parent = nullptr);
    ~Send_window();
    void send_email();
    void to_login_window();
    Ui::Send_window *ui;
    string email_address;
    SOCKET s;

private:

    char bufferresv[10240];
    int len;
    string resv_address;
    string subject;
    string context_body;

};

#endif // SEND_WINDOW_H
