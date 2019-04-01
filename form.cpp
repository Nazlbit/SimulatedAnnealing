#include "form.h"
#include <QPainter>
#include <QPen>

Form::Form(QWidget *parent) :
    QWidget(parent),
    model(100, 100)
{
    model.fillRandom();
}

Form::~Form()
{
}

void Form::paintEvent(QPaintEvent *)
{
    QPainter painter{this};
    painter.setPen(Qt::NoPen);

    double w = static_cast<double>(painter.viewport().width())/model.getWidth();
    double h = static_cast<double>(painter.viewport().height())/model.getHeight();

    for(int64_t i=0;i<model.getHeight();i++)
    {
        for(int64_t j=0;j<model.getWidth();j++)
        {
            if(model.valueAt(i, j) == 1)
            {
                painter.setBrush(Qt::yellow);
            }
            else
            {
                painter.setBrush(Qt::blue);
            }
            painter.drawRect(QRectF(j*w, i*h, w, h));
        }
    }

    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::black);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

void Form::setGrid(IsingModel&& gr)
{
    model = std::move(gr);
    update();
}

void Form::setGrid(const IsingModel& gr)
{
    model = gr;
    update();
}

IsingModel Form::getGrid()
{
    return model;
}

