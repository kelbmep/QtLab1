#include "gswidget.h"
#include "field.h"
#include <QOpenGLFunctions>
#include <QMouseEvent>

GSWidget::GSWidget(QWidget *centralwidget)
    : QOpenGLWidget(centralwidget)
{
    m_field = new Field();

    mousePressed = false;
    paintStarted = false;
    recExist = false;
    m_mainPix = QPixmap(2000, 2000);
    m_mainPix.fill(Qt::white);
}

void GSWidget::mousePressEvent(QMouseEvent *event)
{
    mousePressed = true;

    if(recExist)
        m_prevPos = event->pos();
    else
    {
        m_rectangle.setTopLeft(event->pos());
        m_rectangle.setBottomRight(event->pos());
        m_field->inRect({
                m_rectangle.topLeft().rx(),
                m_rectangle.topLeft().ry(),
                m_rectangle.bottomRight().rx(),
                m_rectangle.bottomRight().ry()},
            m_inRecIds);
    }

    m_recPix = m_mainPix;
    update();
}

void GSWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (mousePressed & !recExist)
        recExist = true;
    else if (!recExist)
        m_rectangle.setBottomRight(event->pos());

    mousePressed = false;
    update();
}

void GSWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(recExist)
    {
        auto vect = event->pos() - m_prevPos;
        m_prevPos = event->pos();

        m_rectangle.setTopLeft(m_rectangle.topLeft() + vect);
        m_rectangle.setBottomRight(m_rectangle.bottomRight() + vect);
    }
    else m_rectangle.setBottomRight(event->pos());

    m_inRecIds.clear();
    if(m_rectangle.topLeft().rx() > m_rectangle.bottomRight().rx() && m_rectangle.topLeft().ry() < m_rectangle.bottomRight().ry())
    {
        m_field->inRect({
                m_rectangle.bottomRight().rx(),
                m_rectangle.topLeft().ry(),
                m_rectangle.topLeft().rx(),
                m_rectangle.bottomRight().ry()},
            m_inRecIds
        );
    }
    else if(m_rectangle.topLeft().rx() < m_rectangle.bottomRight().rx() && m_rectangle.topLeft().ry() > m_rectangle.bottomRight().ry())
    {
        m_field->inRect({
                m_rectangle.topLeft().rx(),
                m_rectangle.bottomRight().ry(),
                m_rectangle.bottomRight().rx(),
                m_rectangle.topLeft().ry()},
            m_inRecIds
        );
    }
    else if(m_rectangle.topLeft().rx() > m_rectangle.bottomRight().rx() && m_rectangle.topLeft().ry() > m_rectangle.bottomRight().ry())
    {
        m_field->inRect({
                m_rectangle.bottomRight().rx(),
                m_rectangle.bottomRight().ry(),
                m_rectangle.topLeft().rx(),
                m_rectangle.topLeft().ry()},
            m_inRecIds
        );
    }
    else
    {
        m_field->inRect({
                m_rectangle.topLeft().rx(),
                m_rectangle.topLeft().ry(),
                m_rectangle.bottomRight().rx(),
                m_rectangle.bottomRight().ry()},
            m_inRecIds
        );
    }
    update();
}

void GSWidget::paintEvent(QPaintEvent */*event*/)
{
    m_painter.begin(this);

    if(mousePressed)
    {
         m_painter.drawPixmap(0, 0, m_recPix);

         m_painter.setPen(QPen(Qt::magenta, 3, Qt::SolidLine, Qt::FlatCap));
         for(size_t i = 0; i < m_inRecIds.size(); ++i)
             m_painter.drawPoint(m_points[m_inRecIds[i]]);

         m_painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::FlatCap));
         m_painter.drawRect(m_rectangle);
         paintStarted = true;
    }
    else if(paintStarted)
    {
        QPainter tempPainter(&m_recPix);

        tempPainter.setPen(QPen(Qt::magenta, 3, Qt::SolidLine, Qt::FlatCap));
        for(size_t i = 0; i < m_inRecIds.size(); ++i)
            tempPainter.drawPoint(m_points[m_inRecIds[i]]);

        tempPainter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::FlatCap));
        tempPainter.drawRect(m_rectangle);
        m_painter.drawPixmap(0, 0, m_recPix);
    }
    else drawPoints();

    m_painter.end();
}

void GSWidget::drawPoints()
{
    QPainter tempPainter(&m_mainPix);
    tempPainter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
    tempPainter.drawPoints(m_points.data(), m_points.size());

    m_painter.begin(this);
    m_painter.drawPixmap(0, 0, m_mainPix);
    m_painter.end();
}

void GSWidget::generatePoints(int number)
{
    cleanPoints();

    m_field->generate(number, w, h);

    m_points.clear();
    m_points.reserve(m_points.size() / 2);

    for(size_t i = 0; i < m_field->m_points.size(); i += 2)
        m_points.push_back(QPoint(
            m_field->m_points[i],
            m_field->m_points[i + 1]
        ));

    m_mainPix = QPixmap(w, h);
    m_mainPix.fill(Qt::white);
    m_recPix = m_mainPix;

    drawPoints();
}

void GSWidget::cleanPoints()
{
    recExist = false;

    auto g = geometry();
    h = g.height();
    w = g.width();

    m_mainPix = QPixmap(w, h);
    m_mainPix.fill(Qt::white);

    m_painter.begin(this);
    m_painter.drawPixmap(0, 0, m_mainPix);
    m_painter.end();

    m_inRecIds.clear();
}

void GSWidget::cleanRec()
{
    recExist = false;

    m_painter.begin(this);
    m_painter.drawPixmap(0, 0, m_mainPix);
    m_painter.end();

    m_inRecIds.clear();
}
