#include "Employee.h"
#include "Attributes.h"

Employee::Employee(const QString& name, const QString& ssn, const QString& type, const QVariantMap& attributes)
    : name(name), socialSecurityNumber(ssn), type(type), attributes(attributes) {}

QString Employee::getName() const {
    return name;
}

QString Employee::getSocialSecurityNumber() const {
    return socialSecurityNumber;
}

QString Employee::getType() const {
    return type;
}

QVariant Employee::getAttribute(const QString& key) const {
    return attributes.value(key);
}

void Employee::setAttribute(const QString& key, const QVariant& value) {
    attributes[key] = value;
}

QVariantMap Employee::calculateSalary() const {
    QVariantMap salaryDetails;
    double salary = 0.0;
    double alternativeSalary = 0.0;
    bool ok, okRate, okHours, okSalary, okBonus;

    if (type == "Monthly Paid Employee") {
        salary = attributes.value(EmployeeAttributes::MonthlyCompensation).toDouble(&ok);
        if (!ok) {
            qWarning() << "invalid Monthly Compensation for" << name;
            salary = 0.0;
        }
    }
    else if (type == "Hourly Paid Employee") {
        double hourlyRate = attributes.value(EmployeeAttributes::HourlyRate).toDouble(&okRate);
        int doneHours = attributes.value(EmployeeAttributes::DoneHours).toInt(&okHours);
        if (!okRate || !okHours) {
            qWarning() << "invalid Hourly Paid attributes for" << name;
            salary = 0.0;
        } else {
            salary = hourlyRate * doneHours;
        }
    }
    else if (type == "Salesman") {
        double baseSalary = attributes.value(EmployeeAttributes::MonthlyCompensation).toDouble(&okSalary);
        double bonusPercentage = attributes.value(EmployeeAttributes::BonusPercentage).toDouble(&okBonus);
        bool outcomeClaim = attributes.value(EmployeeAttributes::OutcomeClaim).toBool();
        double realizedOutcome = attributes.value(EmployeeAttributes::RealizedOutcome).toDouble();

        if (!okSalary || !okBonus) {
            qWarning() << "invalid Salesman attributes for" << name;
            salary = 0.0;
        } else {
            salary = baseSalary;
            alternativeSalary = baseSalary + (realizedOutcome * bonusPercentage / 100);

            if (outcomeClaim) {
                salary = alternativeSalary;
            }
        }
    }
    else {
        qWarning() << "critically missing attributes for" << name;
    }

    salaryDetails["actualSalary"] = salary;
    salaryDetails["alternativeSalary"] = alternativeSalary;
    return salaryDetails;
}
