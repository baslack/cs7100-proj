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
    base->setToolTip("Base value of a static parameter.");
    base->setValidator(d_valid);
    // varying param
    QLabel* min_lb = new QLabel("min:", this);
    min = new QLineEdit(QString::number(defaultval), this);
    min->setToolTip("Minimum value of a varying parameter.");
    min->setValidator(d_valid);
    QLabel* max_lb = new QLabel("max:", this);
    max = new QLineEdit(QString::number(defaultval), this);
    max->setToolTip("Maximum value of varying parameter.");
    max->setValidator(d_valid);
    QLabel* steps_lb = new QLabel("steps:", this);
    steps = new QLineEdit(QString::number(DEF_STEPS), this);
    steps->setToolTip("Number of steps for varying parameter in uniform tesselation.");
    steps->setValidator(i_valid);
    // varying checkbox
    varying = new QCheckBox(this);
    varying->setTristate(false);
    varying->setCheckState(Qt::CheckState::Unchecked);
    varying->setToolTip("Toggles static/varying parameter.");
    //layouts
    //min
    QVBoxLayout* min_chunk = new QVBoxLayout();
    min_chunk->setContentsMargins(0,0,0,0);
    min_chunk->addWidget(min_lb);
    min_chunk->addWidget(min);
    QWidget* min_wrapper = new QWidget(this);
    min_wrapper->setLayout(min_chunk);
    //max
    QVBoxLayout* max_chunk = new QVBoxLayout();
    max_chunk->setContentsMargins(0,0,0,0);
    max_chunk->addWidget(max_lb);
    max_chunk->addWidget(max);
    QWidget* max_wrapper = new QWidget(this);
    max_wrapper->setLayout(max_chunk);
    //steps
    QVBoxLayout* steps_chunk = new QVBoxLayout();
    steps_chunk->setContentsMargins(0,0,0,0);
    steps_chunk->addWidget(steps_lb);
    steps_chunk->addWidget(steps);
    QWidget* steps_wrapper = new QWidget(this);
    steps_wrapper->setLayout(steps_chunk);
    //On state visible controls
    QHBoxLayout* onLayout = new QHBoxLayout(this);
    onLayout->setContentsMargins(0,0,0,0);
    onLayout->addWidget(min_wrapper);
    onLayout->addWidget(max_wrapper);
    onLayout->addWidget(steps_wrapper);
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

QString ParamWidget::getName(){
    return this->name->text();
}

double ParamWidget::getBase(){
    return this->base->text().toDouble();
}

double ParamWidget::getMin(){
    return this->min->text().toDouble();
}

double ParamWidget::getMax(){
    return this->max->text().toDouble();
}

int ParamWidget::getSteps(){
    return this->steps->text().toInt();
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
