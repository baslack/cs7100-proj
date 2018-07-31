#include "functionaction.h"

FunctionAction::FunctionAction(int index, QObject* parent):
    QAction(parent),
    index(index)
{
    connect(
        this,
        SIGNAL(triggered(void)),
        this,
        SLOT(onActionSelected(void))
    );
}

void FunctionAction::onActionSelected(){
    emit functionSelected(index);
}
