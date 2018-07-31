#include "paramwidget.h"

ParamWidget::ParamWidget(
        int index,
        QString name,
        QString tooltip,
        double defaultval,
        QWidget *parent)
    :
      QWidget(parent)
{
    //param index
    this->idx = index;
    // field setup
    QDoubleValidator* d_valid = new QDoubleValidator;
    QIntValidator* i_valid = new QIntValidator;
    // label
    this->name = new QLabel(name, this);
    this->name->setToolTip(tooltip);
    // static param
    base = new QLineEdit(QString::number(defaultval), this);
    base->setValidator(d_valid);
    // varying param
    min = new QLineEdit(QString::number(defaultval), this);
    min->setValidator(d_valid);
    max = new QLineEdit(QString::number(defaultval), this);
    max->setValidator(d_valid);
    steps = new QLineEdit(QString::number(DEF_STEPS), this);
    steps->setValidator(i_valid);
    // varying checkbox
    varying = new QCheckBox(this);
    varying->setTristate(false);
    varying->setCheckState(Qt::CheckState::Unchecked);
    //layouts
    QHBoxLayout* onLayout = new QHBoxLayout(this);
    onLayout->setContentsMargins(0,0,0,0);
    onLayout->addWidget(min);
    onLayout->addWidget(max);
    onLayout->addWidget(steps);
    onState = new QWidget(this);
    onState->setLayout(onLayout);
    onState->setVisible(false);
    QHBoxLayout* offLayout = new QHBoxLayout(this);
    offLayout->setContentsMargins(0,0,0,0);
    offLayout->addWidget(base);
    offState = new QWidget(this);
    offState->setLayout(offLayout);
    QHBoxLayout* controlsLayout = new QHBoxLayout(this);
    controlsLayout->setContentsMargins(0,0,0,0);
    controlsLayout->addWidget(offState);
    controlsLayout->addWidget(onState);
    controlsLayout->addWidget(varying);
    QWidget* controlsWidget = new QWidget(this);
    controlsWidget->setLayout(controlsLayout);
    // wrapper
    QVBoxLayout* wrapperLayout = new QVBoxLayout(this);
    wrapperLayout->setContentsMargins(0,0,0,0);
    wrapperLayout->addWidget(this->name);
    wrapperLayout->addWidget(controlsWidget);
    this->setLayout(wrapperLayout);
    connect(
        varying,
        SIGNAL(stateChanged(int)),
        this,
        SLOT(switchState(int))
    );
}

void ParamWidget::switchState(int state){

    if (varying->checkState() == Qt::CheckState::Unchecked){
        onState->hide();
        offState->show();
        emit varyingChanged(idx, false);
    }else{
        onState->show();
        offState->hide();
        emit varyingChanged(idx, true);
    }
}

void ParamWidget::varyingDisabled(int disabled_index){
    if (disabled_index == idx){
        varying->setCheckState(Qt::CheckState::Unchecked);
    }
}
