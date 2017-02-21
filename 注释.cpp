
#ifndef NETGAME_H
#define NETGAME_H

#include "Board.h"
#include <QTcpServer>
#include <QTcpSocket>
/*
 * 报文格式定义 
 * 1.初始化    两个字节 
 * 2.鼠标点击   四个字节 
 * 3.悔棋      一个字节 
 */
class NetGame : public Board
{
    Q_OBJECT
public:
    explicit NetGame(bool server, QWidget *parent = 0);
    ~NetGame();

    bool _bServer;             //判断是否是服务器 
    QTcpServer* _server;       //服务器 
    QTcpSocket* _socket;       //客户端 

    void back();               //悔棋 
    void click(int id, int row, int col);    //点击信息 

    void backFromNetwork(QByteArray buf);
    void clickFromNetwork(QByteArray buf);
    void initFromNetwork(QByteArray buf);

signals:

public slots:
    void slotNewConnection();   //两个信号槽，就是定义两个线程    发数据 
    void slotDataArrive();      //接收数据 
};

#endif // NETGAME_H 

#include "NetGame.h"

NetGame::NetGame(bool server, QWidget *parent) : Board(parent)    //继承棋盘面板
{
    _server = NULL;          //初始化服务器和客户端，让两者处于关闭状态
    _socket = NULL;
    _bServer = server;

    if(_bServer)      //判断是客户端还是服务器
    {
        _server = new QTcpServer(this);                //TCP协议
        _server->listen(QHostAddress::Any, 9899);      //启动监听端口
        connect(_server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));   //有连接的时候就自动调用槽函数，相当于线程
    }
    else
    {
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress("127.0.0.1"), 9899);
        connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
    }
}

void NetGame::slotNewConnection()         //服务器和客户端发数据的槽函数
{
    if(_socket) return;

    _socket = _server->nextPendingConnection();      //创建一个客户端和服务器通信用的socket
    connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));

    bool bRedSide = qrand()%2>0;	// 产生随机数来决定谁走红色 

    init(bRedSide);      //根据随机值判断是否翻转棋盘

    //发送给对方
    QByteArray buf;        //声明一个字符串数组
    buf.append(1);         //第一个字节固定式1
    buf.append(bRedSide?0:1);   //添加第二个字节，第二个字节是1或者0，1表示接收方走红旗，0表示走黑棋
    _socket->write(buf);        //把字节发送给客户端，让客户端来判断 
}

void NetGame::back()     //悔棋 
{
    if(_bRedTurn != _bSide)
    {
        return;
    }
    backOne();    //双方都后退一步 
    backOne();

    QByteArray buf;    //创建一个字节数组 
    buf.append(3);     //增加固定字节3---代表悔棋 
    _socket->write(buf);   //把悔棋的字节3发送给客户端或服务器 
}

void NetGame::click(int id, int row, int col)       //点击信息 
{
    if(_bRedTurn != _bSide)           //判断是不是在红方的时候去点了黑方的棋 
    {
        return;
    }

    Board::click(id, row, col);

    // 发送给对方 
    QByteArray buf;          //创建一个字节数组
    buf.append(2);           //第一个字节固定为2----代表发送点击信息
    buf.append(id);          //第二个字节为id
    buf.append(row);         //行
    buf.append(col);         //列
    _socket->write(buf);     //发送点击信息
}

void NetGame::backFromNetwork(QByteArray)         //悔棋同步到网络
{
    backOne();
    backOne();
}
void NetGame::clickFromNetwork(QByteArray buf)
{
    Board::click(buf[1], 9-buf[2], 8-buf[3]);     //调用棋盘定义的鼠标点击函数，用来处理鼠标点击的信息
}
void NetGame::initFromNetwork(QByteArray buf)
{
    bool bRedSide = buf.at(1)>0?true:false;       //客户端是否翻转棋盘
    init(bRedSide);
}

void NetGame::slotDataArrive()       //客户端和服务器收信息的槽函数
{
    QByteArray buf = _socket->readAll();   //客户端或服务器接收字节
    switch (buf.at(0))           //判断字节数组中第一个字节，判断接收到的是什么信息
    {
    case 1:
        initFromNetwork(buf);        //为红黑方
        break;
    case 2:
        clickFromNetwork(buf);      //点击信息
        break;
    case 3:
        backFromNetwork(buf);       //执行悔棋操作
        break;
    default:
        break;
    }
}

NetGame::~NetGame()
{

}
