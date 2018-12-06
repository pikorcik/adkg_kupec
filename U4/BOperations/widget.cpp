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

void Widget::on_polygon_button_clicked()
{
    ui->Canvas->setAB();
}

void Widget::on_clear_button_clicked()
{
    ui->Canvas->clearAll();
    repaint();
}
