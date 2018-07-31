#ifndef ADAPTOR_H
#define ADAPTOR_H
#include <iostream>
#include <QString>
#include <QVector>
#include <vector>
#include <string>

enum retcodes {
    SUCCESS,
    INVALID_INDEX,
    INVALID_NAME
};


/*
 * Adaptor Base Class for
 * implementing function adaptors
*/

class Adaptor {
  public:
    Adaptor(QString, QString);
    virtual ~Adaptor(void);
    virtual int call(QVector<double>&, double&) = 0;
    // getters
    QString getName();
    QString getDescription();
    const QVector<QString>& getParamNames();
    const QVector<QString>& getParamTooltips();
    const QVector<double>& getDefaults();
    // setters
    void setName(QString);
    void setDescription(QString);
    // param management
    int NumParams(void);
    int addParam(QString, QString, double);
    int removeParamByIndex(int);
    int removeParamByName(QString);
  private:
    int (*fn);
    QString name;
    QString description;
    QVector<QString> param_names;
    QVector<QString> param_tooltips;
    QVector<double> param_defaults;
};

#endif // ADAPTOR_H
