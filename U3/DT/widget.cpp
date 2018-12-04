#include "widget.h"
#include "ui_widget.h"

#include <vector>
#include <fstream>
#include <QtGui>
#include <QFileDialog>
#include "triangle.h"


//#include "edge.h"
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

void Widget::on_delaunay_button_clicked()
{
    std::vector<QPoint3D> points = ui->Canvas->getPoints();
    /*
    std::ofstream f ("test.fxt");
    for(QPoint3D p: points)
    {
        f << p.x() << "  " << p.y() <<'\n';
    }
    f.close();
    */
    std::vector<Edge> dt = Algorithms::delaunayTriangulation(points);
    ui->Canvas->setDT(dt);
    repaint();
}


void Widget::on_clear_button_clicked()
{
    //Clear and repaint
    ui->Canvas->clearDT();
    repaint();
}

void Widget::on_contours_button_clicked()
{
    //Create contour lines
    std::vector<Edge> dt = ui->Canvas->getDT();
    std::vector<Edge> contours = Algorithms::createContours(dt, 0, 100, 5);
    ui->Canvas->setContours(contours);

    repaint();
}

void Widget::on_dtm_button_clicked()
{
    //Analyze slope and aspect
    std::vector<Edge> dt = ui->Canvas->getDT();
    std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);

    ui->Canvas->setDTM(dtm);

    repaint();
}

void Widget::on_load_button_clicked()
{
    //Get size of canvas
    int canvas_width = ui->Canvas->width()-20;
    int canvas_height = ui->Canvas->height()-20;

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

    //Load points to vector
    QString load_message = ui->Canvas->loadDTM(poly_path_char, canvas_width, canvas_height);

    //Write load message
    ui->load_label->setText(load_message);
}
