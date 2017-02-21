#ifndef BOARD_H
#define BOARD_H

#include <QFrame>
#include "Stone.h"
#include "Step.h"
#include <QVector>

class Board : public QFrame
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    //��Ϸ����
    Stone _s[32];                           //����
    int _r;                                 //���Ӱ뾶
    QPoint _off;                            //����
    bool _bSide;                            //�Ƿ����

    //��¼����
    QVector<Step*> _steps;

    //��Ϸ��������
    int _selectid;                          //ѡ�е�����ID��ûѡ����Ϊ-1
    bool _bRedTurn;                         //�Ƿ�Ϊ�췽�غ�
    void init(bool bRedSide);               //��ʼ������

    //��ͼ����
    void paintEvent(QPaintEvent *);         //���ڴ�����Ϻ�ĵ���
    void drawPlate(QPainter& p);            //��������
    void drawPlace(QPainter& p);            //���ƾŹ���
    void drawInitPosition(QPainter& p);     //��ʼ������λ��
    void drawInitPosition(QPainter& p, int row, int col);       //��ʼ������λ��
    void drawStone(QPainter& p);            //��������
    void drawStone(QPainter& p, int id);    //��������

    //΢������
    QPoint center(int row, int col);        //���ݵ����λ��ȡ����Ӧ�ĵ������
    QPoint center(int id);                  //ͬ��
    QPoint topLeft(int row, int col);       //�������(0, 0)������
    QPoint topLeft(int id);                 //ͬ��
    QRect cell(int row, int col);           //ȡ�����ӵ����Ƶ����ĵ����꣬���ڻ�����������
    QRect cell(int id);                     //ͬ��

    //�������λ�û�Ϊ����
    bool getClickRowCol(QPoint pt, int& row, int& col);

    //��������
    QString name(int id);                   //ȡ��������
    bool red(int id);                       //�ж��Ƿ��Ǻ췽����
    bool sameColor(int id1, int id2);       //�ж�������ɫ�Ƿ���ͬ
    int getStoneId(int row, int col);       //��������ȡ����id
    void killStone(int id);                 //ɱ������
    void reliveStone(int id);               //�ػ�����
    void moveStone(int moveid, int row, int col);   //�ƶ�����
    bool isDead(int id);                    //�ж������Ƿ�����

    //�ƶ�����
    void mouseReleaseEvent(QMouseEvent *);  //����ͷ�ʱ����
    void click(QPoint pt);                  //�������Ĵ�����
    virtual void click(int id, int row, int col);       //���ߵ�����ҵ�����
    void trySelectStone(int id);            //����ѡ������
    void tryMoveStone(int killid, int row, int col);    //�����ƶ�����
    void moveStone(int moveid, int killid, int row, int col);       //�ƶ�����
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);     //���沽��
    void backOne();                         //����һ��
    void back(Step* step);                  //����
    virtual void back();                    //���ߵ������ѡ�����

    //����
    bool canMove(int moveid, int killid, int row, int col);         //�ж������ܷ��ƶ�
    bool canMoveChe(int moveid, int killid, int row, int col);      //܇�ܷ��ƶ�
    bool canMoveMa(int moveid, int killid, int row, int col);       //���ܷ��ƶ�
    bool canMovePao(int moveid, int killid, int row, int col);      //���ܷ��ƶ�
    bool canMoveBing(int moveid, int killid, int row, int col);     //���ܷ��ƶ�
    bool canMoveJiang(int moveid, int killid, int row, int col);    //���ܷ��ƶ�
    bool canMoveShi(int moveid, int killid, int row, int col);      //ʿ�ܷ��ƶ�
    bool canMoveXiang(int moveid, int killid, int row, int col);    //���ܷ��ƶ�

    //�ܷ�ѡ��
    bool canSelect(int id);

    //����������
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);    //�ж���������֮���м�������
    int relation(int row1, int col1, int row, int col);                 //ȡ��2��λ�õ����λ�ã�����ʿ���ࡢ�������ӵ��ƶ��ж�
    bool isBottomSide(int id);                                          //�Ƿ���˽�

signals:

public slots:
    void slotBack();

};

#endif // BOARD_H
