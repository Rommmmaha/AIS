#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<windows.h>
#pragma execution_character_set( "utf-8" )
using namespace std;
struct Student {
    string name;
    string group;
    int attendance;
};

bool compareStudents(const Student& s1, const Student& s2) { return s1.name < s2.name; }

int main() {
    SetConsoleOutputCP(65001);
    vector<Student> students;
    ifstream infile("students.txt");
    string line;

    while (getline(infile, line)) {
        Student s;
        size_t pos = line.find(',');
        s.name = line.substr(0, pos);
        line = line.substr(pos + 1);
        pos = line.find(',');
        s.group = line.substr(0, pos);
        s.attendance = stoi(line.substr(pos + 1));
        students.push_back(s);
    }
    infile.close();

    int choice;
    do
    {
        cout << "1. Додати студента\n";
        cout << "2. Змінити дані про студента\n";
        cout << "3. Видалити студента\n";
        cout << "4. Переглянути список студентів\n";
        cout << "5. Пошук студента\n";
        cout << "6. Сортувати список студентів\n";
        cout << "0. Вихід\n";
        cout << "Введіть номер операції: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Student s;
            cout << "Введіть ім'я студента: ";
            cin >> s.name;
            cout << "Введіть групу: ";
            cin >> s.group;
            cout << "Введіть кількість відвідувань: ";
            cin >> s.attendance;
            students.push_back(s);
            break;
        }
        case 2: {
            string name;
            cout << "Введіть ім'я студента: ";
            cin >> name;
            auto it = find_if(students.begin(), students.end(), [&](const Student& s) { return s.name == name; });
            if (it != students.end()) {
                cout << "Введіть нову групу: ";
                cin >> it->group;
                cout << "Введіть нову кількість відвідувань: ";
                cin >> it->attendance;
            }
            else { cout << "Студент не знайдений\n"; }
            break;
        }
        case 3: {
            string name;
            cout << "Введіть ім'я студента: ";
            cin >> name;
            auto it = find_if(students.begin(), students.end(), [&](const Student& s) { return s.name == name; });
            if (it != students.end()) { students.erase(it); }
            else { cout << "Студент не знайдений\n"; }
            break;
        }
        case 4: {
            for (const auto& s : students) { cout << s.name << ", " << s.group << ", " << s.attendance << "\n"; }
            break;
        }
        case 5:
        {
            string name;
            cout << "Введіть ім'я студента: ";
            cin >> name;
            auto it = find_if(students.begin(), students.end(), [&](const Student& s) { return s.name == name; });
            if (it != students.end()) { cout << it->name << ", " << it->group << ", " << it->attendance << "\n"; }
            else { cout << "Студент не знайдений\n"; }
            break;
        }
        case 6:
        {
            sort(students.begin(), students.end(), compareStudents);
            break;
        }
        case 0: { break; }
        default: { cout << "Невірний вибір\n"; }
        }
    } while (choice != 0);
    ofstream outfile("students.txt");
    for (const auto& s : students) { outfile << s.name << "," << s.group << "," << s.attendance << "\n"; }
    outfile.close();
    return 0;
}