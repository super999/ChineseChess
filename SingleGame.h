#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"

class SingleGame : public Board
{
    Q_OBJECT
public:
    explicit SingleGame(QWidget *parent = 0);
    ~SingleGame();

    /*  AI的智能程度，每 +1 就多考虑1层，消耗的资源更多，但更加智能   */
    int _level;

    /*  用户与 AI 的交互函数    */
    void back();
    void click(int id, int row, int col);

    /*  AI 算法   */
    Step* getBestMove();                                //根据局面分找出最好那一步
    void getAllPossibleMove(QVector<Step*>& steps);     //遍历所有可以走的步骤
    int getMinScore(int level, int curMin);             //取最小分数
    int getMaxScore(int level, int curMax);             //取最大分数
    int score();

    /*  模拟行动，根据行动后的局面计算分数   */
    void fakeMove(Step* step);
    void unfakeMove(Step* step);

signals:

public slots:
    void computerMove();
};

#endif // SINGLEGAME_H
