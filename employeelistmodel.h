#ifndef EMPLOYEELISTMODEL_H
#define EMPLOYEELISTMODEL_H

#include <QAbstractListModel>
#include <QSet>
#include "EmployeeManager.h"
#include <QAbstractListModel>


// model for representing employee data in a list, extends QAbstractListModel to interface with model-view framework
class EmployeeListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit EmployeeListModel(EmployeeManager* manager, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    enum RoleNames {
        RoleName = Qt::UserRole + 1,
        RoleSSN,
        RoleType,
        RoleSalary,
        RoleMonthlyCompensation,
        RoleHourlyRate,
        RoleDoneHours,
        RoleBonusPercentage,
        RoleRealizedOutcome,
        RoleOutcomeClaim
    };

    // specific utility counts distinct employees (names)
    Q_INVOKABLE int countDistinct() const;

public slots:
    void updateData();

private:
    EmployeeManager* employeeManager;
};

#endif
