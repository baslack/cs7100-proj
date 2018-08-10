#ifndef PARAMWIDGET_H
#define PARAMWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>

#define DEF_STEPS 10
#define P_MAX_WIDTH 150

class ParamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParamWidget(
            int,
            QString,
            QString,
            double,
            QWidget *parent = 0);
    QString getName(void);
    double getBase(void);
    double getMin(void);
    double getMax(void);
    int getSteps(void);

private:
    QLabel* name;
    QLineEdit* base;
    QLineEdit* min;
    QLineEdit* max;
    QLineEdit* steps;
    QCheckBox* varying;
    QWidget* onState;
    QWidget* offState;
    int idx;
    double last_base;
    double last_min;
    double last_max;

signals:
    void varyingChanged(int, bool);

public slots:
    void varyingDisabled(int);

private slots:
    void switchState(int);
};

#endif // PARAMWIDGET_H
