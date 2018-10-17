#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>

class Draw : public QWidget
{
    Q_OBJECT

private:
    bool draw_point;
    QPoint q;
    std::vector<QPoint> pol;

public:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void clearCanvas();
    void setDrawPoint();
    explicit Draw(QWidget *parent = nullptr);


signals:

public slots:
};

#endif // DRAW_H
