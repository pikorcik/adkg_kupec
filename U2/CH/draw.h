#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint> points;
    QPolygon ch;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void setCH( QPolygon ch_) {ch = ch_;}
    std::vector<QPoint> generateSet(int shape_index, int num_of_points, int canvas_width, int canvas_height);
    void clearAll();

signals:

public slots:
};

#endif // DRAW_H
