#ifndef ADAPTORPLUGIN_H
#define ADAPTORPLUGIN_H

//#include <QObject>
//#include <QVector>
#include <adaptorinterface.h>

class AdaptorPlugin :
//    public virtual QObject,
    public AdaptorInterface

{
//    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "edu.wmich.cs.7100.slackba.proj.AdaptorInterface.v.000")
//    Q_INTERFACES(AdaptorInterface)
  public:
//    explicit AdaptorPlugin(QObject* parent = 0);
    explicit AdaptorPlugin(QString, QString);
//    ~AdaptorPlugin(void);
    //getters
    QString getName(void);
    QString getDescription(void);
    const QVector<QString>& getParamNames(void);
    const QVector<QString>& getParamTooltips(void);
    const QVector<double>& getDefaults(void);
    //setters
    void setName(QString);
    void setDescription(QString);
    //param management
    int numParams(void);
    int addParam(QString, QString, double);
    int removeParamByIndex(int);
    int removeParamByName(QString);
  protected:
    //data members
    QString name;
    QString description;
    QVector<QString> param_names;
    QVector<QString> param_tooltips;
    QVector<double> param_defaults;
};

#endif // ADAPTORPLUGIN_H
