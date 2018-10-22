#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

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

void Widget::on_pushButton_3_clicked()
{
    //ui je globální něco, co umožňuje přistupovat ke všem grafickým komponentám
    ui->canvas->clearCanvas();
}

void Widget::on_pushButton_clicked()
{
    ui->canvas->setDrawPoint();
}

void Widget::on_pushButton_2_clicked()
{
    //Analyze point and polygon position
    QPoint q = ui->canvas->getQ();
    std::vector<QPoint> pol = ui->canvas->getPolygon();
    int res = algorithms::getPositionRay(q, pol);
    if(res == 1)
        ui->label_2->setText("Inside");
    else
        ui->label_2->setText("Outside");
}
