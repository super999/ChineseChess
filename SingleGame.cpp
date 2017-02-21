#include "SingleGame.h"
#include <QTimer>
#include <QDebug>
SingleGame::SingleGame(QWidget *parent) : Board(parent)
{
    /*  初始化 AI 智能程度 */
    _level = 4;
}

SingleGame::~SingleGame()
{

}

/*  用户选择悔棋，那在电脑这边就得跟着悔棋，所以执行2次  */
void SingleGame::back()
{
    if(_bRedTurn)
    {
        backOne();
        backOne();
    }
}

/*  根据用户做的事返回对应的事   */
void SingleGame::click(int id, int row, int col)
{
    if(_bRedTurn)
    {
        Board::click(id, row, col);
        if(!_bRedTurn)
        {
            /*  启动0.1秒定时器，在0.1秒后电脑再思考  */
            QTimer::singleShot(100, this, SLOT(computerMove()));
        }
    }
}

/*  AI 行动   */
void SingleGame::computerMove()
{
    /*  先获取最佳步骤，然后按最佳步骤行动   */
    Step* step = getBestMove();
    moveStone(step->_moveid, step->_killid, step->_rowTo, step->_colTo);
    delete step;
    update();       //刷新界面
}

/*  获取最佳步骤  */
Step* SingleGame::getBestMove()
{
    /*  创建链表并初始化数据  */
    Step* ret = NULL;
    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int maxInAllMinScore = -300000;     //初始局面分尽可能低

    while(steps.count())
    {
        /*  防止内存泄露，类栈模式，将最后的取出来，在释放其内存后将其在链表中删除  */
        Step* step = steps.last();
        steps.removeLast();

        /*  走棋并计算局面分  */
        fakeMove(step);
        int minScore = getMinScore(this->_level-1, maxInAllMinScore);
        /*  移动后把棋子走回来   */
        unfakeMove(step);

        if(minScore > maxInAllMinScore)
        {
            if(ret)
            {
                delete ret;
            }

            ret = step;
            maxInAllMinScore = minScore;
        }
        else
        {
            delete step;
        }
    }
    return ret;
}

/*  评估局面分   */
int SingleGame::score()
{
    /*  存储每个棋子对应的分值  */
    static int s[] = {1000, 499, 501, 200, 150000, 100, 100};
    /*  黑棋的总分   */
    int scoreBlack = 0;
    /*  红棋的总分   */
    int scoreRed = 0;
    /*  计算红棋分数  */
    for(int i=0; i<16; ++i)
    {
        if(_s[i]._dead)
        {
            continue;
        }
        scoreRed += s[_s[i]._type];
    }
    /*  计算黑棋分数  */
    for(int i=16; i<32; ++i)
    {
        if(_s[i]._dead)
        {
            continue;
        }
        scoreBlack += s[_s[i]._type];
    }
    /*  将 黑棋分数 - 红棋分数 作为局面分 */
    return scoreBlack - scoreRed;
}

int SingleGame::getMinScore(int level, int curMin)
{
    if(level == 0)
    {
        return score();
    }

    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int minInAllMaxScore = 300000;

    while(steps.count())
    {
        /*  防止内存泄露，类栈模式，将最后的取出来，在释放其内存后将其在链表中删除  */
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int maxScore = getMaxScore(level-1, minInAllMaxScore);
        unfakeMove(step);
        delete step;

        if(maxScore <= curMin)
        {
            while(steps.count())
            {
                /*  防止内存泄露，类栈模式，将最后的取出来，在释放其内存后将其在链表中删除  */
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }
    }
    return minInAllMaxScore;
}

int SingleGame::getMaxScore(int level, int curMax)
{
    if(level == 0)
    {
        return score();
    }

    QVector<Step*> steps;
    getAllPossibleMove(steps);
    int maxInAllMinScore = -300000;

    while(steps.count())
    {
        /*  防止内存泄露，类栈模式，将最后的取出来，在释放其内存后将其在链表中删除  */
        Step* step = steps.last();
        steps.removeLast();

        fakeMove(step);
        int minScore = getMinScore(level-1, maxInAllMinScore);
        unfakeMove(step);
        delete step;

        if(minScore >= curMax)
        {
            while(steps.count())
            {
                /*  防止内存泄露，类栈模式，将最后的取出来，在释放其内存后将其在链表中删除  */
                Step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return minScore;
        }
        if(minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
        }


    }
    return maxInAllMinScore;
}

/*  模拟行动并计算对应的局面分   */
void SingleGame::fakeMove(Step *step)
{
    killStone(step->_killid);
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

/*  模拟行动完成后再悔棋  */
void SingleGame::unfakeMove(Step *step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

/*  获取所有能走的步骤   */
void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    /*  根据黑子和红子判断 ID 的取值范围  */
    int min, max;
    if(this->_bRedTurn)
    {
        min = 0, max = 16;
    }
    else
    {
        min = 16, max = 32;
    }

    /*  遍历每个棋子在每一个位置是否能移动   */
    for(int i=min;i<max; i++)
    {
        /*  如果棋子已经死了就不用继续判断了  */
        if(this->_s[i]._dead)
        {
            continue;
        }
        for(int row = 0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                /*  判断两个棋子颜色是否相同，不同就判断能不能移动    */
                int killid = this->getStoneId(row, col);
                if(sameColor(i, killid))
                {
                    continue;
                }

                if(canMove(i, killid, row, col))
                {
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
}
