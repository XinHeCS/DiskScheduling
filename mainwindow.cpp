#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QMessageBox>
#include <algorithm>

DiskScheduling::DiskScheduling(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiskScheduling)
{

    ui->setupUi(this);
    //设置窗口颜色
//    QPalette p;
//    p.setColor(QPalette::Window, QColor(117, 117, 117));
//    setPalette(p);

    setStyleSheet("background-color: rgb(117, 117, 117);");

    //创建正则表达式
    QRegExp regExp("[0-9]{0,3}");
    ui->startLineEdit->setValidator(new QRegExpValidator(regExp, this));

    ui->diskSeqLineEdit->setReadOnly(true);

    //默认选择FCFS算法
    ui->fcfsRadioButton->click();

    //创建按键组，并为每个按键分配ID
    buttonGroup = new QButtonGroup;
    buttonGroup->addButton(ui->fcfsRadioButton, 1);
    buttonGroup->addButton(ui->sstfRadioButton, 2);
    buttonGroup->addButton(ui->scanRadioButton, 3);
    buttonGroup->addButton(ui->cscanRadioButton, 4);

  // ui->outputGroupBox->hide();
//   /*layout()->SetFixedSize(sizeHint())*/
    //layout()->setSizeConstraint(QLayout::SetFixedSize);

    //使输出文本框只读
    ui->readSeqLineEdit->setReadOnly(true);
    ui->tolDisLineEdit->setReadOnly(true);
    ui->AverDisLineEdit->setReadOnly(true);

}

//析构函数
DiskScheduling::~DiskScheduling()
{
    delete ui;
}

void DiskScheduling::on_startLineEdit_textChanged(const QString &arg1)
{

    ui->diskSeqLineEdit->setReadOnly(ui->startLineEdit->text().isEmpty());

}

void DiskScheduling::on_diskSeqLineEdit_textChanged(const QString &arg1)
{

    ui->okButton->setEnabled(!ui->diskSeqLineEdit->text().isEmpty());

    //动态显示输入的正确性

}


void DiskScheduling::on_okButton_clicked()
{
    //初始化输出
    outPut.readSeq.clear();
    outPut.averDisc = 0.0;
    outPut.tolDisc = 0;
    //QVector<double> _result;

    //开始磁道
    int startLoc = ui->startLineEdit->text().toInt();
    //输入的磁道序列
    QString QDiskSeq = ui->diskSeqLineEdit->text();

    QVector<int> diskSeq = dataProcess(QDiskSeq);
    if (diskSeq.size() == 0)
        return;
//    if (ui->comboBox->currentText() == "None") {
//        //弹出提示窗口
//        QMessageBox warn;
//        QPushButton ok;

//        ok.setStyleSheet("background-color:rgb(128, 195, 69);color:rgb(255,255,255);"
//                         "border-radius: 5px; width: 50px; height: 20px;");
//        ok.setText("&Ok");

//        warn.setWindowTitle("Warning");
//        warn.setText("Please select the direction.");
//        warn.setIcon(QMessageBox::Warning);
//        warn.setStyleSheet("background-color: rgb(117, 117, 117);"
//                           "color: white");
//        warn.addButton(&ok, QMessageBox::ActionRole);

//        warn.exec();
//    }
//    else
//    {
//        fcfsScheduling(startLoc, diskSeq);
//        _result.push_back(outPut.tolDisc);
//        clearOutput();
//        sstfScheduling(startLoc, diskSeq);
//        _result.push_back(outPut.tolDisc);
//        clearOutput();
//        scanScheduling(startLoc, diskSeq);
//        _result.push_back(outPut.tolDisc);
//        clearOutput();
//        cscanScheduling(startLoc, diskSeq);
//        _result.push_back(outPut.tolDisc);
//        clearOutput();

//        initUI(_result);
//    }


    //判断选中了哪个按钮
    switch(buttonGroup->checkedId())
    {

    case 1:
        fcfsScheduling(startLoc, diskSeq);
        break;
    case 2:
        sstfScheduling(startLoc, diskSeq);
        break;
    case 3:
        if (ui->comboBox->currentText() == "None") {
            //弹出提示窗口
            QMessageBox warn;
            QPushButton ok;

            ok.setStyleSheet("background-color:rgb(128, 195, 69);color:rgb(255,255,255);"
                             "border-radius: 5px; width: 50px; height: 20px;");
            ok.setText("&Ok");

            warn.setWindowTitle("Warning");
            warn.setText("Please select the direction.");
            warn.setIcon(QMessageBox::Warning);
            warn.setStyleSheet("background-color: rgb(117, 117, 117);"
                               "color: white");
            warn.addButton(&ok, QMessageBox::ActionRole);

            warn.exec();

            break;
        }
        scanScheduling(startLoc, diskSeq);
        break;
    case 4:
        if (ui->comboBox->currentText() == "None") {
            //弹出提示窗口
            QMessageBox warn;
            QPushButton ok;

            ok.setStyleSheet("background-color:rgb(128, 195, 69);color:rgb(255,255,255);"
                             "border-radius: 5px; width: 50px; height: 20px;");
            ok.setText("&Ok");

            warn.setWindowTitle("Warning");
            warn.setText("Please select the direction.");
            warn.setIcon(QMessageBox::Warning);
            warn.setStyleSheet("background-color: rgb(117, 117, 117);"
                               "color: white");
            warn.addButton(&ok, QMessageBox::ActionRole);

            warn.exec();
            break;
        }
        cscanScheduling(startLoc, diskSeq);
        break;
    default:
        break;
    }

    ui->readSeqLineEdit->setText(outPut.readSeq);
    ui->tolDisLineEdit->setText(QString::number(outPut.tolDisc));
    ui->AverDisLineEdit->setText(QString::number(outPut.averDisc));

}

