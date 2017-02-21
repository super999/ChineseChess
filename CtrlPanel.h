#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include <QPushButton>

class CtrlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlPanel(QWidget *parent = 0);
    ~CtrlPanel();

    //回退按钮
    QPushButton* _back;

signals:
    //回退
    void sigBack();

public slots:
};

#endif // CTRLPANEL_H
