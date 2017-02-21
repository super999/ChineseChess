#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

/*  利用构造函数初始化数据 */
Board::Board(QWidget *parent) : QFrame(parent)
{
    /*
     * 初始化棋子半径并初始化窗口，最后调用初始化棋子函数
     */
    this->_r = 20;
    setMinimumSize(_r*18+1, _r*20+1);
    init(true);
}

Board::~Board()
{

}

/*  初始化棋子   */
void Board::init(bool bRedSide)
{
    /*  为32个棋子进行初始化 */
    for(int i=0; i<32; ++i)
    {
        _s[i].init(i);
    }
    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            _s[i].rotate();
        }
    }

    /*  设置选中为无，红方回合，最后刷新界面  */
    _selectid = -1;
    _bRedTurn = true;
    _bSide = bRedSide;
    update();
}

/*  窗口创建完毕后的调用  */
void Board::paintEvent(QPaintEvent *)
{
    /*  创建窗口完毕后，初始化半径和棋盘坐标原点   */
    int r = height()/20;
    _r = r;
    _off = QPoint(r+1, r+1);

    /*  创建绘画工具，准备绘图 */
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    /*  保存P的状态，使其不被函数改变，并调用函数初始化棋盘、九宫格、棋子等*/
    p.save();
    drawPlate(p);
    p.restore();

    p.save();
    drawPlace(p);
    p.restore();

    p.save();
    drawInitPosition(p);
    p.restore();

    p.save();
    drawStone(p);
    p.restore();
}

/*  绘制棋子    */
void Board::drawStone(QPainter &p)
{
    for(int i=0; i<32; i++)
    {
        drawStone(p, i);
    }
}

/*  判断棋子是否阵亡    */
bool Board::isDead(int id)
{
    if(id == -1)
    {
        return true;
    }
    return _s[id]._dead;
}

/*  绘制棋子    */
void Board::drawStone(QPainter &p, int id)
{
    /*  棋子死亡不用画 */
    if(isDead(id))
    {
        return;
    }

    /*  红色棋子就用红色画笔，黑色棋子用蓝色画笔    */
    QColor color;
    if(red(id))
    {
        color = QColor(255, 45, 0);
    }
    else
    {
        color = QColor(0, 245, 255);
    }
    p.setPen(QPen(QBrush(color), 2));

    if(id == _selectid)
    {
        p.setBrush(Qt::white);
    }
    else
    {
        p.setBrush(QColor(255,255,255,0));
    }

    /*  画圆圈和棋子名  */
    p.drawEllipse(cell(id));
    p.setFont(QFont("隶书", _r*1.5, 700));
    p.drawText(cell(id), name(id), QTextOption(Qt::AlignCenter));
}

/*  取棋子名称   */
QString Board::name(int id)
{
    return _s[id].name();
}

/*  判断是否是红方棋子   */
bool Board::red(int id)
{
    return _s[id]._red;
}

/*  初始化棋子位置   */
void Board::drawInitPosition(QPainter &p, int row, int col)
{
    QPoint pt = center(row, col);
    QPoint off = QPoint(_r/6, _r/6);
    int len = _r/3;

    QPoint ptStart;
    QPoint ptEnd;

    if(col != 0)
    {
        ptStart = QPoint(pt.x() - off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);

        ptStart = QPoint(pt.x() - off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);
    }

    if(col != 8)
    {
        ptStart = QPoint(pt.x() + off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);

        ptStart = QPoint(pt.x() + off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);
    }
}

/*  初始化棋子位置 */
void Board::drawInitPosition(QPainter &p)
{
    drawInitPosition(p, 3, 0);
    drawInitPosition(p, 3, 2);
    drawInitPosition(p, 3, 4);
    drawInitPosition(p, 3, 6);
    drawInitPosition(p, 3, 8);

    drawInitPosition(p, 6, 0);
    drawInitPosition(p, 6, 2);
    drawInitPosition(p, 6, 4);
    drawInitPosition(p, 6, 6);
    drawInitPosition(p, 6, 8);

    drawInitPosition(p, 2, 1);
    drawInitPosition(p, 2, 7);

    drawInitPosition(p, 7, 1);
    drawInitPosition(p, 7, 7);
}

