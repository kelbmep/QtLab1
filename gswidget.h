#ifndef GSWIDGET_H
#define GSWIDGET_H

#include <QOpenGLWidget>
#include <QPainter>

class Field;

class GSWidget : public QOpenGLWidget
{
public:
    GSWidget(QWidget *centralwidget);

    void drawPoints();
    void generatePoints(int number);
    void cleanPoints();
    void cleanRec();

    bool mousePressed;
    bool paintStarted;
    bool recExist;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    int h, w;

    QPixmap m_mainPix;
    QPixmap m_recPix;

    QPoint m_prevPos;
    QPainter m_painter;

    QRectF m_rectangle;
    std::vector<QPoint> m_points;
    std::vector<int> m_inRecIds;
    Field* m_field;

};

#endif // GSWIDGET_H
