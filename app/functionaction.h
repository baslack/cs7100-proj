#ifndef FUNCTIONACTION_H
#define FUNCTIONACTION_H

#include <QObject>
#include <QAction>

class FunctionAction :
    public QAction {
    Q_OBJECT
  public:
    FunctionAction(int, QObject* parent = 0);
  signals:
    void functionSelected(int);
  public slots:
    void onActionSelected(void);
  private:
    int index;
};

#endif // FUNCTIONACTION_H
