#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->operation_button->setEnabled(false);
    ui->buffer_button->setEnabled(false);
    ui->clear_res_button->setEnabled(false);
    ui->clear_all_button->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_load_button_clicked()
{
    ui->Canvas->setAB();

    //Get path to current directory
    QDir cur_path = QDir::currentPath();
    cur_path.cdUp();
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
    QString load_message = ui->Canvas->loadPolygon(poly_path_char);

    //Write load message
    ui->load_label->setText(load_message);

    //Enable operation button
    if(load_message == "File successfully open.")
        ui->operation_button->setEnabled(true);
    else
        on_clear_all_button_clicked();
}

void Widget::on_operation_button_clicked()
{
    //Perform boolean operation

    TBooleanOperation operation;
    switch(ui->comboBox->currentIndex())
    {
        case 0: operation = INTERSECTION; break;
        case 1: operation = UNION; break;
        case 2: operation = DIFFAB; break;
        case 3: operation = DIFFBA;
    }

    // Get those polygons
    std::vector<QPointFB> A = ui->Canvas->getA();
    std::vector<QPointFB> B = ui->Canvas->getB();

    std::vector<std::vector<QPointFB>> result = Algorithms::BooleanOper(A, B, operation);

    ui->Canvas->setRes(result);

    ui->clear_res_button->setEnabled(true);
    ui->clear_all_button->setEnabled(true);

    repaint();
}

void Widget::on_buffer_button_clicked()
{
    // Create buffer over polygons
    std::vector<QPointFB> A = ui->Canvas->getA();
    std::vector<QPointFB> B = ui->Canvas->getB();

    std::vector<std::vector<QPointFB> > bufferA = Algorithms::polygonOffset(A, 20,  30.0 * M_PI / 180);
    //std::vector<std::vector<QPointFB> > bufferB = Algorithms::polygonOffset(B, 20,  7.0 * M_PI / 180);

    //bufferB.insert(bufferB.end(), bufferA.begin(), bufferA.end());

    ui->Canvas->setBuff(bufferA);

    repaint();
}

void Widget::on_clear_res_button_clicked()
{
    ui->Canvas->clearResults();

    repaint();
}

void Widget::on_clear_all_button_clicked()
{
    ui->Canvas->clearAll();

    ui->operation_button->setEnabled(false);
    ui->clear_res_button->setEnabled(false);
    ui->clear_all_button->setEnabled(false);

    repaint();
}
