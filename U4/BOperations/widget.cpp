#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->Canvas->setAB();
}

void Widget::on_pushButton_4_clicked()
{
    ui->Canvas->clearAll();

    repaint();
}

void Widget::on_pushButton_2_clicked()
{

}
