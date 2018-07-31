#ifndef ADAPTORINTERFACE_H
#define ADAPTORINTERFACE_H

#include <QVector>
#include <QString>
#include <QtPlugin>

enum {
    SUCCESS,
    INVALID_INDEX,
    INVALID_NAME
};

class AdaptorInterface
{
public:
    // needed destructor
    // reminder, this function can't be pure virtual
    // the symbol needs to be defined
    // defining it here in the header/interface is
    // sufficient - B.S.
    virtual ~AdaptorInterface(){}
    //function caller
    virtual int call(const QVector<double>&, double&) = 0;
    //getters
    virtual QString getName(void) = 0;
    virtual QString getDescription(void) = 0;
    virtual const QVector<QString>& getParamNames() = 0;
    virtual const QVector<QString>& getParamTooltips() = 0;
    virtual const QVector<double>& getDefaults() = 0;
    //setters
    virtual void setName(QString) = 0;
    virtual void setDescription(QString) = 0;
    //param management
    virtual int numParams(void) = 0;
    virtual int addParam(QString, QString, double) = 0;
    virtual int removeParamByIndex(int) = 0;
    virtual int removeParamByName(QString) = 0;
};

#define AdaptorInterface_iid "edu.wmich.cs.7100.slackba.proj.AdaptorInterface.v.000"

Q_DECLARE_INTERFACE(AdaptorInterface, AdaptorInterface_iid)

#endif // ADAPTORINTERFACE_H