/*  绘制九宫格    */
void Board::drawPlace(QPainter &p)
{
    p.setPen(Qt::white);

    p.drawLine(center(0, 3), center(2, 5));
    p.drawLine(center(2, 3), center(0, 5));

    p.drawLine(center(9, 3), center(7, 5));
    p.drawLine(center(7, 3), center(9, 5));
}

/*  绘制棋盘    */
void Board::drawPlate(QPainter &p)
{
    for(int i=0; i<10; ++i)
    {
        /*
        if(i==0 || i==9)
        {
            p.setPen(QPen(Qt::white, 3, Qt::SolidLine));
        }
        else
        {
        */
            p.setPen(QPen(Qt::white, 1, Qt::SolidLine));
        //}
        p.drawLine(center(i, 0), center(i, 8));
    }

    for(int i=0; i<9; ++i)
    {

        if(i==0 || i==8)
        {
            p.setPen(QPen(Qt::white, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(9, i));
        }
        else
        {

            p.setPen(QPen(Qt::white, 1, Qt::SolidLine));
            p.drawLine(center(0, i), center(4, i));
            p.drawLine(center(5, i), center(9, i));
        }
    }
}

/*  根据点击的位置取出对应的点的坐标    */
QPoint Board::center(int row, int col)
{
    QPoint pt(_r*col*2, _r*row*2);
    return pt + _off;
}

/*  根据点击的位置取出对应的点的坐标    */
QPoint Board::center(int id)
{
    return center(_s[id]._row, _s[id]._col);
}

/*  相对棋盘(0, 0)的坐标   */
QPoint Board::topLeft(int row, int col)
{
    return center(row, col) - QPoint(_r, _r);
}

/*  相对棋盘(0, 0)的坐标   */
QPoint Board::topLeft(int id)
{
    return center(id) - QPoint(_r, _r);
}

/*  取出棋子的名称的中心点坐标，用于绘制棋子名称  */
QRect Board::cell(int row, int col)
{
    return QRect(topLeft(row, col), QSize(_r*2-1, _r*2-1));
}

/*  取出棋子的名称的中心点坐标，用于绘制棋子名称  */
QRect Board::cell(int id)
{
    return QRect(topLeft(id), QSize(_r*2-1, _r*2-1));
}

/*  将点击的位置化为坐标  */
bool Board::getClickRowCol(QPoint pt, int &row, int &col)
{
    /*每个坐标都判断，看看哪一个和所给坐标有 x^2 + y^2 <= r^2 的关系，有就是这个点   */
    for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            QPoint distance = center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < _r* _r)
            {
                return true;
            }
        }
    }
    return false;
}

/*  鼠标释放时调用  */
void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }

    click(ev->pos());
}

/*  能否被选中   */
bool Board::canSelect(int id)
{
    return _bRedTurn == _s[id]._red;
}

/*  尝试选中棋子  */
void Board::trySelectStone(int id)
{
    /*  如果先前选中了棋子，就不能再选中    */
    if(id == -1)
    {
        return;
    }
    /*  如果选中的棋子不是奔放的棋子，不能选中 */
    if(!canSelect(id))
    {
        return;
    }
    /*  选中棋子并刷新窗口   */
    _selectid = id;
    update();
}

/*  判断棋子颜色是否相同  */
bool Board::sameColor(int id1, int id2)
{
    /*  如果有一个棋子不存在就为假，否则返回比较结果  */
    if(id1 == -1 || id2 == -1)
    {
        return false;
    }
    return red(id1) == red(id2);
}

/*  尝试移动棋子  */
void Board::tryMoveStone(int killid, int row, int col)
{
    /*  如果移动的位置有棋子并且棋子和选中的棋子同色，就尝试选中新棋子    */
    if(killid != -1 && sameColor(killid, _selectid))
    {
        trySelectStone(killid);
        return;
    }

    /*  如果那个位置没有棋子或选中棋子和移动的位置的那个棋子不同色，就尝试杀死棋子并完成移动  */
    bool ret = canMove(_selectid, killid, row, col);
    if(ret)
    {
        moveStone(_selectid, killid, row, col);
        _selectid = -1;
        update();
    }
}

