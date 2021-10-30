# 邮箱系统（SMTP+POP3)(QT实现)



## 目录：

- mail 文件夹下包含了所有QT的代码实现以及ui界面实现
- mail_exe 文件夹下包含了可执行文件



## 执行效果

- 基本登录界面

![image-20211030204930870](https://raw.githubusercontent.com/learner-lu/picbed/master/image-20211030204930870.png) 

- 写信界面

<img src="https://raw.githubusercontent.com/learner-lu/picbed/master/image-20211030205548099.png" style="zoom:80%;" /> 

- 收信界面

<img src="https://raw.githubusercontent.com/learner-lu/picbed/master/9S15VSM5TSMNEQQ4MEKD}_X.png" style="zoom:80%;" /> 

<img src="https://raw.githubusercontent.com/learner-lu/picbed/master/1.png" alt=" " style="zoom:80%;" /> 



## 功能：

- 支持smtp登录 pop3登录
- 支持发送，阅读，删除邮件
- 如果同时开启IMAP可实现客户端与服务器同步删除邮件



## run:

```shell
./mail_exe/mail.exe
```

或者直接进入mail_exe文件夹双击mail.exe执行

**本项目已经把QT所有的动态链接库全部加入mail_exe文件夹下，不需要配置QT的运行环境**



## attention:

- **客户端登录首先需要确认邮箱是否支持smtp/pop3登录，常见的邮箱类型如163/qq均不支持客户端登录，所以需要先开启客户端的smtp/pop3功能**，以163邮箱为例，其他邮箱开启方式类似，请自行解决

  - 设置->pop3/SMTP/IMAP

    ![image-20211030211640654](https://raw.githubusercontent.com/learner-lu/picbed/master/image-20211030211640654.png) 

  - 开启smtp/pop3服务

  ![image-20211030211855609](C:/Users/Administrator/AppData/Roaming/Typora/typora-user-images/image-20211030211855609.png)

  - **会提供一个登录码，改密码只会显示一次，请注意保存。登陆的时候使用此密码替换掉原先邮箱的密码，即邮箱名不变，密码使用此密码！**

- 需要连接网络，点击按钮之后如果出现卡顿、等待属于正常情况，请耐心等待一小会儿。如果长时间空转请通过任务管理器强制杀死进程
- 加速器等翻墙工具不会造成影响，无需担心。
- **部分邮箱，如qq即使开启了pop3服务仍然不支持pop3连接，在使用收信功能时会异常退出**
- MIME格式的邮件提取信息的部分为笔者自行实现，仅仅支持比较简单的邮件，如邮件基本文本传输，标题支持了汉语GBK编码，正文部分如果出现中文字符编码解码会出现base64加密乱码的情况，且如果是淘宝等大小网站发送的复杂邮件也可能会出现一些意料之外的问题。也不支持html、js等界面显示。
- 界面反复切换有时候也会出现卡顿或者进程崩溃等情况，尽量不要快速且频繁的切换
- **本项目消息缓冲区大小为10240b，如果某邮件(邮件消息实体部分)大小超过会造成缓冲区溢出的情况，解决办法请浏览代码实现部分**
- 本项目的解决方案并不完美，正常来说应该把登录发送连接调入后台多线程并行，在文件信息处理的时候应该用正则表达式，程序内部的信息纠错完成了一部分，可能还会出现一些用户意想不到的输入导致的错误。也没有在发送邮件的时候进行os/sql注入防御，可能被恶意攻击。
- 笔者水平有限，不足之处深感抱歉



## 代码实现：

### 头文件概览：

- CBase64.h   ~.cpp为base64编码实现

借鉴改动了 [源代码]( https://blog.csdn.net/chenxiaohua/article/details/4084602 ) ,实现了常见字符的编码解码 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

- 本项目使用socket 库，需要在QT中添加静态链接库，具体位置为mail.pro ，添加 ”LIBS += -lpthread libwsock32 libws2_32“

- mainwindow.h 登录窗口设计：如果缓冲区溢出修改所有位置（mainwindow.h+send_window.h+reveive_window.h)中的缓冲区大小

![image-20211030224506960](https://raw.githubusercontent.com/learner-lu/picbed/master/image-20211030224506960.png) 

- send_window.h 发送窗口设计

![image-20211030224750356](C:/Users/Administrator/AppData/Roaming/Typora/typora-user-images/image-20211030224750356.png) 

- receive_window.h 接收信息窗口实现

![image-20211030224846499](https://raw.githubusercontent.com/learner-lu/picbed/master/image-20211030224846499.png) 



### 框架设计：

- mainwindow中私有类创建发送、接收类的对象指针，并在界面切换时候传递对象内部需要的参数
- 创建按钮事件关联，以及信号的传递，指针+引用传递
- 关于c/c++的socket编程以及smtp/pop3连接，参考[网站](https://blog.csdn.net/yuzhenxiong0823/article/details/7713059?utm_source=app&app_version=4.17.0&utm_source=app) 
- 关于pop3指令，参考[rfc1939文件](https://www.ietf.org/rfc/rfc1939.txt?number=1939)

- 项目文件划分，函数命名以及变量名命名较为规范，而且在QT代码项目中也在合适的位置添加了qDebug()调试命令，便于找到问题所在。
- 关于代码实现细节,思路并不复杂,请自行理解



## 参考文件：

[qt导出exe文件](https://blog.csdn.net/qq_39054069/article/details/96481902)

[base64在线转码解码](https://www.qqxiuzi.cn/bianma/base64.htm)

