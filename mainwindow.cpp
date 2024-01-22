#include "mainwindow.h"
#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    employeeManager(new EmployeeManager(this, "C:/Users/Sauli/Documents/QtProjektit/PersonnelManagement/data.csv")), //needs to be loaded or pre-defined
    employeeListModel(new EmployeeListModel(employeeManager, this)) {

    // connect the employee list update signal to a slot for reloading data
    connect(employeeManager, &EmployeeManager::employeeListChanged, this, &MainWindow::reloadEmployeeData);

    quickWidget = new QQuickWidget(this);
    //QML-env setup
    quickWidget->rootContext()->setContextProperty("employeeManager", employeeManager);
    quickWidget->rootContext()->setContextProperty("employeeListModel", employeeListModel);
    quickWidget->setSource(QUrl(QStringLiteral("qrc:/mainview.qml")));
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    setCentralWidget(quickWidget);
}

MainWindow::~MainWindow() {
}

void MainWindow::reloadEmployeeData() {
    employeeListModel->updateData();
}
