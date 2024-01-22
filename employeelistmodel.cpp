#include "EmployeeListModel.h"
#include "Employee.h"

EmployeeListModel::EmployeeListModel(EmployeeManager* manager, QObject *parent)
    : QAbstractListModel(parent), employeeManager(manager) {}

int EmployeeListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return employeeManager->getEmployees().count();
}

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

int EmployeeListModel::countDistinct() const {
    QSet<QString> uniqueEmployeeNames;
    auto employeeList = employeeManager->getEmployees();
    for (const auto &employee : employeeList) {
        uniqueEmployeeNames.insert(employee->getName());
    }
    return uniqueEmployeeNames.size();
}

void EmployeeListModel::updateData() {
    beginResetModel();
    endResetModel();
}