/*  判断两个棋子之间有几个棋子   */
int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;                    //中间有多少棋子
    /*  如果是斜着走就返回 -1    */
    if(row1 != row2 && col1 != col2)
    {
        return -1;
    }
    /*  如果是原地移动也返回 -1   */
    if(row1 == row2 && col1 == col2)
    {
        return -1;
    }

    /*  如果是 x 相同，取出连线上每个坐标判断有没有棋子   */
    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getStoneId(row1, col) != -1)
            {
                ++ret;
            }
        }
    }
    /*  如果是 y 相同，取出连线上每个坐标偶的有没有棋子   */
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getStoneId(row, col1) != -1)
            {
                ++ret;
            }
        }
    }

    return ret;
}

/*  取出2个位置的相对位置，用于士、相、将等棋子的移动判断 */
int Board::relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10 + qAbs(col1-col);
}

/*  車能否移动   */
bool Board::canMoveChe(int moveid, int, int row, int col)
{
    /*  判断连线中间有没有棋子，没有就可以走，有就不能走    */
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row1, col1, row, col);
    if(ret == 0)
    {
        return true;
    }
    return false;
}

/*  马能否移动   */
bool Board::canMoveMa(int moveid, int, int row, int col)
{
    /*  取出两个坐标的相对位置，如果是 12 或者是 21 就代表走的是"日"  */
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 12 && r != 21)
    {
        return false;
    }

    /*  判断是横着还是竖着，然后就判断马腿有没有棋子  */
    if(r == 12)
    {
        if(getStoneId(row1, (col+col1)/2) != -1)
        {
            return false;
        }
    }
    else
    {
        if(getStoneId((row+row1)/2, col1) != -1)
        {
            return false;
        }
    }

    return true;
}

/*  炮能否移动   */
bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    /*  判断目的地有没有棋子，有的话中间要有一个棋子，没有的话中间不能有棋子  */
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row, col, row1, col1);
    if(killid != -1)
    {
        if(ret == 1)
        {
            return true;
        }
    }
    else
    {
        if(ret == 0)
        {
            return true;
        }
    }
    return false;
}

/*  兵能否移动   */
bool Board::canMoveBing(int moveid, int, int row, int col)
{
    /*  取两个坐标的相对位置，如果是 1 或者 10 就代表是符合规则的走法  */
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10)
    {
        return false;
    }

    /*  判断兵是哪一方的，有没有越界，才能走横着    */
    if(isBottomSide(moveid))
    {
        if(row > row1)
        {
            return false;
        }
        /*  没越界横着走  */
        if(row1 >= 5 && row == row1)
        {
            return false;
        }
    }
    else
    {
        if(row < row1)
        {
            return false;
        }
        /*  没越界横着走  */
        if(row1 <= 4 && row == row1)
        {
            return false;
        }
    }

    return true;
}

/*  将能否移动   */
bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
    /*  将对将 */
    if(killid != -1 && _s[killid]._type == Stone::JIANG)
    {
        return canMoveChe(moveid, killid, row, col);
    }

    /*  取出相对位置，如果为 1 或者 10 就判断有没有出九宫格，没有就能移动    */
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10)
    {
        return false;
    }

    /*  根据坐标判断有没有出九宫格  */
    if(col < 3 || col > 5)
    {
        return false;
    }
    if(isBottomSide(moveid))
    {
        if(row < 7)
        {
            return false;
        }
    }
    else
    {
        if(row > 2)
        {
            return false;
        }
    }
    return true;
}

/*  士能否移动   */
bool Board::canMoveShi(int moveid, int, int row, int col)
{
    /*  取相对位置，必须是11，否则不能走   */
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 11)
    {
        return false;
    }

    /*  根据坐标判断有没有出九宫格   */
    if(col < 3 || col > 5)
    {
        return false;
    }
    if(isBottomSide(moveid))
    {
        if(row < 7)
        {
            return false;
        }
    }
    else
    {
        if(row > 2)
        {
            return false;
        }
    }
    return true;
}

/*  相能否移动   */
bool Board::canMoveXiang(int moveid, int, int row, int col)
{
    /*  取相对位置，必须为 22，否则不能移动 */
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 22)
    {
        return false;
    }

    /*  判断象眼有没有棋子   */
    int rEye = (row+row1)/2;
    int cEye = (col+col1)/2;
    if(getStoneId(rEye, cEye) != -1)
    {
        return false;
    }

    /*  判断有没有过河 */
    if(isBottomSide(moveid))
    {
        if(row < 4)
        {
            return false;
        }
    }
    else
    {
        if(row > 5)
        {
            return false;
        }
    }
    return true;
}

