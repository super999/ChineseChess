#include <QApplication>
#include "ChooseDlg.h"
#include "MainWnd.h"
#include <QTime>
int main(int argc, char* argv[])
{
    /*  进入windows消息循环   */
    QApplication app(argc, argv);

    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    /*  创建选择对话框，选择对战模式  */
    ChooseDlg dlg;
    /*  设置窗口拥有固定大小  */
    dlg.setFixedSize(200, 200);
    /*  设置窗口标题  */
    dlg.setWindowTitle(QStringLiteral("中国象棋 - V 1.0 - 模式选择"));
    if(dlg.exec() != QDialog::Accepted)
    {
        return 0;
    }

    /*  根据用户选择的结果创建游戏窗口  */
    MainWnd wnd(dlg._selected);
    /*  显示窗口  */
    wnd.show();
    /*  设置窗口标题  */
    wnd.setWindowTitle(QStringLiteral("中国象棋 - V 1.0"));
    /*  设置窗口拥有固定大小  */
    wnd.setFixedSize(480, 440);
    /*  设置窗口背景颜色为黑  */
    wnd.setAutoFillBackground(true);
    QPalette p;
    p.setColor(QPalette::Background,QColor(0, 0, 0));
    wnd.setPalette(p);

    /*  退出程序    */
    return app.exec();
}
