import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

Rectangle {
    id: mainWindow
    visible: true
    width: 1000
    height: 1000
    color: "#f0f0f0"

    property var selectedEmployee: null

    RowLayout {
        anchors.fill: parent
        spacing: 20

        GroupBox {
            title: "Add New Employee"
            Layout.preferredWidth: parent.width * 0.5
            padding: 20

            ColumnLayout {
                spacing: 10
                Layout.fillWidth: true

                FileDialog {
                    id: fileDialog
                    title: "Select a CSV File"
                    nameFilters: ["CSV files (*.csv)"]
                    onAccepted: {
                        console.log("File selected:", fileDialog.selectedFile)
                        employeeManager.loadCsv(fileDialog.selectedFile)
                    }
                }

                Button {
                    text: "Load CSV"
                    onClicked: fileDialog.open()
                }
                ComboBox {
                    id: employeeType
                    model: ["Monthly Paid Employee", "Hourly Paid Employee", "Salesman"]
                    Layout.fillWidth: true
                }

                TextField { id: nameField; placeholderText: "Name"; Layout.fillWidth: true }
                TextField { id: ssnField; placeholderText: "Social Security Number"; Layout.fillWidth: true }
                TextField { id: monthlyCompField; placeholderText: "Monthly Compensation"; Layout.fillWidth: true }
                TextField { id: hourlyRateField; placeholderText: "Hourly Compensation"; Layout.fillWidth: true }
                TextField { id: doneHoursField; placeholderText: "Done Hours"; Layout.fillWidth: true }
                TextField { id: bonusPercentageField; placeholderText: "Bonus % (0-100)"; Layout.fillWidth: true }
                TextField { id: realizedOutcomeField; placeholderText: "Realized Outcome"; Layout.fillWidth: true }
                TextField { id: outcomeClaimField; placeholderText: "Outcome Claim"; Layout.fillWidth: true }

                Button {
                    text: "Add Employee"
                    Layout.fillWidth: true
                    onClicked: {
                        var isValid = true;
                        var errorMessage = "";

                        if (nameField.text.length === 0) {
                            isValid = false;
                            errorMessage += "employee not named!\n";
                        }

                        var monthlyComp = parseFloat(monthlyCompField.text);
                        var hourlyRate = parseFloat(hourlyRateField.text);
                        var doneHours = parseInt(doneHoursField.text);
                        var bonusPercentage = parseFloat(bonusPercentageField.text);
                        var realizedOutcome = parseFloat(realizedOutcomeField.text);

                        if (isNaN(monthlyComp) || isNaN(hourlyRate) || isNaN(doneHours) || isNaN(bonusPercentage) || isNaN(realizedOutcome)) {
                            isValid = false;
                            errorMessage += "a numeric field must have numbers\n";
                        }

                        if (bonusPercentage < 0 || bonusPercentage > 100) {
                            isValid = false;
                            errorMessage += "percentage should be between 0 and 100.\n";
                        }

                        var outcomeClaim = outcomeClaimField.text.toLowerCase() === 'true' || outcomeClaimField.text.toLowerCase() === 'false';
                        if (!outcomeClaim) {
                            isValid = false;
                            errorMessage += "cannot leave outcome claim empty.\n";
                        }

                        if (!isValid) {
                            console.log("some input wasnt valid: \n" + errorMessage);
                            return;
                        }

                        var attributes = {
                            "MonthlyCompensation": monthlyComp,
                            "HourlyRate": hourlyRate,
                            "DoneHours": doneHours,
                            "BonusPercentage": bonusPercentage,
                            "RealizedOutcome": realizedOutcome,
                            "OutcomeClaim": outcomeClaimField.text.toLowerCase() === 'true'
                        };

                        employeeManager.addEmployee(nameField.text, ssnField.text, employeeType.currentText, attributes);
                    }
                }

            }

        }

        // just a separator line
             Rectangle {
                 width: 2
                 color: "#cccccc"
                 Layout.fillHeight: true


             }


             GroupBox {
                 title: "Employees"
                 Layout.preferredWidth: parent.width * 0.6
                 padding: 20

                 ColumnLayout {
                     spacing: 10
                     Layout.fillWidth: true

                     RowLayout {
                         spacing: 10
                         Layout.fillWidth: true

                         ListView {
                             id: employeeList
                             width: 250
                             height: 300
                             model: employeeListModel
                             clip: true

                             ScrollBar.vertical: ScrollBar {}

                             delegate: Rectangle {
                                 id: delegateRect
                                 width: employeeList.width
                                 height: 70
                                 color: "#f8f8f8"
                                 ColumnLayout {
                                     anchors.fill: parent
                                     spacing: 2

                                     Text {
                                         text: model.name
                                         font.pixelSize: 16
                                         font.bold: true
                                         Layout.alignment: Qt.AlignLeft
                                     }

                                     Text {
                                         text: "Calculated Salary: " + model.salary.actualSalary
                                         font.pixelSize: 12
                                         Layout.alignment: Qt.AlignLeft
                                     }

                                     Text {
                                         text: "Type: " + model.type
                                         font.pixelSize: 12
                                         Layout.alignment: Qt.AlignLeft
                                     }
                                }

                                 MouseArea {
                                     width: delegateRect.width
                                     height: delegateRect.height
                                     hoverEnabled: true

                                     onClicked: {
                                         selectedEmployee = model
                                     }

                                     onEntered: {
                                         delegateRect.color = "#e0e0e0"
                                     }

                                     onExited: {
                                         delegateRect.color = "#f8f8f8"
                                     }
                                 }
                             }
                         }

                         GroupBox {
                             title: "Employee Information"
                             visible: selectedEmployee !== null
                             width: 250
                             padding: 10

                             ColumnLayout {
                                 spacing: 10
                                 Layout.fillWidth: true

                                 Text {
                                     text: "Name: " + (selectedEmployee ? selectedEmployee.name : "")
                                     visible: selectedEmployee !== null
                                 }

                                 Text {
                                     text: "SSN: " + (selectedEmployee ? selectedEmployee.ssn : "")
                                     visible: selectedEmployee !== null
                                 }

                                 Text {
                                     text: "Type: " + (selectedEmployee ? selectedEmployee.type : "")
                                     visible: selectedEmployee !== null
                                 }

                                 Text {
                                     text: "Salary: " + (selectedEmployee ? selectedEmployee.salary.actualSalary : "")
                                     visible: selectedEmployee !== null
                                 }

                                 Text {
                                     text: "Salary without Outcome: " + (selectedEmployee ? selectedEmployee.MonthlyCompensation : "")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Salesman" && selectedEmployee.OutcomeClaim
                                 }

                                 Text {
                                     text: "Salary w/ Outcome Claim: " + (selectedEmployee ? selectedEmployee.salary.alternativeSalary : "")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Salesman" && !selectedEmployee.OutcomeClaim
                                 }

                                 Text {
                                     text: "Hourly Rate: " + (selectedEmployee ? selectedEmployee.HourlyRate : "")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Hourly Paid Employee"
                                 }

                                 Text {
                                     text: "Done Hours: " + (selectedEmployee ? selectedEmployee.DoneHours : "")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Hourly Paid Employee"
                                 }

                                 Text {
                                     text: "Bonus Percentage: " + (selectedEmployee ? selectedEmployee.BonusPercentage + "%": "")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Salesman"
                                 }

                                 Text {
                                     text: "Realized Outcome: " + (selectedEmployee ? selectedEmployee.RealizedOutcome : "")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Salesman"
                                 }

                                 Text {
                                     text: "Monthly Compensation: " + (selectedEmployee ? selectedEmployee.MonthlyCompensation : "")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Monthly Paid Employee"
                                 }


                                 Text {
                                     text: "Outcome Claim: " + (selectedEmployee && selectedEmployee.OutcomeClaim ? "True" : "False")
                                     visible: selectedEmployee !== null && selectedEmployee.type === "Salesman"
                                 }
                             }
                         }
                     }
                     Text {
                         text: employeeListModel ? "Total: " + employeeListModel.countDistinct() : ""
                         Layout.fillWidth: true
                         Layout.topMargin: 10
                     }


                 }
             }
         }
     }
