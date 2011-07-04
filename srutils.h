#ifndef SRUTILS_H
#define SRUTILS_H

#include <QtGui/QMainWindow>
#include "ui_srutils.h"

class SRUtils : public QMainWindow
{
    Q_OBJECT

public:
    SRUtils(QWidget *parent = 0);
    ~SRUtils();

private:
    Ui::SRUtilsClass ui;
};

#endif // SRUTILS_H