//输入数据处理
QVector<int> DiskScheduling::dataProcess(QString input) {

    QStringList strList = input.split(' ');

    QVector<QString> diskSeq_QStr;
    for (QStringList::iterator iter = strList.begin();
         iter != strList.end(); ++iter)
        diskSeq_QStr.push_back(*iter);

    QVector<int> diskSeq;
    for (QVector<QString>::iterator iter = diskSeq_QStr.begin();
         iter != diskSeq_QStr.end(); ++iter) {
        int tmp = iter->toInt();
        if (tmp < 1000 && tmp > 0)
            diskSeq.push_back(tmp);
        else {
            QMessageBox warn;
            QPushButton ok;

            ok.setStyleSheet("background-color:rgb(128, 195, 69);color:rgb(255,255,255);"
                             "border-radius: 5px; width: 50px; height: 20px;");
            ok.setText("&Ok");

            warn.setWindowTitle("Warning");
            warn.setText("Please enter correct message.");
            warn.setIcon(QMessageBox::Warning);
            warn.setStyleSheet("background-color: rgb(117, 117, 117);"
                               "color: white");
            warn.addButton(&ok, QMessageBox::ActionRole);

            warn.exec();

            return QVector<int>();
        }
    }

    return diskSeq;

}

//fcfs磁盘调度
void DiskScheduling::fcfsScheduling(const int &startLoc,
                                    QVector<int> diskSeq)
{
    QVector<double> x, y;
    int i = 0;
    outPut.readSeq.append(outPut.readSeq.number(startLoc));

    for (QVector<int>::iterator iter = diskSeq.begin();
         iter != diskSeq.end(); ++iter) {

        outPut.readSeq.append("->");
        outPut.readSeq.append(outPut.readSeq.number(*iter));
        x.push_back(++i);
        y.push_back(*iter);

        if (iter == diskSeq.begin())
            outPut.tolDisc += abs(*iter - startLoc);
        else
            outPut.tolDisc += abs(*iter - *(iter - 1));

    }

    outPut.averDisc = static_cast<double>(outPut.tolDisc)
            / diskSeq.size();

    initUI(x, y);
}

