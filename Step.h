#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();

    /*
     * 链表的数据
     * 移动的棋子的ID
     * 被杀死的棋子的ID
     * 移动棋子的横坐标
     * 移动棋子的纵坐标
     * 棋子移到的位置的横坐标
     * 棋子移到的位置的纵坐标
     */
    int _moveid;
    int _killid;
    int _rowFrom;
    int _colFrom;
    int _rowTo;
    int _colTo;

signals:

public slots:
};

#endif // STEP_H
