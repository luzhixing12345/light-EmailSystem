#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_send_window.h"
#include "ui_receive_window.h"

using namespace std;
#define WSWENS MAKEWORD(2,0)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(WSAStartup(WSWENS, &wsadata) !=0){
        QMessageBox::critical(this,"错误","初始化失败");
        return;
    }

    connect(ui->pushButton_exit,&QPushButton::clicked,this,&MainWindow::close);
    connect(ui->pushButton_login,&QPushButton::clicked,this,&MainWindow::login);

    connect(SW->ui->logout,&QPushButton::clicked,this,&MainWindow::to_login);
    connect(SW->ui->resv_letter,&QPushButton::clicked,this,&MainWindow::to_RW);

    connect(RW->ui->logout,&QPushButton::clicked,this,&MainWindow::to_login);
    connect(RW->ui->write_letter,&QPushButton::clicked,this,&MainWindow::to_SW);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login(){

    SOCKET s = socket(PF_INET,SOCK_STREAM,0);
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(25); //smtp端口号
    string email_address = ui->email_address->text().toStdString();
    string password = ui->password->text().toStdString();

    RW ->email_address = email_address;
    RW ->password = password;

    int pos=-1;
    for(int i=0;i<int(email_address.size());i++){
        if(email_address[i]=='@'){
            pos=i;
            break;
        }
    }
    if(pos==-1){QMessageBox::critical(this,"错误","邮箱地址无效");return;};
    string user_name = email_address.substr(0,pos);
    string email_type = "smtp."+email_address.substr(pos+1);
    RW ->email_type = "pop3."+email_address.substr(pos+1);

    qDebug("readString = %s",qPrintable(QString::fromStdString(email_type)));
    hostent* hptr = gethostbyname(email_type.c_str());
    memcpy(&sin.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
    printf("IP of smtp_address is : %d:%d:%d:%d",
        sin.sin_addr.S_un.S_un_b.s_b1,
        sin.sin_addr.S_un.S_un_b.s_b2,
        sin.sin_addr.S_un.S_un_b.s_b3,
        sin.sin_addr.S_un.S_un_b.s_b4);

    cout << endl;
    if(::connect(s, (sockaddr*)&sin, sizeof(sin)))
    {
        QMessageBox::critical(this,"错误","邮箱地址无效");
        return;
    }
    else{
        cout<<"connect succeed!";
    }
    int len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;
    // send "helo ehlo"
    char bufferHello[] = "helo ehlo\r\n";
    cout << "helo ehlo..." << endl;
    send(s, bufferHello, strlen(bufferHello), 0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;
    // send "auth login"
    char bufferLogin[] = "auth login\r\n";
    cout << "auth login..." << endl;
    send(s, bufferLogin, strlen(bufferLogin),0);
    len = recv(s, bufferresv, 10240, 0);

    bufferresv[len]=0;
    printf("%s",bufferresv);

    cout << endl;
    // send "username" send "psw"
    CBase64 *encoder = new CBase64();
    string trans_base64_username="";
    string trans_base64_password="";
    bool key1 = encoder->Encode(user_name.c_str(),user_name.size(),trans_base64_username);
    bool key2 = encoder->Encode(password.c_str(),password.size(),trans_base64_password);
    if(!key1|!key2){
        QMessageBox::critical(this,"错误","邮箱地址无效");
        return;
    }
//    char bufferUserName[trans_base64_username.size()+4];
//    char bufferpsw[trans_base64_password.size()+4];
//    strcpy(bufferUserName,(trans_base64_username+"\r\n").c_str());
//    strcpy(bufferpsw,(trans_base64_password+"\r\n").c_str());

    cout << "input username...." << endl;

    trans_base64_username+="\r\n";
    trans_base64_password+="\r\n";

//    char bufferUserName[trans_base64_username.size()];
//    char bufferpwd[trans_base64_password.size()];
//    strcpy(bufferUserName,trans_base64_username.c_str());
//    strcpy(bufferpwd,trans_base64_password.c_str());
    send(s,trans_base64_username.c_str(),trans_base64_username.size(),0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;
    cout << "input password..." << endl;
    send(s,trans_base64_password.c_str(),trans_base64_password.size(),0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    if(login_success(bufferresv)){
        SW->email_address = email_address;
        SW->s =  s;
        to_SW();
    }
    else return ;



}

bool MainWindow::login_success(char * bufferresv){
    string s = bufferresv;
    if(s=="235 Authentication successful\r\n")return true;
    else return false;
}


void MainWindow::to_SW(){
    SW->setVisible(true);
    RW->hide();
    this->hide();
}

void MainWindow::to_login(){
    this->setVisible(true);
    SW->hide();
    RW->hide();
}

void MainWindow::to_RW(){
    RW->login();
    RW->setVisible(true);
    this->hide();
    SW->hide();
}