//sstf磁盘调度
void DiskScheduling::sstfScheduling(const int &startLoc,
                                    QVector<int> diskSeq)
{
    QVector<double> x, y;
    int i = 0;
    outPut.readSeq.append(outPut.readSeq.number(startLoc));

    //从小到大对磁盘访问序列进行排序
    qSort(diskSeq.begin(), diskSeq.end());

    //找出距离起始位置最近的磁道的位置
    int min = 1000;
    QVector<int>::iterator mid_iter;
    for (QVector<int>::iterator iter = diskSeq.begin();
         iter != diskSeq.end(); ++iter)
        if (abs(startLoc - *iter) < min) {
            min = startLoc - *iter;
            mid_iter = iter;
        }
        else
            break;

    //输出第一次移动的过程
    outPut.readSeq.append("->");
    outPut.readSeq.append(QString::number(*mid_iter));
    x.push_back(++i);
    y.push_back(*mid_iter);
    //计算第一次移动的距离
    outPut.tolDisc += abs(startLoc - *mid_iter);

    //是否到达左右两边尽头
    bool leftTag = false, rightTag = false;

    //确定左右迭代器所指向的位置
    QVector<int>::iterator left_iter, right_iter;
    left_iter = right_iter = mid_iter;
    if (left_iter != diskSeq.begin())
        left_iter -= 1;
    else leftTag = true;

    if (right_iter != (diskSeq.end() - 1))
        right_iter += 1;
    else rightTag = true;

    while (!leftTag || !rightTag) {
        //判断左右两边，那个距离mid_iter较近
        //左边到尽头了
        if (leftTag) {
            outPut.tolDisc += abs(*right_iter - *mid_iter);
            mid_iter = right_iter;
            if (right_iter != (diskSeq.end() - 1))
                right_iter += 1;
            else rightTag = true;
        }
        //右边到尽头了
        else if (rightTag) {
            outPut.tolDisc += abs(*left_iter - *mid_iter);
            mid_iter = left_iter;
            if (left_iter != diskSeq.begin())
                left_iter -= 1;
            else leftTag = true;
        }
        //左右两边都没到尽头
        else if (!leftTag && !rightTag)
            if (abs(*left_iter - *mid_iter) <
                    abs(*right_iter - *mid_iter)) {
                outPut.tolDisc += abs(*left_iter - *mid_iter);
                mid_iter = left_iter;
                if (left_iter != diskSeq.begin())
                    left_iter -= 1;
                else leftTag = true;
            }
            else {
                outPut.tolDisc += abs(*right_iter - *mid_iter);
                mid_iter = right_iter;
                if (right_iter != (diskSeq.end() - 1))
                    right_iter += 1;
                else rightTag = true;
            }

        outPut.readSeq.append("->");
        outPut.readSeq.append(QString::number(*mid_iter));
        x.push_back(++i);
        y.push_back(*mid_iter);
    }

    outPut.averDisc = static_cast<double>(outPut.tolDisc)
            / diskSeq.size();
    initUI(x, y);
}

