#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QVariantMap>

class Employee {
public:
    Employee(const QString& name, const QString& ssn, const QString& type, const QVariantMap& attributes);
    virtual ~Employee() = default;

    QString getName() const;
    QString getSocialSecurityNumber() const;
    QString getType() const;
    QVariant getAttribute(const QString& key) const;
    void setAttribute(const QString& key, const QVariant& value);

    QVariantMap calculateSalary() const;

private:
    QString name;
    QString socialSecurityNumber;
    QString type;
    QVariantMap attributes;
};

#endif
