#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include<windows.h>
#pragma execution_character_set( "utf-8" )

struct Employee {
    int id;
    std::string name;
    int departmentId;
};

struct Department {
    int id;
    std::string name;
};

struct Salary {
    int employeeId;
    double amount;
    std::string date;
};

struct Bonus {
    int employeeId;
    double amount;
    std::string reason;
};

std::vector<Employee> loadEmployees(const std::string& filename) {
    std::vector<Employee> employees;
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string field;
        Employee employee;

        std::getline(iss, field, ',');
        employee.id = std::stoi(field);
        std::getline(iss, field, ',');
        employee.name = field;
        std::getline(iss, field, ',');
        employee.departmentId = std::stoi(field);

        employees.push_back(employee);
    }

    file.close();
    return employees;
}

std::vector<Department> loadDepartments(const std::string& filename) {
    std::vector<Department> departments;
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string field;
        Department department;

        std::getline(iss, field, ',');
        department.id = std::stoi(field);
        std::getline(iss, field, ',');
        department.name = field;

        departments.push_back(department);
    }

    file.close();
    return departments;
}
std::vector<Salary> loadSalaries(const std::string& filename) {
    std::vector<Salary> salaries;
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string field;
        Salary salary;

        std::getline(iss, field, ',');
        salary.employeeId = std::stoi(field);
        std::getline(iss, field, ',');
        salary.amount = std::stod(field);
        std::getline(iss, field, ',');
        salary.date = field;

        salaries.push_back(salary);
    }

    file.close();
    return salaries;
}

std::vector<Bonus> loadBonuses(const std::string& filename) {
    std::vector<Bonus> bonuses;
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string field;
        Bonus bonus;

        std::getline(iss, field, ',');
        bonus.employeeId = std::stoi(field);
        std::getline(iss, field, ',');
        bonus.amount = std::stod(field);
        std::getline(iss, field, ',');
        bonus.reason = field;

        bonuses.push_back(bonus);
    }

    file.close();
    return bonuses;
}
void simpleQuery(const std::vector<Employee>& employees) {
    std::cout << "Список співробітників:\n";
    for (const auto& employee : employees) {
        std::cout << "ID: " << employee.id << ", Ім'я: " << employee.name << ", Відділ: " << employee.departmentId << "\n";
    }
}

void complexQuery(const std::vector<Employee>& employees, const std::vector<Department>& departments) {
    std::cout << "Співробітники та їхні відділи:\n";
    for (const auto& employee : employees) {
        std::string departmentName = "Невідомий відділ";
        for (const auto& department : departments) {
            if (department.id == employee.departmentId) {
                departmentName = department.name;
                break;
            }
        }
        std::cout << "ID: " << employee.id << ", Ім'я: " << employee.name << ", Відділ: " << departmentName << "\n";
    }
}

void queryWithCalculation(const std::vector<Salary>& salaries) {
    double totalSalary = 0.0;
    for (const auto& salary : salaries) {
        totalSalary += salary.amount;
    }
    std::cout << "Загальна сума зарплат: " << totalSalary << "\n";
}
void groupingQuery(const std::vector<Bonus>& bonuses) {
    std::cout << "Бонуси за співробітниками:\n";
    std::map<int, std::vector<Bonus>> employeeBonuses;

    for (const auto& bonus : bonuses) {
        employeeBonuses[bonus.employeeId].push_back(bonus);
    }

    for (const auto& pair : employeeBonuses) {
        int employeeId = pair.first;
        const std::vector<Bonus>& bonusesForEmployee = pair.second;

        std::cout << "Співробітник ID: " << employeeId << "\n";
        for (const auto& bonus : bonusesForEmployee) {
            std::cout << "  Бонус: " << bonus.amount << ", Причина: " << bonus.reason << "\n";
        }
        std::cout << "\n";
    }
}

void filterQuery(const std::vector<Employee>& employees, const std::vector<Salary>& salaries) {
    double minSalary = 2000.0;

    std::cout << "Співробітники з зарплатою вище " << minSalary << ":\n";

    for (const auto& employee : employees) {
        double totalSalary = 0.0;
        for (const auto& salary : salaries) {
            if (salary.employeeId == employee.id) {
                totalSalary += salary.amount;
            }
        }

        if (totalSalary >= minSalary) {
            std::cout << "ID: " << employee.id << ", Ім'я: " << employee.name << ", Зарплата: " << totalSalary << "\n";
        }
    }
}

void report(const std::vector<Employee>& employees, const std::vector<Salary>& salaries, const std::vector<Bonus>& bonuses) {
    std::cout << "Звіт про зарплати та бонуси:\n";

    for (const auto& employee : employees) {
        double totalSalary = 0.0;
        double totalBonus = 0.0;

        for (const auto& salary : salaries) {
            if (salary.employeeId == employee.id) {
                totalSalary += salary.amount;
            }
        }

        for (const auto& bonus : bonuses) {
            if (bonus.employeeId == employee.id) {
                totalBonus += bonus.amount;
            }
        }

        std::cout << "ID: " << employee.id << ", Ім'я: " << employee.name << "\n";
        std::cout << "  Загальна зарплата: " << totalSalary << "\n";
        std::cout << "  Загальний бонус: " << totalBonus << "\n";
        std::cout << "\n";
    }
}

int main() {
    SetConsoleOutputCP(65001);
    std::vector<Employee> employees = loadEmployees("employees.txt");
    std::vector<Department> departments = loadDepartments("departments.txt");
    std::vector<Salary> salaries = loadSalaries("salaries.txt");
    std::vector<Bonus> bonuses = loadBonuses("bonuses.txt");

    simpleQuery(employees);
    complexQuery(employees, departments);
    queryWithCalculation(salaries);
    groupingQuery(bonuses);
    filterQuery(employees, salaries);
    report(employees, salaries, bonuses);

    return 0;
}