#include "Stone.h"
#include <QDebug>

Stone::Stone()
{

}

Stone::~Stone()
{

}

/*  根据类型返回对应的棋子名称   */
QString Stone::name()
{
    switch(this->_type)
    {
    case CHE:
        return QStringLiteral("車");
    case MA:
        return QStringLiteral("馬");
    case PAO:
        return QStringLiteral("炮");
    case BING:
        if (this->_red)
        {
            return QStringLiteral("兵");
        }
        else
        {
            return QStringLiteral("卒");
        }
    case JIANG:
        if (this->_red)
        {
            return QStringLiteral("帅");
        }
        else
        {
            return QStringLiteral("将");
        }
    case SHI:
        if (this->_red)
        {
            return QStringLiteral("仕");
        }
        else
        {
            return QStringLiteral("士");
        }
    case XIANG:
        if (this->_red)
        {
            return QStringLiteral("象");
        }
        else
        {
            return QStringLiteral("相");
        }
    }
    return QStringLiteral("错误");
}

/*  初始化函数   */
void Stone::init(int id)
{
    /*  初始时棋子的横坐标、纵坐标、类型  */
    struct {
        int row, col;
        Stone::TYPE type;
    } pos[16] = {
    {0, 0, Stone::CHE},
    {0, 1, Stone::MA},
    {0, 2, Stone::XIANG},
    {0, 3, Stone::SHI},
    {0, 4, Stone::JIANG},
    {0, 5, Stone::SHI},
    {0, 6, Stone::XIANG},
    {0, 7, Stone::MA},
    {0, 8, Stone::CHE},

    {2, 1, Stone::PAO},
    {2, 7, Stone::PAO},
    {3, 0, Stone::BING},
    {3, 2, Stone::BING},
    {3, 4, Stone::BING},
    {3, 6, Stone::BING},
    {3, 8, Stone::BING},
    };

    if(id < 16)
    {
        this->_col = pos[id].col;
        this->_row = pos[id].row;
        this->_type = pos[id].type;
    }
    else
    {
        this->_col = 8-pos[id-16].col;
        this->_row = 9-pos[id-16].row;
        this->_type = pos[id-16].type;
    }

    this->_dead = false;
    this->_red = id<16;
}

/*  翻转棋子  */
void Stone::rotate()
{
    this->_col = 8-this->_col;
    this->_row = 9-this->_row;
}
