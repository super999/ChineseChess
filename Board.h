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

    //游戏数据
    Stone _s[32];                           //棋子
    int _r;                                 //棋子半径
    QPoint _off;                            //坐标
    bool _bSide;                            //是否过河

    //记录步骤
    QVector<Step*> _steps;

    //游戏过程数据
    int _selectid;                          //选中的棋子ID，没选中则为-1
    bool _bRedTurn;                         //是否为红方回合
    void init(bool bRedSide);               //初始化棋子

    //绘图函数
    void paintEvent(QPaintEvent *);         //窗口创建完毕后的调用
    void drawPlate(QPainter& p);            //绘制棋盘
    void drawPlace(QPainter& p);            //绘制九宫格
    void drawInitPosition(QPainter& p);     //初始化棋子位置
    void drawInitPosition(QPainter& p, int row, int col);       //初始化棋子位置
    void drawStone(QPainter& p);            //绘制棋子
    void drawStone(QPainter& p, int id);    //绘制棋子

    //微调函数
    QPoint center(int row, int col);        //根据点击的位置取出对应的点的坐标
    QPoint center(int id);                  //同上
    QPoint topLeft(int row, int col);       //相对棋盘(0, 0)的坐标
    QPoint topLeft(int id);                 //同上
    QRect cell(int row, int col);           //取出棋子的名称的中心点坐标，用于绘制棋子名称
    QRect cell(int id);                     //同上

    //将点击的位置化为坐标
    bool getClickRowCol(QPoint pt, int& row, int& col);

    //辅助函数
    QString name(int id);                   //取棋子名称
    bool red(int id);                       //判断是否是红方棋子
    bool sameColor(int id1, int id2);       //判断棋子颜色是否相同
    int getStoneId(int row, int col);       //根据坐标取棋子id
    void killStone(int id);                 //杀死棋子
    void reliveStone(int id);               //重绘棋子
    void moveStone(int moveid, int row, int col);   //移动棋子
    bool isDead(int id);                    //判断棋子是否阵亡

    //移动棋子
    void mouseReleaseEvent(QMouseEvent *);  //鼠标释放时调用
    void click(QPoint pt);                  //鼠标点击后的处理函数
    virtual void click(int id, int row, int col);       //告诉电脑玩家的走棋
    void trySelectStone(int id);            //尝试选中棋子
    void tryMoveStone(int killid, int row, int col);    //尝试移动棋子
    void moveStone(int moveid, int killid, int row, int col);       //移动棋子
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);     //保存步骤
    void backOne();                         //回退一步
    void back(Step* step);                  //回退
    virtual void back();                    //告诉电脑玩家选择回退

    //规则
    bool canMove(int moveid, int killid, int row, int col);         //判断棋子能否移动
    bool canMoveChe(int moveid, int killid, int row, int col);      //車能否移动
    bool canMoveMa(int moveid, int killid, int row, int col);       //马能否移动
    bool canMovePao(int moveid, int killid, int row, int col);      //炮能否移动
    bool canMoveBing(int moveid, int killid, int row, int col);     //兵能否移动
    bool canMoveJiang(int moveid, int killid, int row, int col);    //将能否移动
    bool canMoveShi(int moveid, int killid, int row, int col);      //士能否移动
    bool canMoveXiang(int moveid, int killid, int row, int col);    //相能否移动

    //能否被选中
    bool canSelect(int id);

    //规则辅助函数
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);    //判断两个棋子之间有几个棋子
    int relation(int row1, int col1, int row, int col);                 //取出2个位置的相对位置，用于士、相、将等棋子的移动判断
    bool isBottomSide(int id);                                          //是否过了界

signals:

public slots:
    void slotBack();

};

#endif // BOARD_H
