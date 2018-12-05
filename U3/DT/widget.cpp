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

    //Set initial values
    ui->contours_spinbox->setRange(1, 500);
    ui->contours_spinbox->setValue(5);
    ui->contours_spinbox->setSingleStep(5);

    ui->delaunay_button->setEnabled(false);
    ui->contours_button->setEnabled(false);
    ui->slope_button->setEnabled(false);
    ui->aspect_button->setEnabled(false);
    ui->clear_button->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_load_button_clicked()
{
    //Clear previous data
    //ui->Canvas->clearDT();
    on_clear_button_clicked();

    //Get size of canvas
    int canvas_width = ui->Canvas->width()-30;
    int canvas_height = ui->Canvas->height()-30;

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
    QString load_message = ui->Canvas->loadDTM(poly_path_char, canvas_width, canvas_height, z_min, z_max);

    //Write load message
    ui->load_label->setText(load_message);

    //Enable triangulation button
    if(load_message == "File successfully open.")
        ui->delaunay_button->setEnabled(true);
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

    //Calculate Delaunay triangulation
    std::vector<Edge> dt = Algorithms::delaunayTriangulation(points);

    //Analyze slope and aspect
    std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);

    ui->Canvas->setDTM(dtm);
    ui->Canvas->setDT(dt);

    repaint();

    //Enable buttons (contours, slope, aspect, clear)
    ui->contours_button->setEnabled(true);
    ui->slope_button->setEnabled(true);
    ui->aspect_button->setEnabled(true);
    ui->clear_button->setEnabled(true);
}

void Widget::on_contours_button_clicked()
{
    //Create contour lines
    std::vector<Edge> dt = ui->Canvas->getDT();
    std::vector<double> z_contours;

    //Get selected step
    int dz = ui->contours_spinbox->value();

    //Change contour interval to int
    int z_min_int = z_min;
    int z_max_int = z_max;
    z_min_int = z_min_int - z_min_int%dz;
    z_max_int = z_max_int - z_max_int%dz + dz;

    //Create contours
    std::vector<Edge> contours = Algorithms::createContours(dt, z_min_int, z_max_int, dz, z_contours);
    ui->Canvas->setContours(contours, z_contours, dz);

    repaint();
}

void Widget::on_slope_button_clicked()
{
    ui->Canvas->drawSlope();
    repaint();
}

void Widget::on_aspect_button_clicked()
{
    ui->Canvas->drawAspect();
    repaint();
}

void Widget::on_clear_button_clicked()
{
    //Clear and repaint
    ui->Canvas->clearDT();
    repaint();
    ui->load_label->clear();
    ui->load_line->clear();
    ui->delaunay_button->setEnabled(false);
    ui->contours_button->setEnabled(false);
    ui->slope_button->setEnabled(false);
    ui->aspect_button->setEnabled(false);
    ui->clear_button->setEnabled(false);
}