//scan磁盘调度
void DiskScheduling::scanScheduling(const int &startLoc,
                                    QVector<int> diskSeq)
{
    QVector<double> x, y;
    int i = 0;
    outPut.readSeq.append(outPut.readSeq.number(startLoc));

    //从小到大对磁盘访问序列进行排序
    qSort(diskSeq.begin(), diskSeq.end());

    //是否到达左右两边尽头
    bool leftTag = false, rightTag = false;

    //找出距离磁头移动方向最近的磁道
    QVector<int>::iterator left_iter, right_iter;
    for (QVector<int>::iterator iter = diskSeq.begin(); ; ++iter)
        if (startLoc <= *iter) {

            if (startLoc < *iter)
                right_iter = iter;
            else if (startLoc == *iter)
                if (ui->comboBox->currentText() == "up")
                    right_iter = iter;
                else if (ui->comboBox->currentText() == "down")
                    right_iter = iter + 1;

            if (right_iter == diskSeq.begin())
                leftTag = true;
            else left_iter = right_iter - 1;

            break;
        }
        else if (iter == diskSeq.end()) {
            left_iter = diskSeq.end() - 1;
            rightTag = true;
            break;
        }

    if (leftTag)
        while (right_iter != diskSeq.end()) {

            if (right_iter == diskSeq.begin())
                outPut.tolDisc += abs(*right_iter - startLoc);
            else
                outPut.tolDisc += abs(*right_iter - *(right_iter - 1));

            outPut.readSeq.append("->");
            outPut.readSeq.append(QString::number(*right_iter));
            x.push_back(++i);
            y.push_back(*right_iter);

            right_iter++;

        }
    else if (rightTag)
        while (left_iter != (diskSeq.begin() - 1)) {

            if (left_iter == (diskSeq.end() - 1))
                outPut.tolDisc += abs(*left_iter - startLoc);
            else
                outPut.tolDisc += abs(*left_iter - *(left_iter + 1));

            outPut.readSeq.append("->");
            outPut.readSeq.append(QString::number(*left_iter));
            x.push_back(++i);
            y.push_back(*left_iter);

            left_iter--;

        }
    else {

        if (ui->comboBox->currentText() == "up") {

            for (QVector<int>::iterator iter = right_iter;
                 iter != diskSeq.end(); ++iter) {

                if (iter == right_iter)
                    outPut.tolDisc += abs(*iter - startLoc);
                else
                    outPut.tolDisc += abs(*iter - *(iter - 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);

            }

            for (QVector<int>::iterator iter = left_iter;
                 iter != (diskSeq.begin() - 1); --iter) {

                if (iter == left_iter)
                    outPut.tolDisc += abs(*iter - *(diskSeq.end() - 1));
                else
                    outPut.tolDisc += abs(*iter - *(iter + 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);

            }

        }

        else if (ui->comboBox->currentText() == "down") {

            for (QVector<int>::iterator iter = left_iter;
                 iter != (diskSeq.begin() - 1); --iter) {

                if (iter == left_iter)
                    outPut.tolDisc += abs(*iter - startLoc);
                else
                    outPut.tolDisc += abs(*iter - *(iter + 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);
            }

            for (QVector<int>::iterator iter = right_iter;
                 iter != diskSeq.end(); ++iter) {

                if (iter == right_iter)
                    outPut.tolDisc += abs(*iter - *diskSeq.begin());
                else
                    outPut.tolDisc += abs(*iter - *(iter - 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);
            }

        }

    }

    outPut.averDisc = static_cast<double>(outPut.tolDisc)
            / diskSeq.size();
    initUI(x, y);
}

//cscan磁盘调度
void DiskScheduling::cscanScheduling(const int &startLoc,
                                     QVector<int> diskSeq)
{
    QVector<double> x, y;
    int i = 0;
    outPut.readSeq.append(outPut.readSeq.number(startLoc));

    //从小到大对磁盘访问序列进行排序
    qSort(diskSeq.begin(), diskSeq.end());

    //是否到达左右两边尽头
    bool leftTag = false, rightTag = false;

    //找出距离磁头移动方向最近的磁道
    QVector<int>::iterator left_iter, right_iter;
    for (QVector<int>::iterator iter = diskSeq.begin(); ; ++iter)
        if (startLoc <= *iter) {

            if (startLoc < *iter)
                right_iter = iter;
            else if (startLoc == *iter)
                if (ui->comboBox->currentText() == "up")
                    right_iter = iter;
                else if (ui->comboBox->currentText() == "down")
                    right_iter = iter + 1;

            if (right_iter == diskSeq.begin())
                leftTag = true;
            else left_iter = right_iter - 1;
            break;
        }
        else if (iter == diskSeq.end()) {
            left_iter = diskSeq.end() - 1;
            rightTag = true;
            break;
        }

    if (leftTag)
        while (right_iter != diskSeq.end()) {

            if (right_iter == diskSeq.begin())
                outPut.tolDisc += abs(*right_iter - startLoc);
            else
                outPut.tolDisc += abs(*right_iter - *(right_iter - 1));

            outPut.readSeq.append("->");
            outPut.readSeq.append(QString::number(*right_iter));
            x.push_back(++i);
            y.push_back(*right_iter);


            right_iter++;

        }
    else if (rightTag)
        while (left_iter != (diskSeq.begin() - 1)) {

            if (left_iter == (diskSeq.end() - 1))
                outPut.tolDisc += abs(*left_iter - startLoc);
            else
                outPut.tolDisc += abs(*left_iter - *(left_iter + 1));

            outPut.readSeq.append("->");
            outPut.readSeq.append(QString::number(*left_iter));
            x.push_back(++i);
            y.push_back(*left_iter);


            left_iter--;

        }
    else {

        if (ui->comboBox->currentText() == "up") {

            for (QVector<int>::iterator iter = right_iter;
                 iter != diskSeq.end(); ++iter) {

                if (iter == right_iter)
                    outPut.tolDisc += abs(*iter - startLoc);
                else
                    outPut.tolDisc += abs(*iter - *(iter - 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);
            }

            for (QVector<int>::iterator iter = diskSeq.begin();
                 iter != right_iter; ++iter) {

                if (iter == diskSeq.begin())
                    outPut.tolDisc += abs(*iter - *(diskSeq.end() - 1));
                else
                    outPut.tolDisc += abs(*iter - *(iter - 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);
            }

        }

        else if (ui->comboBox->currentText() == "down") {

            for (QVector<int>::iterator iter = left_iter;
                 iter != (diskSeq.begin() - 1); --iter) {

                if (iter == left_iter)
                    outPut.tolDisc += abs(*iter - startLoc);
                else
                    outPut.tolDisc += abs(*iter - *(iter + 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);
            }

            for (QVector<int>::iterator iter = (diskSeq.end() - 1);
                 iter != left_iter; --iter) {

                if (iter == (diskSeq.end() - 1))
                    outPut.tolDisc += abs(*iter - *diskSeq.begin());
                else
                    outPut.tolDisc += abs(*iter - *(iter + 1));

                outPut.readSeq.append("->");
                outPut.readSeq.append(QString::number(*iter));
                x.push_back(++i);
                y.push_back(*iter);
            }

        }

    }

    outPut.averDisc = static_cast<double>(outPut.tolDisc)
            / diskSeq.size();
    initUI(x, y);
}

void DiskScheduling::on_close_btn_clicked()
{
    close();
}

void DiskScheduling::initUI(const QVector<double> &x, const QVector<double> &y)
{
    QVector<double> tmp(y);
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);

    ui->widget->xAxis->setLabel(QStringLiteral("移动次数"));
    ui->widget->yAxis->setLabel(QStringLiteral("访问磁道"));
    ui->widget->xAxis->setRange(x.first(), x.last());
    std::sort(tmp.begin(), tmp.end());
    ui->widget->yAxis->setRange(tmp.first(), tmp.last());

//    ui->widget->xAxis->setAutoTicks(false);
//    ui->widget->xAxis->setAutoTickLabels(false);
//    ui->widget->xAxis->setAutoTickStep(false);

//    ui->widget->addPlottable(bars);
//    ui->widget->rescaleAxes();

//    double wid=ui->widget->xAxis->range().upper-ui->widget->xAxis->range().lower;
//    double cl=bars->width()+(1.0*wid-bars->width()*4)/3;

//    QVector<double> coor;
//    for(int i=0;i<4;++i)
//        coor.append(ui->widget->xAxis->range().lower+i*cl+bars->width()/2);
//    ui->widget->xAxis->setTickVector(coor);
//    ui->widget->xAxis->setTickVectorLabels(labels);
    ui->widget->replot();
}
