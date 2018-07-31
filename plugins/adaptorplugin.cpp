#include "adaptorplugin.h"

//AdaptorPlugin::AdaptorPlugin(QObject* parent) :
//    QObject(parent) {

//}

//AdaptorPlugin::AdaptorPlugin(QObject* parent, QString name, QString desc):
//    QObject (parent),
//    name(name),
//    description(desc) {

//}

AdaptorPlugin::AdaptorPlugin(QString name, QString desc):
name(name),
description(desc)
{

}

//AdaptorPlugin::~AdaptorPlugin(){

//}

QString AdaptorPlugin::getName(void) {
    return name;
}

QString AdaptorPlugin::getDescription(void) {
    return description;
}

const QVector<QString>& AdaptorPlugin::getParamNames(void) {
    return param_names;
}

const QVector<QString>& AdaptorPlugin::getParamTooltips(void) {
    return param_tooltips;
}

const QVector<double>& AdaptorPlugin::getDefaults(void) {
    return param_defaults;
}

void AdaptorPlugin::setName(QString newname) {
    name = newname;
}

void AdaptorPlugin::setDescription(QString newdesc) {
    description = newdesc;
}

int AdaptorPlugin::numParams(void) {
    return param_names.size();
}

int AdaptorPlugin::addParam(
    QString param_name,
    QString param_tip,
    double d_val) {
    param_names.push_back(param_name);
    param_tooltips.push_back(param_tip);
    param_defaults.push_back(d_val);
    return SUCCESS;
}

int AdaptorPlugin::removeParamByIndex(int index) {
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

int AdaptorPlugin::removeParamByName(QString name) {
    QVector<QString> backup_names = param_names;
    QVector<QString> backup_tips = param_tooltips;
    QVector<double> backup_vals = param_defaults;
    int index = 0;
    bool found = false;
    try {
        for (auto iter = param_names.begin(); iter != param_names.end(); iter++) {
            if (*iter == name) {
                found = true;
                break;
            } else {
                index++;
            }
        }
        if (found) {
            int retval = removeParamByIndex(index);
            if (retval != SUCCESS) {
                return INVALID_NAME;
            }
        } else {
            return INVALID_NAME;
        }
    } catch (...) {
        param_names = backup_names;
        param_tooltips = backup_tips;
        param_defaults = backup_vals;
        return INVALID_NAME;
    }
    return SUCCESS;
}
