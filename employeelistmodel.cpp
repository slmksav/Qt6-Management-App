#include "EmployeeListModel.h"
#include "Employee.h"

EmployeeListModel::EmployeeListModel(EmployeeManager* manager, QObject *parent)
    : QAbstractListModel(parent), employeeManager(manager) {}

// num of rows = num of employees
int EmployeeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return employeeManager->getEmployees().count();
}

// fetches specific data for an employee based on the given index and role, the data returned depends on the role
// data is retrieved from employee object at the specified index in employeemanagers lsit.
QVariant EmployeeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= employeeManager->getEmployees().count())
        return QVariant();

    const Employee* employee = employeeManager->getEmployees().at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return employee->getName();
    case RoleSSN:
        return employee->getSocialSecurityNumber();
    case RoleType:
        return employee->getType();
    case RoleSalary:
        return employee->calculateSalary();
    case RoleMonthlyCompensation:
        return employee->getAttribute("MonthlyCompensation");
    case RoleHourlyRate:
        return employee->getAttribute("HourlyRate");
    case RoleDoneHours:
        return employee->getAttribute("DoneHours");
    case RoleBonusPercentage:
        return employee->getAttribute("BonusPercentage");
    case RoleRealizedOutcome:
        return employee->getAttribute("RealizedOutcome");
    case RoleOutcomeClaim:
        return employee->getAttribute("OutcomeClaim");
    default:
        return QVariant();
    }
}

// maps integer role identifiers to QByteArray role names for use in QML,
// enables access to employee data fields in QML using readable names,
// such as 'name', 'ssn', 'type', etc., rather than numeric role IDs.
// essential for integrating custom data roles with QML view components.
QHash<int, QByteArray> EmployeeListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[RoleSSN] = "ssn";
    roles[RoleType] = "type";
    roles[RoleSalary] = "salary";
    roles[RoleMonthlyCompensation] = "MonthlyCompensation";
    roles[RoleHourlyRate] = "HourlyRate";
    roles[RoleDoneHours] = "DoneHours";
    roles[RoleBonusPercentage] = "BonusPercentage";
    roles[RoleRealizedOutcome] = "RealizedOutcome";
    roles[RoleOutcomeClaim] = "OutcomeClaim";
    return roles;
}

// amount of employees with different names
int EmployeeListModel::countDistinct() const {
    QSet<QString> uniqueEmployeeNames;
    auto employeeList = employeeManager->getEmployees();
    for (const auto &employee : employeeList) {
        uniqueEmployeeNames.insert(employee->getName());
    }
    return uniqueEmployeeNames.size();
}

// used when employee data changes
void EmployeeListModel::updateData() {
    beginResetModel();
    endResetModel();
}
