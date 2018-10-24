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
    void writeResult(int res);

private slots:
    void on_load_button_clicked();

    void on_analyze_button_clicked();

    void on_clear_button_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
