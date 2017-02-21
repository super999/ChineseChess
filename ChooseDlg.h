#ifndef ChooseDlg_H
#define ChooseDlg_H

#include <QDialog>
#include <QPushButton>

class ChooseDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseDlg(QWidget *parent = 0);
    ~ChooseDlg();

    //创建4个按钮
    QPushButton* _buttons[4];
    //点击的是哪个按钮
    int _selected;
signals:

public slots:
    //将获取到的用户点击的按钮的编号传输给main.cpp，并调用对应函数创建窗口
    void slotClicked();
};

#endif // ChooseDlg_H