/*  判断棋子能否移动    */
bool Board::canMove(int moveid, int killid, int row, int col)
{
    /*  判断两次选中的棋子会不会同色，会就更换选中的棋子并返回 false   */
    if(sameColor(moveid, killid))
    {
        return false;
    }

    /*  根据棋子类型调用不同的函数判断能否移动 */
    switch (_s[moveid]._type)
    {
    case Stone::CHE:
        return canMoveChe(moveid, killid, row, col);
    case Stone::MA:
        return canMoveMa(moveid, killid, row, col);
    case Stone::PAO:
        return canMovePao(moveid, killid, row, col);
    case Stone::BING:
        return canMoveBing(moveid, killid, row, col);
    case Stone::JIANG:
        return canMoveJiang(moveid, killid, row, col);
    case Stone::SHI:
        return canMoveShi(moveid, killid, row, col);
    case Stone::XIANG:
        return canMoveXiang(moveid, killid, row, col);
    }
    return false;
}

/*  重绘棋子    */
void Board::reliveStone(int id)
{
    /*  复活棋子，如果棋子不存在就不复活    */
    if(id==-1)
    {
        return;
    }
    _s[id]._dead = false;
}

/*  杀死棋子    */
void Board::killStone(int id)
{
    /*  杀死棋子，如果棋子不存在就不杀死    */
    if(id==-1)
    {
        return;
    }
    _s[id]._dead = true;
}

/*  是否过了界   */
bool Board::isBottomSide(int id)
{
    return _bSide == _s[id]._red;
}

/*  移动棋子    */
void Board::moveStone(int moveid, int row, int col)
{
    /*  更改坐标后切换回合   */
    _s[moveid]._row = row;
    _s[moveid]._col = col;

    _bRedTurn = !_bRedTurn;
}

/*  保存步骤    */
void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    /*  将这一步做的事存到链表里，方便悔棋   */
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}

/*  移动棋子    */
void Board::moveStone(int moveid, int killid, int row, int col)
{
    /*  先保存步骤，然后执行步骤的内容 */
    saveStep(moveid, killid, row, col, _steps);

    /*  判断有没有杀死主将，杀了就结束游戏  */
    if (_s[killid]._type == Stone::JIANG)
    {
        /*  加入游戏结束代码！！！！！！！！！！！！！！！！！！！    */
        exit(0);
    }

    /*  杀死并移动棋子  */
    killStone(killid);
    moveStone(moveid, row, col);
}

/*  告诉电脑玩家的走棋  */
void Board::click(int id, int row, int col)
{
    /*  之前有选中的棋子就移动，没有就选中   */
    if(this->_selectid == -1)
    {
        trySelectStone(id);
    }
    else
    {
        tryMoveStone(id, row, col);
    }
}

/*  鼠标点击后的处理函数  */
void Board::click(QPoint pt)
{
    /*  判断点击的位置是否在棋盘内，有没有棋子，有棋子返回id  */
    int row, col;
    bool bClicked = getClickRowCol(pt, row, col);
    if(!bClicked)
    {
        return;
    }

    int id = getStoneId(row, col);
    click(id, row, col);
}

/*  根据坐标取棋子id   */
int Board::getStoneId(int row, int col)
{
    /*  挨个棋子判断是否在这个坐标   */
    for(int i=0; i<32; ++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && !isDead(i))
        {
            return i;
        }
    }
    return -1;
}

/*  回退  */
void Board::back(Step *step)
{
    /*  复活被杀的棋子，方向走之前保存的步骤  */
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
    _selectid = -1;
    update();
}

/*  回退一步    */
void Board::backOne()
{
    if(this->_steps.size() == 0)
    {
        return;
    }

    Step* step = this->_steps.last();
    _steps.removeLast();
    back(step);

    _selectid = -1;
    update();
    delete step;
}

/*  子类回退    */
void Board::back()
{
    backOne();
}

/*  回退  */
void Board::slotBack()
{
    back();
}
