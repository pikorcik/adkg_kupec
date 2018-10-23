#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QFileDialog>

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
    //Get path to current directory
    QDir cur_path = QDir::currentPath();
    QString dir_path = cur_path.path();

    //Open file dialog and select path to polygon file
    QString poly_path = QFileDialog::getOpenFileName(
                this,
                tr("Select file to open"),
                dir_path,
                "Text file (*.txt)");

    //Write selected path to UI
    ui->load_line->setText(poly_path);

    //Convert path from QString to char* to use in ifstream
    const char* poly_path_char = poly_path.toLatin1().data();

    //Load polygons to vector
    QString load_message = ui->canvas->loadPolygon(poly_path_char);

    //Write load message
    ui->load_label->setText(load_message);
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
    ui->load_label->clear();
}
