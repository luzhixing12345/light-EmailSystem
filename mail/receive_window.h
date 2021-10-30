#ifndef RECEIVE_WINDOW_H
#define RECEIVE_WINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include <winsock.h>
#include<iostream>
#include<string>
#include<stdio.h>
#include<string.h>
#include<QDebug>
#include<QListWidget>
#include<QString>
#include<CBase64.h>
using namespace std;
namespace Ui {
class receive_window;
}

class receive_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit receive_window(QWidget *parent = nullptr);
    ~receive_window();
    Ui::receive_window *ui;
    void login();
    int find_message_num(char *bufferresv);

    string email_address;
    string email_type;
    string password;
    void load_listwidget(int n,SOCKET s);
    vector<string> find_name(string s);
    void change_read();
    void back();
    void delete_mail();
    string find_body(string s);

private:
    SOCKET s;
    sockaddr_in sin;
    WSADATA wsadata;
    char bufferresv[10240];
    int len;
};

#endif // RECEIVE_WINDOW_H
