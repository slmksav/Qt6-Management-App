#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include <QObject>
#include <QList>
#include "Employee.h"

class EmployeeManager : public QObject {
    Q_OBJECT

public:
    explicit EmployeeManager(QObject *parent = nullptr, const QString& csvFilePath = "");
    ~EmployeeManager();

    void loadFromCsv(const QString& fileName);
    QList<Employee*> getEmployees() const;
    Q_INVOKABLE QVariantList getEmployeesForQml() const;

signals:
    void employeeListChanged();

public slots:
    void saveToCsv();
    void loadCsv(const QString& filePath);
    void addEmployee(const QString& name, const QString& ssn, const QString& type, const QVariantMap& attributes);

private:
    QStringList parseCSV(const QString &string);
    QString csvFilePath;
    QList<Employee*> employees;
};

#endif
