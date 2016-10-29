#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*===================================
 *
 *      磁盘调度算法演示
 *
 *      @author
 *          何昕  胡斐  李远航 陈博渊
 *      @date
 *          2016.4.3
 *
 *      ALL_RIGHTS_RESERVED
 *
 * ==================================*/

#include <QVector>
#include <QButtonGroup>
#include <QMainWindow>

typedef struct {

    //磁盘调度序列
    QString readSeq;
    //移动柱面距离
    unsigned tolDisc;
    //平均寻道长度
    double averDisc;

}Output;

namespace Ui {
class DiskScheduling;
}

class DiskScheduling : public QMainWindow
{
    Q_OBJECT

public:
    //fcfs磁盘调度
    void fcfsScheduling(const int &startLoc,
                        QVector<int> diskSeq);

    //sstf磁盘调度
    void sstfScheduling(const int &startLoc,
                        QVector<int> diskSeq);

    //scan磁盘调度
    void scanScheduling(const int &startLoc,
                        QVector<int> diskSeq);

    //cscan磁盘调度
    void cscanScheduling(const int &startLoc,
                        QVector<int> diskSeq);

    explicit DiskScheduling(QWidget *parent = 0);
    ~DiskScheduling();

private slots:
    void on_startLineEdit_textChanged(const QString &arg1);

    void on_diskSeqLineEdit_textChanged(const QString &arg1);

    void on_okButton_clicked();

    void on_close_btn_clicked();

private:
    void initUI(const QVector<double> &x, const QVector<double> &y);
    void clearOutput()
    {
        outPut.readSeq.clear();
        outPut.averDisc = 0.0;
        outPut.tolDisc = 0;
    }

    //暂存磁盘请求访问序列
    QVector<int> dataProcess(QString input);

    QButtonGroup *buttonGroup;
    //输出磁盘访问序列
    Output outPut = {"", 0, 0};

    Ui::DiskScheduling *ui;

//    //调用OepnGL绘制结果
//    DrawGL draw_dlg;
};

#endif // MAINWINDOW_H
