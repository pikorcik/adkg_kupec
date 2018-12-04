#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_delaunay_button_clicked();

    void on_clear_button_clicked();

    void on_contours_button_clicked();

    void on_dtm_button_clicked();

    void on_load_button_clicked();

private:
    Ui::Widget *ui;
    double z_min, z_max;
};

#endif // WIDGET_H
