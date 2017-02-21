#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>

class Stone
{
public:
    Stone();
    ~Stone();

    /*  棋子的种类   */
    enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};

    /*  初始化函数   */
    void init(int id);

    /*
     * 棋子
     * 横坐标
     * 纵坐标
     * 类型
     * 是否存活
     * 是否为红方
     * 棋子ID
     */
    int _row;
    int _col;
    TYPE _type;
    bool _dead;
    bool _red;
    int _id;

    /*  取棋子名称   */
    QString name();

    /*  翻转棋子  */
    void rotate();
};

#endif // STONE_H
