#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("/home/chihyuan/ZenboNurseHelper1/ZenboFacialExpression.jpeg");
    ui->label_FacialExpressions->setPixmap(pix.scaled(701,541,Qt::KeepAspectRatio));
//        ui->label_FacialExpressions->setPixmap(pix);

    QStandardItemModel* ItemModel = new QStandardItemModel(this);
    QStringList strList;

    strList.append("ACTIVE");
    strList.append("AWARE_LEFT");
    strList.append("AWARE_RIGHT");
    strList.append("CONFIDENT");
    strList.append("DEFAULT");
    strList.append("DEFAULT_STILL");
    strList.append("DOUBTING");
    strList.append("EXPECTING");
    strList.append("HAPPY");
    strList.append("HELPLESS");
    strList.append("HIDEFACE");
    strList.append("IMPATIENT");
    strList.append("INNOCENT");
    strList.append("INTERESTED");
    strList.append("LAZY");
    strList.append("PLEASED");
    strList.append("PRETENDING");
    strList.append("PROUD");
    strList.append("QUESTIONING");
    strList.append("SERIOUS");
    strList.append("SHOCKED");
    strList.append("SHY");
    strList.append("SINGING");
    strList.append("TIRED");
    strList.append("WORRIED");


    int nCount = strList.size();
    for(int i = 0; i < nCount; i++)
    {
        QString string = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        ItemModel->appendRow(item);
    }
    ui->listView_FacialExpressions->setModel(ItemModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
