#include "receive_window.h"
#include "ui_receive_window.h"

receive_window::receive_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::receive_window)
{
    ui->setupUi(this);
    ui->Return->setVisible(false);
    ui->text_body->setVisible(false);
    ui->sender->setVisible(false);
    ui->subject->setVisible(false);
    ui->detele->setVisible(false);
    connect(ui->mail_list,&QListWidget::itemClicked,this,&receive_window::change_read);
    connect(ui->mail_list,&QListWidget::itemDoubleClicked,this,&receive_window::change_read);
    connect(ui->Return,&QPushButton::clicked,this,&receive_window::back);
    connect(ui->detele,&QPushButton::clicked,this,&receive_window::delete_mail);
}

receive_window::~receive_window()
{
    delete ui;
}

void receive_window::login(){

    ui->mail_list->clear();
    s = socket(PF_INET,SOCK_STREAM,0);
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(110); //pop3端口号
    hostent* hptr = gethostbyname(email_type.c_str());
    memcpy(&sin.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
    printf("IP of pop3_address is : %d:%d:%d:%d",
        sin.sin_addr.S_un.S_un_b.s_b1,
        sin.sin_addr.S_un.S_un_b.s_b2,
        sin.sin_addr.S_un.S_un_b.s_b3,
        sin.sin_addr.S_un.S_un_b.s_b4);

    cout << endl;
    if(::connect(s, (sockaddr*)&sin, sizeof(sin)))
    {
        QMessageBox::critical(this,"错误","pop3连接失败");
        return;
    }
    else{
        cout<<"connect succeed!";
    }
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;

    cout<<"input user_name"<<endl;
    string address = "user "+email_address+"\r\n";
    send(s, address.c_str(), address.size(), 0);
    len = recv(s, bufferresv, 10240, 0);
    bufferresv[len]=0;
    printf("%s",bufferresv);
    cout << endl;

    cout<<"input password"<<endl;
    string pwd = "pass "+password+"\r\n";
    send(s,pwd.c_str(),pwd.size(),0);
    len = recv(s,bufferresv,10240,0);
    bufferresv[len]= 0;
    printf("%s",bufferresv);
    cout << endl;

    cout<<"input list"<<endl;
    char list[] = "list\r\n";
    send(s,list,strlen(list),0);
    len = recv(s,bufferresv,10240,0);

    bufferresv[len]= 0;
    string list_ = bufferresv;
    cout<<list_<<endl;
    int message_num = find_message_num(bufferresv);
    cout<<"list_num = "<<message_num;

    load_listwidget(message_num,s);
}

int receive_window::find_message_num(char *bufferresv){
    //+OK 3 message(s) [23685 byte(s)]
    int len=-1;
    for(int i=4;i<int(strlen(bufferresv));i++){
        if(bufferresv[i]==' '){
            len=i-4;
            break;
        }
    }
    int num=0;
    for(int i=0;i<len;i++){
        num=num*10+bufferresv[4+len-1-i]-'0';
    }
    return num;
}
void receive_window::load_listwidget(int n,SOCKET s){
    for(int i=1;i<=n;i++){
        string retr = "retr "+to_string(i)+"\r\n";
        string email_context;
        send(s,retr.c_str(),retr.size(),0);
        len = recv(s,bufferresv,10240,0);
        bufferresv[len]= 0;
        email_context = bufferresv;
        qDebug("%s",qPrintable(QString::fromStdString(email_context)));
        len = recv(s,bufferresv,10240,0);
        bufferresv[len]= 0;
        email_context = bufferresv;
        qDebug("%s",qPrintable(QString::fromStdString(email_context)));
        vector<string>temp = find_name(email_context);
        string from_add = temp[0];
        string subject = temp[1];
        string time = temp[2];
        qDebug()<<"here";
        string name = from_add+"    "+subject+"    "+time;
        ui->mail_list->insertItem(i-1,QString::fromStdString(name));
        qDebug()<<i<<"#";
    }
}

vector<string> receive_window::find_name(string s){
    vector<string>temp(3);
    int pos0,pos1,pos2;
    pos0 = s.find("From:");
    if(pos0!=-1){
        pos1 = s.find("<",pos0);
        pos2 = s.find(">",pos1);
        temp[0] = s.substr(pos1+1,pos2-pos1-1);
    }

    qDebug()<<"2";
    pos0 = s.find("Subject:");
    if(pos0!=-1){
        pos1 =pos0;
        pos2 = s.find("\n",pos1);
        temp[1] = s.substr(pos1,pos2-pos1);
    }

    qDebug()<<"3";
    pos0 = s.find("Date:");
    if(pos0!=-1){
        pos1 = pos0;
        pos2 = s.find("\n",pos1);
        temp[2] = s.substr(pos1,pos2-pos1);
    }
    qDebug()<<"4";
    return temp;
}

void receive_window::change_read(){
    int num = ui->mail_list->currentRow()+1;
    string retr = "retr "+to_string(num)+"\r\n";
    string email_context;
    send(s,retr.c_str(),retr.size(),0);
    len = recv(s,bufferresv,10240,0);
    //bufferresv[len]= 0;
    len = recv(s,bufferresv,10240,0);
    bufferresv[len]= 0;
    email_context = bufferresv;
    vector<string>temp = find_name(email_context);
    string from_add = temp[0];
    string subject = temp[1];
    cout<<"from_add = "<<from_add;
    cout<<"subject = "<<subject;
    string body = find_body(email_context);
    ui->sender->setVisible(true);
    ui->subject->setVisible(true);
    ui->detele->setVisible(true);
    ui->sender->setText(QString::fromStdString(from_add));
    ui->subject->setText(QString::fromStdString(subject));
    ui->mail_list->setVisible(false);
    ui->Return->setVisible(true);
    ui->text_body->setVisible(true);
    ui->text_body->setText(QString::fromStdString(body));

}

void receive_window::back(){
    ui->mail_list->setVisible(true);
    ui->Return->setVisible(false);
    ui->text_body->setVisible(false);
    ui->sender->setVisible(false);
    ui->subject->setVisible(false);
    ui->detele->setVisible(false);
}

string receive_window::find_body(string s){
    string base64_s="";
    int pos0 = s.find("boundary=")+10;
    int pos1 = s.find("\"",pos0);
    string std = s.substr(pos0,pos1-pos0-1);
    pos0 = s.find(std,pos1);
    pos0 = s.find("\n",pos0);
    pos0 = s.find("\n",pos0+1);
    pos0 = s.find("\n",pos0+1)+3;
    pos1 = s.find("\n",pos0);
    base64_s = s.substr(pos0,pos1-pos0);
    char *trans_s = new char [1024];
    string ans;
    long l;
    qDebug()<<"before_trans";
    int key = CBase64::Decode(base64_s,trans_s,&l);
    trans_s[l] = 0;
    qDebug()<<"after_trans";
    if(!key)ans = "fall to decode from base 64 !";
    else ans= trans_s;
    return ans;
}

void receive_window::delete_mail(){
    int num = ui->mail_list->currentRow()+1;
    cout<<"current_row  = "<<num;
    string delt = "dele "+to_string(num)+"\r\n";
    send(s,delt.c_str(),delt.size(),0);
    len = recv(s,bufferresv,10240,0);
    bufferresv[len]= 0;
    string delete_rpy = bufferresv;
    cout<<delete_rpy<<endl;


    char list[] = "list\r\n";
    send(s,list,strlen(list),0);
    len = recv(s,bufferresv,10240,0);
    bufferresv[len]= 0;
    string list_ = bufferresv;
    cout<<list_<<endl;
    int message_num = find_message_num(bufferresv);
    ui->mail_list->clear();
    load_listwidget(message_num,s);
    back();

    QMessageBox::information(NULL,"","删除成功");
}
