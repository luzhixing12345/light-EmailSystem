#include "send_window.h"
#include "ui_send_window.h"

Send_window::Send_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Send_window)
{
    ui->setupUi(this);

    connect(ui->send,&QPushButton::clicked,this,&Send_window::send_email);
    //connect(ui->resv_letter,&QPushButton::clicked,this,&Send_window::to_RW);


}

Send_window::~Send_window()
{
    delete ui;
}

void Send_window::send_email(){

    resv_address = ui->receiver->text().toStdString();
    subject = ui->subject->text().toStdString();
    context_body = ui->context_body->toPlainText().toStdString();
    string from_add = "mail from:<"+email_address+">\r\n";

    cout << "mail from:<"+email_address+">..." << endl;
    send(s, from_add.c_str(), from_add.size(), 0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;

    string to_add = "rcpt to:<"+resv_address+">\r\n";
    send(s, to_add.c_str(), to_add.size(), 0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    cout << "rcpt to:<"+resv_address+">..." << endl;
    printf("%s",bufferresv);
    cout << endl;

    // data
    char bufferData[] = "data\r\n";
    cout << "data..." << endl;
    send(s, bufferData,strlen(bufferData), 0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;
    //
    to_add = "To:"+resv_address+"\r\n";
    from_add = "From: <"+email_address+">\r\n";
    cout << "to:1269185526@qq.com... " << endl;
    send(s, to_add.c_str(),to_add.size(),0);
    cout << "from:luzhixing12345@163.com..." << endl;
    send(s, from_add.c_str(), from_add.size(), 0);

    subject = "Subject: "+subject+"\r\n";
    cout << "subject: hello from kaho..." << endl;
    send(s, subject.c_str(), subject.size(), 0);

    char bufferversion[] = "MIME-Version: 1.0\r\n";
    send(s, bufferversion, strlen(bufferversion), 0);

    char buffertype []= "Content-Type: multipart/mixed;boundary=\"V0hVLWx6eA==\"\r\n\r\n";
    send(s, buffertype, strlen(buffertype), 0);

    CBase64 *encoder = new CBase64();
    string base_64_context_body="";
    string head= "--V0hVLWx6eA==\r\nContent-Type: text/plain;charset=\"gb2312\"\r\nContent-Transfer-Encoding: base64\r\n\r\n";
    bool key = encoder->Encode(context_body.c_str(),context_body.size(),base_64_context_body);

    if(!key){
        QMessageBox::critical(this,"错误","转码失败");
        return;
    }
    base_64_context_body=head+base_64_context_body+"\r\n";

    cout << "--V0hVLWx6eA==" << endl;
    send(s,  base_64_context_body.c_str(),  base_64_context_body.size(), 0);

    char bufferPoint[] = ".\r\n";
    send(s, bufferPoint, strlen(bufferPoint), 0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;
    string mail_rpy = bufferresv;
    if(mail_rpy.substr(0,3)=="250"){
        QMessageBox::information(NULL,"","发送成功 ！");
    }
}


