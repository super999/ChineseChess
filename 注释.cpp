
#ifndef NETGAME_H
#define NETGAME_H

#include "Board.h"
#include <QTcpServer>
#include <QTcpSocket>
/*
 * ���ĸ�ʽ���� 
 * 1.��ʼ��    �����ֽ� 
 * 2.�����   �ĸ��ֽ� 
 * 3.����      һ���ֽ� 
 */
class NetGame : public Board
{
    Q_OBJECT
public:
    explicit NetGame(bool server, QWidget *parent = 0);
    ~NetGame();

    bool _bServer;             //�ж��Ƿ��Ƿ����� 
    QTcpServer* _server;       //������ 
    QTcpSocket* _socket;       //�ͻ��� 

    void back();               //���� 
    void click(int id, int row, int col);    //�����Ϣ 

    void backFromNetwork(QByteArray buf);
    void clickFromNetwork(QByteArray buf);
    void initFromNetwork(QByteArray buf);

signals:

public slots:
    void slotNewConnection();   //�����źŲۣ����Ƕ��������߳�    ������ 
    void slotDataArrive();      //�������� 
};

#endif // NETGAME_H 

#include "NetGame.h"

NetGame::NetGame(bool server, QWidget *parent) : Board(parent)    //�̳��������
{
    _server = NULL;          //��ʼ���������Ϳͻ��ˣ������ߴ��ڹر�״̬
    _socket = NULL;
    _bServer = server;

    if(_bServer)      //�ж��ǿͻ��˻��Ƿ�����
    {
        _server = new QTcpServer(this);                //TCPЭ��
        _server->listen(QHostAddress::Any, 9899);      //���������˿�
        connect(_server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));   //�����ӵ�ʱ����Զ����òۺ������൱���߳�
    }
    else
    {
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress("127.0.0.1"), 9899);
        connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
    }
}

void NetGame::slotNewConnection()         //�������Ϳͻ��˷����ݵĲۺ���
{
    if(_socket) return;

    _socket = _server->nextPendingConnection();      //����һ���ͻ��˺ͷ�����ͨ���õ�socket
    connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));

    bool bRedSide = qrand()%2>0;	// ���������������˭�ߺ�ɫ 

    init(bRedSide);      //�������ֵ�ж��Ƿ�ת����

    //���͸��Է�
    QByteArray buf;        //����һ���ַ�������
    buf.append(1);         //��һ���ֽڹ̶�ʽ1
    buf.append(bRedSide?0:1);   //��ӵڶ����ֽڣ��ڶ����ֽ���1����0��1��ʾ���շ��ߺ��죬0��ʾ�ߺ���
    _socket->write(buf);        //���ֽڷ��͸��ͻ��ˣ��ÿͻ������ж� 
}

void NetGame::back()     //���� 
{
    if(_bRedTurn != _bSide)
    {
        return;
    }
    backOne();    //˫��������һ�� 
    backOne();

    QByteArray buf;    //����һ���ֽ����� 
    buf.append(3);     //���ӹ̶��ֽ�3---������� 
    _socket->write(buf);   //�ѻ�����ֽ�3���͸��ͻ��˻������ 
}

void NetGame::click(int id, int row, int col)       //�����Ϣ 
{
    if(_bRedTurn != _bSide)           //�ж��ǲ����ں췽��ʱ��ȥ���˺ڷ����� 
    {
        return;
    }

    Board::click(id, row, col);

    // ���͸��Է� 
    QByteArray buf;          //����һ���ֽ�����
    buf.append(2);           //��һ���ֽڹ̶�Ϊ2----�����͵����Ϣ
    buf.append(id);          //�ڶ����ֽ�Ϊid
    buf.append(row);         //��
    buf.append(col);         //��
    _socket->write(buf);     //���͵����Ϣ
}

void NetGame::backFromNetwork(QByteArray)         //����ͬ��������
{
    backOne();
    backOne();
}
void NetGame::clickFromNetwork(QByteArray buf)
{
    Board::click(buf[1], 9-buf[2], 8-buf[3]);     //�������̶��������������������������������Ϣ
}
void NetGame::initFromNetwork(QByteArray buf)
{
    bool bRedSide = buf.at(1)>0?true:false;       //�ͻ����Ƿ�ת����
    init(bRedSide);
}

void NetGame::slotDataArrive()       //�ͻ��˺ͷ���������Ϣ�Ĳۺ���
{
    QByteArray buf = _socket->readAll();   //�ͻ��˻�����������ֽ�
    switch (buf.at(0))           //�ж��ֽ������е�һ���ֽڣ��жϽ��յ�����ʲô��Ϣ
    {
    case 1:
        initFromNetwork(buf);        //Ϊ��ڷ�
        break;
    case 2:
        clickFromNetwork(buf);      //�����Ϣ
        break;
    case 3:
        backFromNetwork(buf);       //ִ�л������
        break;
    default:
        break;
    }
}

NetGame::~NetGame()
{

}
