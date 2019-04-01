#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "ising_model.h"

class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    void setGrid(IsingModel&&);
    void setGrid(const IsingModel&);
    IsingModel getGrid();

protected:
    void paintEvent(QPaintEvent *);

private:
    IsingModel model;
};

#endif // FORM_H
