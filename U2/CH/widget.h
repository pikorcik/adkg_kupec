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
    void on_ch_button_clicked();

    void on_clear_button_clicked();

    void on_set_button_clicked();

private:
    Ui::Widget *ui;
    std::vector<QPoint> points_original;
};

#endif // WIDGET_H
