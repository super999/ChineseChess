#include "MainWnd.h"
#include <QHBoxLayout>
#include "SingleGame.h"
#include "MultiGame.h"
#include "NetGame.h"

MainWnd::MainWnd(int gameType, QWidget *parent) : QWidget(parent)
{
    /*  获取游戏类型  */
    _gameType = gameType;

    /*
     *  0：创建人机对战
     *  1：创建人人对战
     *  2：创建网络对战服务器
     *  3：创建网络对战客户端
     */
    if(_gameType == 0)
    {
        /*  游戏部分，即下棋区域  */
        SingleGame* game = new SingleGame;
        /*  控制面板，即悔棋操作  */
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
    }
    else if(_gameType == 1)
    {
        MultiGame* game = new MultiGame;
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
    }
    else if(_gameType == 2)
    {
        NetGame* game = new NetGame(true);
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
    }
    else if(_gameType == 3)
    {
        NetGame* game = new NetGame(false);
        CtrlPanel* panel = new CtrlPanel;

        QHBoxLayout* hLay = new QHBoxLayout(this);
        hLay->addWidget(game, 1);
        hLay->addWidget(panel);
        connect(panel, SIGNAL(sigBack()), game, SLOT(slotBack()));
    }
}

MainWnd::~MainWnd()
{

}

