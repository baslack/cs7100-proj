#include "ui.h"
#include <QApplication>
#include <QPluginLoader>
#include <QSize>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QDir>
#include <QDebug>

#define TEMPSTATIC 3
#define FIELD_MAXWIDTH 150
#define LOG_MAXHEIGHT 150
#define VIEW_MINWIDTH 640
#define LEFT_WIDTH 200
#define WINDOW_DEFAULT_HEIGHT 768
#define WINDOW_DEFAULT_WIDTH 1024

ui::ui(QWidget* parent):
    QMainWindow(parent) {
    connect(
        this,
        SIGNAL(adaptorChanged()),
        this,
        SLOT(onAdaptorChanged())
    );
}

bool ui::loadPlugin() {
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {

        QString filePath = pluginsDir.absoluteFilePath(fileName);
        QPluginLoader pluginLoader(filePath);
        QObject* plugin = pluginLoader.instance();
        QString message = pluginLoader.errorString();
        //        qDebug() << message;
        if (plugin) {
            currentAdaptor = qobject_cast<AdaptorInterface*>(plugin);
            if (currentAdaptor)
                adaptors.push_back(currentAdaptor);
            //                return true;
        }
    }
    if (adaptors.size() != 0) {
        return true;
    } else {
        return false;
    }
}

void ui::createActions() {
    int index = 0;
    for (QVector<AdaptorInterface*>::iterator iter = adaptors.begin();
            iter != adaptors.end();
            iter++) {
        FunctionAction* this_action = new FunctionAction(index, this);
        this_action->setText((*iter)->getName());
        this_action->setToolTip((*iter)->getDescription());
        this_action->setShortcut("Ctrl+" + QString::number(index));
        adaptor_actions.push_back(this_action);
        connect(
            this_action,
            SIGNAL(functionSelected(int)),
            this,
            SLOT(setCurrentAdaptor(int))
        );
        index++;
    }
}

void ui::createMenus() {
    function_menu = menuBar()->addMenu("Functions");
    for (auto iter = adaptor_actions.begin();
            iter != adaptor_actions.end();
            iter++) {
        function_menu->addAction((*iter));
    }
}

void ui::setup() {
    //    currentAdaptor = new ExampleAdaptor;
    loadPlugin();
    createActions();
    createMenus();
    setupLeft();
    setupRight();
    this->setCentralWidget(new QWidget());
    QHBoxLayout* cw_layout = new QHBoxLayout();
    cw_layout->addWidget(lwrapper);
    cw_layout->addWidget(rwrapper);
    this->centralWidget()->setLayout(cw_layout);
    this->setGeometry(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
}

void ui::setupLeft() {
    if (lwrapper != Q_NULLPTR) {
        QWidget* new_lwrapper = new QWidget(this);
        this->centralWidget()->layout()->replaceWidget(lwrapper, new_lwrapper);
        delete lwrapper;
        lwrapper = new_lwrapper;
        params.clear();
    }else{
        lwrapper = new QWidget(this);
    }
    QVector<QString>names = currentAdaptor->getParamNames();
    QVector<QString>tips = currentAdaptor->getParamTooltips();
    QVector<double>defaults = currentAdaptor->getDefaults();
    QVBoxLayout* param_layout = new QVBoxLayout(lwrapper);
    for (int i = 0; i < names.size(); i++) {
        this->params.push_back(
            new ParamWidget(
                i,
                names[i],
                tips[i],
                defaults[i],
                lwrapper
            )
        );
        param_layout->addWidget(params[i]);
        connect(
            params[i],
            SIGNAL(varyingChanged(int, bool)),
            this,
            SLOT(updateVaryingAdaptorIndexes(int, bool))
        );
        connect(
            this,
            SIGNAL(varyingParamDisabled(int)),
            params[i],
            SLOT(varyingDisabled(int))
        );
    }
    param_layout->addStretch();
    lwrapper->setLayout(param_layout);
    lwrapper->setMaximumWidth(LEFT_WIDTH);
}

void ui::setupRight() {
    if (rwrapper != Q_NULLPTR) {
        delete rwrapper;
    }
    rwrapper = new QWidget();
    // GL viewport
    this->viewport = new GL();
    this->viewport->setMinimumWidth(VIEW_MINWIDTH);
    this->log = new QTextEdit();
    log->setMaximumHeight(LOG_MAXHEIGHT);
    log->setReadOnly(true);
    QVBoxLayout* right_layout = new QVBoxLayout;
    right_layout->addWidget(viewport);
    right_layout->addWidget(log);
    rwrapper->setLayout(right_layout);
}

void ui::updateVaryingAdaptorIndexes(int idx, bool state) {
    if (!state) {
        varying_adaptor_indexes.removeOne(idx);
    } else {
        varying_adaptor_indexes.push_back(idx);
        while (varying_adaptor_indexes.size() > 2) {
            int disabled_index = varying_adaptor_indexes.takeFirst();
            emit varyingParamDisabled(disabled_index);
        }
    }
}

void ui::setCurrentAdaptor(int index) {
    currentAdaptor = adaptors[index];
    emit adaptorChanged();
}

void ui::onAdaptorChanged(void) {
    varying_adaptor_indexes.clear();
    setupLeft();
    this->show();
}