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

void Widget::on_load_button_clicked()
{

}

void Widget::on_draw_button_clicked()
{
    ui->canvas->setDrawPoint();
}

void Widget::on_analyze_button_clicked()
{
    //Analyze point and polygon position
    QPoint q = ui->canvas->getPoint();
    std::vector<QPoint> pol = ui->canvas->getPolygon();
    int res = Algorithms::getPositionRay(q, pol);
    if(res == 1)
        ui->analyze_label->setText("Inside");
    else
        ui->analyze_label->setText("Outside");
}

void Widget::on_clear_button_clicked()
{
    //ui je globální něco, co umožňuje přistupovat ke všem grafickým komponentám
    ui->canvas->clearCanvas();
    ui->analyze_label->clear();
}
