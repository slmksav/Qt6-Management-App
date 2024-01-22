#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQuickWidget>
#include "EmployeeManager.h"
#include "EmployeeListModel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QQuickWidget *quickWidget;
    EmployeeManager *employeeManager;
    EmployeeListModel *employeeListModel;

private slots:
    void reloadEmployeeData();
};

#endif
