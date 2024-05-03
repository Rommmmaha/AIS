#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
struct Employee {
    std::string name;
    std::string position;
};
struct Car {
    std::string make;
    std::string model;
    std::string fuelType;
};
struct FuelStation {
    std::string name;
    std::unordered_map<std::string, double> fuelPrices;
};
struct Service {
    std::string customer;
    std::string carMake;
    std::string fuelType;
    double fuelAmount;
    double totalCost;
};
void loadData(std::vector<Employee>& employees, std::vector<Car>& cars, std::vector<FuelStation>& fuelStations, std::vector<Service>& services) {
    std::ifstream file("db.txt");
    std::string line;

    while (std::getline(file, line)) {
        if (line.substr(0, 9) == "Employee:") {
            Employee employee;
            employee.name = line.substr(10, line.find(',', 10) - 10);
            employee.position = line.substr(line.find(',', 10) + 2);
            employees.push_back(employee);
        }
        else if (line.substr(0, 4) == "Car:") {
            Car car;
            car.make = line.substr(5, line.find(',', 5) - 5);
            car.model = line.substr(line.find(',', 5) + 2, line.find(',', line.find(',', 5) + 2) - (line.find(',', 5) + 2));
            car.fuelType = line.substr(line.find(',', line.find(',', 5) + 2) + 2);
            cars.push_back(car);
        }
        else if (line.substr(0, 12) == "FuelStation:") {
            FuelStation fuelStation;
            fuelStation.name = line.substr(13, line.find(',', 13) - 13);
            std::string fuelPrices = line.substr(line.find(',', 13) + 2);
            size_t pos = 0;
            while ((pos = fuelPrices.find(';')) != std::string::npos) {
                std::string fuelType = fuelPrices.substr(0, pos);
                fuelPrices = fuelPrices.substr(pos + 1);
                pos = fuelPrices.find(',');
                double price = std::stod(fuelPrices.substr(0, pos));
                fuelStation.fuelPrices[fuelType] = price;
                fuelPrices = fuelPrices.substr(pos + 1);
            }
            fuelStations.push_back(fuelStation);
        }
        else if (line.substr(0, 8) == "Service:") {
            Service service;
            service.customer = line.substr(9, line.find(',', 9) - 9);
            line = line.substr(line.find(',', 9) + 2);
            service.carMake = line.substr(0, line.find(','));
            line = line.substr(line.find(',') + 2);
            service.fuelType = line.substr(0, line.find(','));
            line = line.substr(line.find(',') + 2);
            service.fuelAmount = std::stod(line.substr(0, line.find(',')));
            line = line.substr(line.find(',') + 2);
            service.totalCost = std::stod(line);
            services.push_back(service);
        }
    }

    file.close();
}
void saveData(const std::vector<Employee>& employees, const std::vector<Car>& cars, const std::vector<FuelStation>& fuelStations, const std::vector<Service>& services) {
    std::ofstream file("db.txt");

    for (const auto& employee : employees) {
        file << "Employee:" << employee.name << "," << employee.position << "\n";
    }

    for (const auto& car : cars) {
        file << "Car:" << car.make << "," << car.model << "," << car.fuelType << "\n";
    }

    for (const auto& fuelStation : fuelStations) {
        file << "FuelStation:" << fuelStation.name << ",";
        for (const auto& fuelPrice : fuelStation.fuelPrices) {
            file << fuelPrice.first << ";" << fuelPrice.second << ",";
        }
        file << "\n";
    }

    for (const auto& service : services) {
        file << "Service:" << service.customer << "," << service.carMake << "," << service.fuelType << "," << service.fuelAmount << "," << service.totalCost << "\n";
    }

    file.close();
}
void generateServiceReport(const std::vector<Service>& services) {
    std::cout << "Service Report:\n";
    double totalRevenue = 0.0;
    for (const auto& service : services) {
        std::cout << "Customer: " << service.customer << ", Car Make: " << service.carMake << ", Fuel Type: " << service.fuelType << ", Fuel Amount: " << service.fuelAmount << ", Total Cost: " << service.totalCost << "\n";
        totalRevenue += service.totalCost;
    }
    std::cout << "Total Revenue: " << totalRevenue << "\n";
}
void generateCarMakeReport(const std::vector<Car>& cars, const std::vector<Service>& services) {
    std::unordered_map<std::string, int> carMakeCount;
    for (const auto& car : cars) {
        carMakeCount[car.make]++;
    }
    for (const auto& service : services) {
        carMakeCount[service.carMake]++;
    }

    std::cout << "Car Make Report:\n";
    for (const auto& pair : carMakeCount) {
        std::cout << "Make: " << pair.first << ", Count: " << pair.second << "\n";
    }
}
void generateFuelTypeReport(const std::vector<Car>& cars, const std::vector<Service>& services) {
    std::unordered_map<std::string, int> fuelTypeCount;
    for (const auto& car : cars) {
        fuelTypeCount[car.fuelType]++;
    }
    for (const auto& service : services) {
        fuelTypeCount[service.fuelType]++;
    }

    std::cout << "Fuel Type Report:\n";
    for (const auto& pair : fuelTypeCount) {
        std::cout << "Fuel Type: " << pair.first << ", Count: " << pair.second << "\n";
    }
}

int main() {
    std::vector<Employee> employees;
    std::vector<Car> cars;
    std::vector<FuelStation> fuelStations;
    std::vector<Service> services;
    loadData(employees, cars, fuelStations, services);

    int choice;
    do {
        std::cout << "1. Generate Service Report\n";
        std::cout << "2. Generate Car Make Report\n";
        std::cout << "3. Generate Fuel Type Report\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            generateServiceReport(services);
            break;
        case 2:
            generateCarMakeReport(cars, services);
            break;
        case 3:
            generateFuelTypeReport(cars, services);
            break;
        case 0:
            saveData(employees, cars, fuelStations, services);
            break;
        default:
            std::cout << "Invalid choice\n";
        }
    } while (choice != 0);

    return 0;
}