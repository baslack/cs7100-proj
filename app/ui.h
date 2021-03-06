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
#include "adaptivemesh.h"

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
    void onSave(void);
    void onMaxRegionsChanged(void);
    void onAreaThresholdChanged(void);
    void onAspectLimitChanged(void);

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
    QMenu* file_menu;
    void createMenus(void);
    void createActions(void);

    // new left hand members
    QWidget* lwrapper = Q_NULLPTR;
    QVector<ParamWidget*> params;
    int m_adaptive_max_regions = 512;
    GLfloat m_adaptive_area_threshold = 0.03f;
    GLfloat m_adaptive_aspect_limit= 2.0f;

    // new right hand members
    QWidget* rwrapper = Q_NULLPTR;
    GL* viewport;
    QTextEdit* log;

    //display geometry
    Geometry_Interface* m_geo = Q_NULLPTR;
};

#endif // UI_H
