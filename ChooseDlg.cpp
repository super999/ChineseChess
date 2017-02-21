#include "ChooseDlg.h"
#include <QVBoxLayout>

ChooseDlg::ChooseDlg(QWidget *parent) : QDialog(parent)
{
    /*  创建 4 个按钮   */
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->addWidget(_buttons[0] = new QPushButton("人机对战"));
    lay->addWidget(_buttons[1] = new QPushButton("人人对战"));
    lay->addWidget(_buttons[2] = new QPushButton("网络对战(server)"));
    lay->addWidget(_buttons[3] = new QPushButton("网络对战(client)"));

    /*  把按钮关联到窗口上   */
    for(int i=0; i<4; ++i)
    {
        connect(_buttons[i], SIGNAL(clicked()), this, SLOT(slotClicked()));
    }
}

/*  将获取到的用户点击的按钮的编号传输给main.cpp，并调用对应函数创建窗口   */
void ChooseDlg::slotClicked()
{
    /*  将被选中的棋子传递给 main ，由 main 创建对应的游戏窗口  */
    QObject* s = sender();
    for(int i=0; i<4; ++i)
    {
        if(_buttons[i] == s)
        {
            this->_selected = i;
            break;
        }
    }
    accept();
}

ChooseDlg::~ChooseDlg()
{

}

