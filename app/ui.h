#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVector>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QString>
#include <QVector>
#include <QtPlugin>
#include "gl.h"
#include "paramwidget.h"
#include "adaptorinterface.h"
#include "functionaction.h"
#include "geometry_interface.h"
#include "mesh.h"

class ui : public QMainWindow {
    Q_OBJECT
  public:
    explicit ui(QWidget* parent = Q_NULLPTR);
//    ~ui(void);
    void setup(void);

  signals:
    void adaptorChanged(void);
    void varyingParamDisabled(int);

  private slots:
    void setupLeft(void);
    void setupRight(void);
    void updateVaryingAdaptorIndexes(int, bool);
    void setCurrentAdaptor(int);
    void onAdaptorChanged(void);

  public slots:
    void onVisualize(void);
    void onAdaptiveVisualize(void);
    void onMeshDataReady(void);
    void onCenteringChanged(void);
    void onRangeScalingChanged(void);

  private:
    // adaptors
    bool loadPlugin(void);
    AdaptorInterface* currentAdaptor = Q_NULLPTR;
    QVector<AdaptorInterface*> adaptors;
    QVector<int> varying_adaptor_indexes;

    // menu
    QVector<FunctionAction*> adaptor_actions;
    QMenu* function_menu;
    void createMenus(void);
    void createActions(void);

    // new left hand members
    QWidget* lwrapper = Q_NULLPTR;
    QVector<ParamWidget*> params;

    // new right hand members
    QWidget* rwrapper = Q_NULLPTR;
    GL* viewport;
    QTextEdit* log;

    //display geometry
    Geometry_Interface* m_geo = Q_NULLPTR;
};

#endif // UI_H
