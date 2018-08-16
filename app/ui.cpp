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
#include <QSlider>
#include <QImage>
#include <QFileDialog>
#include "cube.h"
#include <cmath>

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

//ui::~ui(){
//    Mesh* temp = static_cast<Mesh*>(m_geo);
//    delete temp;
//}

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
    QAction* save_action = new QAction(this);
    save_action->setText("Save Frame Buffer...");
    save_action->setToolTip("Saves the current frame buffer to disk.");
    save_action->setShortcut(QString("Ctrl+S"));
    connect(
        save_action,
        &QAction::triggered,
        this,
        &ui::onSave
    );
    file_menu = menuBar()->addMenu("File");
    file_menu->addAction(save_action);
    function_menu = menuBar()->addMenu("Functions");
    for (auto iter = adaptor_actions.begin();
            iter != adaptor_actions.end();
            iter++) {
        function_menu->addAction((*iter));
    }
}

void ui::setup() {
    loadPlugin();
    createActions();
    createMenus();
    setupLeft();
    setupRight();
    this->setCentralWidget(new QWidget(this));
    QHBoxLayout* cw_layout = new QHBoxLayout();
    cw_layout->addWidget(lwrapper);
    cw_layout->addWidget(rwrapper);
    this->centralWidget()->setLayout(cw_layout);
    this->setGeometry(0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
}

void ui::setupLeft() {
    if (lwrapper != Q_NULLPTR) {
        QWidget* new_lwrapper = new QWidget(this);
        QLayoutItem* dumpme = this->centralWidget()->layout()->replaceWidget(lwrapper, new_lwrapper);
        delete dumpme;
        delete lwrapper;
        lwrapper = new_lwrapper;
        params.clear();
    } else {
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
    //centering
    QLabel* center_lb = new QLabel("Centering:", lwrapper);
    QSlider* center = new QSlider(lwrapper);
    center->setObjectName("center_slider");
    center->setOrientation(Qt::Horizontal);
    center->setMinimum(0);
    center->setMaximum(100);
    center->setTickInterval(1);
    center->setTickPosition(QSlider::NoTicks);
    center->setValue(0);
    connect(
        center,
        &QSlider::valueChanged,
        this,
        &ui::onCenteringChanged
    );
    //range scaling
    QLabel* range_lb = new QLabel("Range Scaling:", lwrapper);
    QSlider* range = new QSlider(lwrapper);
    range->setObjectName("range_slider");
    range->setOrientation(Qt::Horizontal);
    range->setMinimum(0);
    range->setMaximum(100);
    range->setTickInterval(1);
    range->setTickPosition(QSlider::NoTicks);
    range->setValue(0);
    connect(
        range,
        &QSlider::valueChanged,
        this,
        &ui::onRangeScalingChanged
    );
    // adaptive
    QPushButton* adaptiveVisualize = new QPushButton(lwrapper);
    adaptiveVisualize->setText("Visualize\nAdaptive");
    adaptiveVisualize->setToolTip("Perform an adaptive visualization (points only)");
    connect(
        adaptiveVisualize,
        &QPushButton::clicked,
        this,
        &ui::onAdaptiveVisualize
    );
    // uniform
    QPushButton* visualizeButton = new QPushButton(lwrapper);
    visualizeButton->setText("Visualize\nUniform");
    visualizeButton->setToolTip("Perform a uniformly tesselated visualization.");
    connect(
        visualizeButton,
        SIGNAL(clicked()),
        this,
        SLOT(onVisualize())
    );
    param_layout->addWidget(center_lb);
    param_layout->addWidget(center);
    param_layout->addWidget(range_lb);
    param_layout->addWidget(range);
    param_layout->addWidget(adaptiveVisualize);
    param_layout->addWidget(visualizeButton);
    lwrapper->setLayout(param_layout);
    lwrapper->setMaximumWidth(LEFT_WIDTH);
}

void ui::setupRight() {
    if (rwrapper != Q_NULLPTR) {
        delete rwrapper;
    }
    rwrapper = new QWidget(this);
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

void ui::onCenteringChanged() {
    QSlider* center = lwrapper->findChild<QSlider*>("center_slider", Qt::FindChildrenRecursively);
    int temp = center->value();
    if (temp == 0) {
        viewport->setCentering(0);
    } else if (temp == 100) {
        viewport->setCentering(1);

    } else {
        viewport->setCentering(GLfloat(temp) / 100);
    }
}

void ui::onRangeScalingChanged() {
    QSlider* range = lwrapper->findChild<QSlider*>("range_slider", Qt::FindChildrenRecursively);
    int temp = range->value();
    if (temp == 0) {
        viewport->setRangeScaling(0);
    } else if (temp == 100) {
        viewport->setRangeScaling(1);
    } else {
        viewport->setRangeScaling(GLfloat(std::log(temp) / std::log(100)));
    }
}

#define ADAPTIVE_REGIONS 2048

void ui::onAdaptiveVisualize() {
    viewport->setPointsOnly(true);
    // get the params off the widgets
    log->append("Starting Visualization...\n");

    // bail out
    if (varying_adaptor_indexes.size() < 2) {
        log->append("Error: only single paramater selected for varying.\n");
        return;
    }
    // id the varying params
    int x_index = varying_adaptor_indexes[0];
    int y_index = varying_adaptor_indexes[1];
    // id the static params
    QVector<int> static_indexes;
    for (int i = 0; i < params.size(); i++) {
        static_indexes.push_back(i);
    }
    static_indexes.removeAll(x_index);
    static_indexes.removeAll(y_index);
    // setup an input vector & result for call
    QVector<double> input_vector(params.size());
    // pull the static params base values
    for (auto this_index = static_indexes.begin();
            this_index != static_indexes.end();
            this_index++) {
        // for each of the static indexes, pull the field contents
        input_vector[(*this_index)] = params[(*this_index)]->getBase();
        log->append("Static param: "
                    + params[(*this_index)]->getName()
                    + " set to, "
                    + QString::number(params[(*this_index)]->getBase())
                    + "\n"
                   );
    }
    // pull the ranges and steps from the varying pm widgets
    double x_min = params[x_index]->getMin();
    double x_max = params[x_index]->getMax();
    //    int x_steps = params[x_index]->getSteps();
    double y_min = params[y_index]->getMin();
    double y_max = params[y_index]->getMax();
    //    int y_steps = params[y_index]->getSteps();

    // Mesh will generated sizes by bounds
    // UI will make the calls
    log->append("Setting up Adaptive Mesh\n");
    if (m_geo != Q_NULLPTR) {
        delete m_geo;
    }
    AdaptiveMesh* a_mesh = new AdaptiveMesh(
        currentAdaptor,
        input_vector,
        x_index,
        y_index,
        QVector2D(x_min, y_min),
        QVector2D(x_max, y_max),
        ADAPTIVE_REGIONS
    );
    m_geo = a_mesh;
    connect(
        a_mesh,
        &AdaptiveMesh::pointDataReady,
        this,
        &ui::onMeshDataReady
    );
    a_mesh->dumpPoints();
}

void ui::onVisualize() {
    viewport->setPointsOnly(false);
    log->append("Starting Visualization...\n");
    // 1. get the params of tess form the widgets
    //    log->append("Testing VBO update...\n");
    //    cube testcube(2.5);
    //    viewport->updateVertexBuffer(testcube.constData(), testcube.count());
    //    return;

    // bail out
    if (varying_adaptor_indexes.size() < 2) {
        log->append("Error: only single paramater selected for varying.\n");
        return;
    }
    // id the varying params
    int x_index = varying_adaptor_indexes[0];
    int y_index = varying_adaptor_indexes[1];
    // id the static params
    QVector<int> static_indexes;
    for (int i = 0; i < params.size(); i++) {
        static_indexes.push_back(i);
    }
    static_indexes.removeAll(x_index);
    static_indexes.removeAll(y_index);
    // setup an input vector & result for call
    QVector<double> input_vector(params.size());
    double result = 0.0;
    // pull the static params base values
    for (auto this_index = static_indexes.begin();
            this_index != static_indexes.end();
            this_index++) {
        // for each of the static indexes, pull the field contents
        input_vector[(*this_index)] = params[(*this_index)]->getBase();
        log->append("Static param: "
                    + params[(*this_index)]->getName()
                    + " set to, "
                    + QString::number(params[(*this_index)]->getBase())
                    + "\n"
                   );
    }
    // pull the ranges and steps from the varying pm widgets
    double x_min = params[x_index]->getMin();
    double x_max = params[x_index]->getMax();
    int x_steps = params[x_index]->getSteps();
    double y_min = params[y_index]->getMin();
    double y_max = params[y_index]->getMax();
    int y_steps = params[y_index]->getSteps();
    log->append("X param: "
                + params[x_index]->getName()
                + " ranging from "
                + QString::number(x_min)
                + " to "
                + QString::number(x_max)
                + " over "
                + QString::number(x_steps)
                + " steps.\n"
               );
    log->append("Y param: "
                + params[y_index]->getName()
                + " ranging from "
                + QString::number(y_min)
                + " to "
                + QString::number(y_max)
                + " over "
                + QString::number(y_steps)
                + " steps.\n"
               );
    // Mesh will generated sizes by bounds
    // UI will make the calls
    log->append("Setting up Mesh\n");
    if (m_geo != Q_NULLPTR) {
        delete m_geo;
    }
    Mesh* visgeo = new Mesh(x_steps, y_steps, this);
    connect(
        visgeo,
        &Mesh::MeshDataReady,
        this,
        &ui::onMeshDataReady
    );
    m_geo = visgeo;
    // setup the domain slices
    double x_slice = (x_max - x_min) / x_steps;
    double y_slice = (y_max - y_min) / y_steps;
    // iterate over the varying domain
    log->append("Creating Points\n");
    for (int i = 0; i < x_steps; i++) {
        for (int j = 0; j < y_steps; j++) {
            input_vector[x_index] = i * x_slice + x_min;
            input_vector[y_index] = j * y_slice + y_min;
            this->currentAdaptor->call(input_vector, result);
            visgeo->addPoint(QVector3D(
                                 input_vector[x_index],
                                 input_vector[y_index],
                                 result)
                            );
        }
    }
}

void ui::onMeshDataReady(void) {
    // move the mesh data into the opengl context
    viewport->updateVertexBuffer(
        m_geo->dataTrianglesPositions(),
        m_geo->countTrianglesPositions()
    );
    viewport->updateNormalBuffer(
        m_geo->dataTrianglesNormals(),
        m_geo->countTrianglesNormals()
    );
    viewport->updateUVWBuffer(
        m_geo->dataTrianglesUVWs(),
        m_geo->countTrianglesUVWs()
    );
    viewport->updatePointBuffer(
        m_geo->dataPointPositions(),
        m_geo->countPointPositions()
    );
    viewport->setCenteringMat(m_geo->centeringTransform());
    viewport->setRangeScaleMat(m_geo->rangeScalingTransform());
    viewport->forceUpdate();
}

void ui::onSave(){
    QImage fb = viewport->grabFramebuffer();
    QString filename = QFileDialog::getSaveFileName(
                this,
                "Save Image",
                QDir::homePath() + "/Desktop/",
                "Images (*.png)",
                0);
    if (!filename.isEmpty()){
        fb.save(filename, "PNG");
    }
;}
