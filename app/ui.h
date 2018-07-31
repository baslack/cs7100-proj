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

class ui : public QMainWindow {
    Q_OBJECT
  public:
    explicit ui(QWidget* parent = Q_NULLPTR);
    void setup(void);

  signals:
    void AdaptorChanged(void);

  private slots:
    void setupLeft(void);
    void setupRight(void);

  public slots:

  private:
    // adaptors
    AdaptorInterface* currentAdaptor = Q_NULLPTR;
    QVector<AdaptorInterface*> adaptors;

    // menu
    QVector<QAction*> adaptor_actions;
    QMenu* function_menu;
    void createMenus(void);
    void createActions(void);


    bool loadPlugin(void);

    // new left hand members
    QWidget* lwrapper = Q_NULLPTR;
    QVector<ParamWidget*> params;

    // new right hand members
    QWidget* rwrapper = Q_NULLPTR;
    GL* viewport;
    QTextEdit* log;
};

#endif // UI_H
