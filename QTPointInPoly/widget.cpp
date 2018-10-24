#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QFileDialog>
#include <vector>

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

void Widget::on_analyze_button_clicked()
{
    //Vector of polygons which contain point q or is on their boundary
    std::vector<std::vector<QPoint>> poly_fill;

    //Final result for writing output after all polygons are analyzed
    int final_res = 0;

    //Get number of polygons
    int n = ui->canvas->getPolygonCount();

    //Analyze point and polygon position
    QPoint q = ui->canvas->getPoint();

    //Ray algorithm selected
    if(ui->method_combobox->currentIndex())
    {
        //Go through all polygons from file
        for(int i = 0; i < n; i++)
        {
            //Get one polygon
            std::vector<QPoint> pol = ui->canvas->getPolygon(i);

            //Get relation of point and polygon
            int res = Algorithms::getPositionRay(q, pol);

            //Quit if point inside of polygon is found
            if(res == 1)
            {
                final_res = 1;
                poly_fill.push_back(pol);
                break;
            }

            //Append polygon to polygons to fill
            if(res == -1)
            {
                final_res = -1;
                poly_fill.push_back(pol);
            }
        }

        //Write result of analysis
        writeResult(final_res);

        //Fill polygons containing point q
        ui->canvas->fillPolygon(poly_fill);
    }

    //Winding algorithm selected
    else
    {
        //Go through all polygons from file
        for(int i = 0; i < n; i++)
        {
            //Get one polygon
            std::vector<QPoint> pol = ui->canvas->getPolygon(i);

            //Get relation of point and polygon
            int res = Algorithms::getPositionWinding(q, pol);

            //Quit if point inside of polygon is found
            if(res == 1)
            {
                final_res = 1;
                poly_fill.push_back(pol);
                break;
            }

            //Append polygon to polygons to fill
            if(res == -1)
            {
                final_res = -1;
                poly_fill.push_back(pol);
            }
        }

        //Write result of analysis
        writeResult(final_res);

        //Fill polygons containing point q
        ui->canvas->fillPolygon(poly_fill);
    }

    poly_fill.clear();
}

void Widget::writeResult(int res)
{
    //Write result of analysis to window

    //Point q inside P
    if(res == 1)
        ui->analyze_label->setText("Point is inside of polygon.");

    //Point q outside P
    else if (res == 0)
        ui->analyze_label->setText("Point is outside of polygon.");

    //Point q on the boundary
    else
        ui->analyze_label->setText("Point is on the boundary.");
}

void Widget::on_clear_button_clicked()
{
    //ui je globální něco, co umožňuje přistupovat ke všem grafickým komponentám
    ui->canvas->clearCanvas();
    ui->analyze_label->clear();
    ui->load_label->clear();
    ui->load_line->clear();
}
