#include "EmployeeManager.h"
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include <QUrl>
#include "Employee.h"

EmployeeManager::EmployeeManager(QObject *parent, const QString& csvFilePath)
    : QObject(parent), csvFilePath(csvFilePath) {
    loadFromCsv(csvFilePath);
}

EmployeeManager::~EmployeeManager() {
    qDeleteAll(employees);
    employees.clear();
}

QStringList EmployeeManager::parseCSV(const QString &string) {
    enum State {Normal, Quote} state = Normal;
    QStringList fields;
    QString value;

    for (int i = 0; i < string.size(); i++) {
        const QChar current = string.at(i);

        if (state == Normal) {
            if (current == ',') {
                fields.append(value.trimmed());
                value.clear();
            } else if (current == '"') {
                state = Quote;
                value += current;
            } else {
                value += current;
            }
        } else if (state == Quote) {
            if (current == '"') {
                if (i < string.size() - 1 && string.at(i + 1) == '"') {
                    value += '"';
                    i++;
                } else {
                    state = Normal;
                    value += '"';
                }
            } else {
                value += current;
            }
        }
    }

    if (!value.isEmpty()) {
        fields.append(value.trimmed());
    }

    return fields;
}

void EmployeeManager::loadFromCsv(const QString& fileName) {
    QFile file(fileName);
    qDebug() << "Trying to open CSV file at:" << fileName;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Unable to open the CSV file:" << fileName;
        return;
    }

    QTextStream in(&file);

    if (!in.atEnd()) {
        in.readLine();
    }

    int employeeCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = parseCSV(line);

        if (fields.size() < 9) {
            qWarning() << "Invalid CSV format in line:" << line;
            continue;
        }

        QString name = fields[0];
        QString ssn = fields[1];
        QString type = fields[2];
        QVariantMap attributes;
        attributes["MonthlyCompensation"] = fields[3].toDouble();
        attributes["HourlyRate"] = fields[4].toDouble();
        attributes["DoneHours"] = fields[5].toInt();
        attributes["BonusPercentage"] = fields[6].toDouble();
        attributes["RealizedOutcome"] = fields[7].toDouble();
        attributes["OutcomeClaim"] = fields[8].toLower() == "true";

        if (name.isEmpty() || ssn.isEmpty()) {
            qWarning() << "Name or SSN is empty for an employee in line:" << line;
            continue;
        }

        employeeCount++;
        Employee* employee = new Employee(name, ssn, type, attributes);
        qDebug() << "Loaded employee:" << employee->getName() << ", Type:" << employee->getType();
        employees.append(employee);

    }

    qDebug() << "Total valid employees loaded:" << employeeCount;
    file.close();
}

QList<Employee*> EmployeeManager::getEmployees() const {
    return employees;
}

QVariantList EmployeeManager::getEmployeesForQml() const {
    QVariantList employeeList;
    for (Employee* employee : employees) {
        QVariantMap salaryDetails = employee->calculateSalary();

        QVariantMap map;
        map["name"] = employee->getName();
        map["ssn"] = employee->getSocialSecurityNumber();
        map["type"] = employee->getType();
        map["salary"] = salaryDetails["actualSalary"];
        map["alternativeSalary"] = salaryDetails["alternativeSalary"];
        map["MonthlyCompensation"] = employee->getAttribute("MonthlyCompensation");
        map["HourlyRate"] = employee->getAttribute("HourlyRate");
        map["DoneHours"] = employee->getAttribute("DoneHours");
        map["BonusPercentage"] = employee->getAttribute("BonusPercentage");
        map["RealizedOutcome"] = employee->getAttribute("RealizedOutcome");
        map["OutcomeClaim"] = employee->getAttribute("OutcomeClaim");

        employeeList.append(map);
    }
    return employeeList;
}

void EmployeeManager::saveToCsv() {
    if (csvFilePath.isEmpty()) {
        qWarning() << "CSV file path is not set.";
        return;
    }

    QFile file(csvFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Unable to open the CSV file for writing:" << csvFilePath;
        return;
    }

    QTextStream out(&file);
    out << "Name,SSN,Type,MonthlyCompensation,HourlyRate,DoneHours,BonusPercentage,RealizedOutcome,OutcomeClaim\n";

    for (auto* employee : employees) {
        out << employee->getName() << ","
            << employee->getSocialSecurityNumber() << ","
            << employee->getType() << ","
            << employee->getAttribute("MonthlyCompensation").toString() << ","
            << employee->getAttribute("HourlyRate").toString() << ","
            << employee->getAttribute("DoneHours").toString() << ","
            << employee->getAttribute("BonusPercentage").toString() << ","
            << employee->getAttribute("RealizedOutcome").toString() << ","
            << (employee->getAttribute("OutcomeClaim").toBool() ? "true" : "false") << "\n";
    }

    file.close();
}

void EmployeeManager::addEmployee(const QString& name, const QString& ssn, const QString& type, const QVariantMap& attributes) {
    Employee* newEmployee = new Employee(name, ssn, type, attributes);
    employees.append(newEmployee);
    saveToCsv();
    emit employeeListChanged();
}

void EmployeeManager::loadCsv(const QString& filePath) {
    QUrl fileUrl(filePath);
    QString localPath = fileUrl.isLocalFile() ? fileUrl.toLocalFile() : filePath;
    qDebug() << "Loading CSV file from:" << localPath;
    qDeleteAll(employees);
    employees.clear();
    loadFromCsv(localPath);
    emit employeeListChanged();
}
