#include "CtrlPanel.h"
#include <QGridLayout>
#include <QVBoxLayout>
/*  �������尴ť  */
CtrlPanel::CtrlPanel(QWidget *parent) : QWidget(parent)
{
    /*  ���ð�ť��Դ��ڵ�λ�ñ���  */
    QGridLayout* grid = new QGridLayout(this);
    QVBoxLayout* vLay = new QVBoxLayout;
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(2, 1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(2, 1);
    grid->addLayout(vLay, 1, 1);

    /*  �������尴ť���󶨵���Ϸ������ */
    vLay->addWidget(this->_back = new QPushButton(QStringLiteral("����")));
    connect(this->_back, SIGNAL(clicked()), this, SIGNAL(sigBack()));
}

CtrlPanel::~CtrlPanel()
{

}

