#include <ctime>
#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <fstream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->points_spinBox->setRange(1000, 1000000);
    ui->points_spinBox->setValue(1000);
    ui->points_spinBox->setSingleStep(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_ch_button_clicked()
{

    //Open file
    ofstream myfile;
    myfile.open("..\\vysledky.txt", ios::app);

    //Write selected info
    QString shape = ui->shape_comboBox->currentText();
    int num_points = ui->points_spinBox->value();
    QString method = ui->method_comboBox->currentText();

    myfile << "Selected shape: " << shape.toLatin1().data() << endl;
    myfile << "Selected number of points: " << num_points << endl;
    myfile << "Selected method: " << method.toLatin1().data() << endl;

    //Assign points to new variable (not to change original set)
    //vector<QPoint> points = points_original;

    QPolygon ch;

    //Generate convex hull
    //Check whether there are points in set
    if(points_original.size() == 0)
        ui->time_label->setText("No points in set");
    else
    {
        for(int i = 0; i < 10; i++)
        {

            vector<QPoint> points = points_original;

            //Start time
            clock_t s = std::clock();

            //Construct convex hull
            if(ui->method_comboBox->currentIndex()==0)
                ch = Algorithms::CHJarvis(points);
            else if(ui->method_comboBox->currentIndex()==1)
                ch = Algorithms::QHull(points);
            else
                ch = Algorithms::CHSweepLine(points);

            //End time
            clock_t e = std::clock();

            //Time difference in miliseconds
            clock_t time = e-s;
            ui->time_label->setText(QString::number(time) + " ms");

            //Write time to file
            myfile << time << ";";
        }

    }

    //Set and repaint
    ui->Canvas->setCH(ch);
    repaint();

    //Close file
    myfile << endl << "-----" << endl;
    myfile.close();
}

void Widget::on_clear_button_clicked()
{
    //Clear canvas and measured time
    ui->Canvas->clearAll();
    ui->time_label->clear();
    ui->set_time_label->clear();
    points_original.clear();
    repaint();
}


void Widget::on_set_button_clicked()
{
    //Generate set of points based on chosen shape and size

    //Start time
    clock_t s1 = std::clock();

    //Clear canvas
    ui->Canvas->clearAll();

    //Get chosen set parameters
    int shape_index = ui->shape_comboBox->currentIndex();
    int num_of_points = ui->points_spinBox->value();

    //Get size of canvas
    int canvas_width = ui->Canvas->width()-5;
    int canvas_height = ui->Canvas->height()-5;

    //Generate set of points
    points_original = ui->Canvas->generateSet(shape_index, num_of_points, canvas_width, canvas_height);

    //End time
    clock_t e1 = std::clock();

    //Time difference in miliseconds
    clock_t time1 = e1-s1;
    ui->set_time_label->setText(QString::number(time1) + " ms");

    repaint();

}

