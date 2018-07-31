#include "adaptor.h"

Adaptor::Adaptor(
    QString name,
    QString description):
    name(name),
    description(description) {
}

Adaptor::~Adaptor() {
    // required by c++ spec
}

QString Adaptor::getName() {
    return name;
}

QString Adaptor::getDescription() {
    return description;
}

const QVector<QString>& Adaptor::getParamNames() {
    return param_names;
}

const QVector<QString>& Adaptor::getParamTooltips() {
    return param_tooltips;
}

const QVector<double>& Adaptor::getDefaults() {
    return param_defaults;
}

void Adaptor::setName(QString name) {
    this->name = name;
}

void Adaptor::setDescription(QString desc) {
    this->description = desc;
}

int Adaptor::NumParams() {
    return param_names.size();
}

int Adaptor::addParam(QString name,
                      QString tip,
                      double d_val) {
    param_names.push_back(name);
    param_tooltips.push_back(tip);
    param_defaults.push_back(d_val);
    return SUCCESS;
}

int Adaptor::removeParamByIndex(int index) {
    QVector<QString> backup_names = param_names;
    QVector<QString> backup_tips = param_tooltips;
    QVector<double> backup_vals = param_defaults;
    try {
        param_names.erase(param_names.begin() + index);
        param_tooltips.erase(param_tooltips.begin() + index);
        param_defaults.erase(param_defaults.begin() + index);
    } catch (...) {
        param_names = backup_names;
        param_tooltips = backup_tips;
        param_defaults = backup_vals;
        return INVALID_INDEX;
    }
    return SUCCESS;
}

int Adaptor::removeParamByName(QString name){
    QVector<QString> backup_names = param_names;
    QVector<QString> backup_tips = param_tooltips;
    QVector<double> backup_vals = param_defaults;
    int index = 0;
    bool found = false;
    try{
        for (auto iter = param_names.begin(); iter != param_names.end(); iter++){
            if (*iter == name) {
                found = true;
                break;
            }else{
                index++;
            }
        }
        if (found){
            int retval = removeParamByIndex(index);
            if (retval != SUCCESS){
                return INVALID_NAME;
            }
        }else{
            return INVALID_NAME;
        }
    }catch (...){
        param_names = backup_names;
        param_tooltips = backup_tips;
        param_defaults = backup_vals;
        return INVALID_NAME;
    }
    return SUCCESS;
}
