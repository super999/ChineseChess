#include "CtrlPanel.h"
#include <QGridLayout>
#include <QVBoxLayout>
/*  创建悔棋按钮  */
CtrlPanel::CtrlPanel(QWidget *parent) : QWidget(parent)
{
    /*  设置按钮相对窗口的位置比例  */
    QGridLayout* grid = new QGridLayout(this);
    QVBoxLayout* vLay = new QVBoxLayout;
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(2, 1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(2, 1);
    grid->addLayout(vLay, 1, 1);

    /*  创建悔棋按钮并绑定到游戏窗口上 */
    vLay->addWidget(this->_back = new QPushButton(QStringLiteral("悔棋")));
    connect(this->_back, SIGNAL(clicked()), this, SIGNAL(sigBack()));
}

CtrlPanel::~CtrlPanel()
{

}

