#pragma once
#include <iostream>
#include <cstdio>
#include <windows.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

void menu_user();
void main_menu_admin();
void adm_menu_admin();
void us_menu_admin();
void rooms_menu();

class Admin {
protected:
    string login;
    string password;
    string file;
    void saveToFile(const vector<string>& lines);
public:
    Admin() {
        file = "admins.txt";
        login = "";
        password = "";
    }
    int authorization();
    void registration();
    virtual void viewInfo() const;
    void delAccount();
    void editAccount();
    void sortAccounts() const;
 //   void encrypt();
};

class User : public Admin {
public:
    User() {
        file = "users.txt";
    }
    //void registration();
    void viewInfo() const override;
};

template <typename T>
class Room {
    int number;
    string type;
    double price;
    T status;
    static string file;
    void saveToFile(const vector<string>& lines);
public:
    void addRoom();
    static bool isRoomNumberUsed(int roomNumber);
    void displayRooms();
    void editRoom();
    void deleteRoom();
    void sortRooms();
    void filterFreeRooms();
    void searchRoomByNumber();
    void editStatus(int);
};

template <typename T>
string Room<T>::file = "rooms.txt";

template <typename T>
bool Room<T>::isRoomNumberUsed(int roomNumber) {
    ifstream inFile(file);
    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла для чтения." << endl;
        return false;
    }

    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        vector<string> tokens;
        string token;

        while (getline(iss, token, ';')) {
            tokens.push_back(token);
        }

        if (!tokens.empty()) {
            int existingNumber = stoi(tokens[0]);
            if (existingNumber == roomNumber) {
                inFile.close();
                return true; // Номер уже существует
            }
        }
    }

    inFile.close();
    return false; // Номер свободен
}

template <typename T>
void Room<T>::addRoom() {
    do {
        cout << "Введите номер комнаты: ";
        cin >> this->number;
        if (cin.fail() || this->number < 100 || this->number > 999) {
            cout << "Номер комнаты должен включать в себя цифры от 100 до 999. Повторите ввод: " << endl;
        }
    } while (cin.fail() || this->number < 100 || this->number > 999);

    cin.ignore();

    // Проверка наличия номера комнаты в файле
    if (isRoomNumberUsed(this->number)) {
        cout << "Ошибка! Данный номер комнаты уже занят." << endl;
        system("pause");
        return;
    }

    //cout << "Введите тип комнаты: ";
    //getline(cin, type);

    cout << "Выберите тип комнаты:" << endl;
    cout << "1. Одноместная" << endl;
    cout << "2. Двуместная" << endl;
    cout << "3. Люкс" << endl;

    int choice;
    do {
        cout << "Введите номер выбранного типа: ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 3) {
            cout << "Некорректный ввод. Повторите ввод." << endl;
        }
    } while (cin.fail() || choice < 1 || choice > 3);

    // Присваиваем тип комнаты в зависимости от выбора пользователя
    switch (choice) {
    case 1:
        this->type = "Одноместная";
        break;
    case 2:
        this->type = "Двуместная";
        break;
    case 3:
        this->type = "Люкс";
        break;
    }

    do {
        cout << "Введите стоимость: ";
        cin >> this->price;
        if (cin.fail() || this->price < 0) {
            cout << "Стоимость номера должна быть больше 0. Повторите ввод: " << endl;
        }
        cin.ignore(100, '\n');
    } while (this->price <= 0);

    this->status = T(); // Устанавливаем значение по умолчанию (0)

    ofstream outFile(this->file, ios::app);
    if (!outFile.is_open()) {
        cerr << "Ошибка открытия файла для записи." << endl;
        return;
    }

    outFile << this->number << ";" << this->type << ";" << this->price << ";" << this->status << endl;

    outFile.close();

    cout << "Номер успешно добавлен." << endl;
    system("pause");
}

template <typename T>
void Room<T>::displayRooms() {
    vector<int> roomNumbers;
    vector<string> types;
    vector<double> prices;
    vector<T> statuses;

    ifstream inFile(Room<T>::file);

    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, ';');
        roomNumbers.push_back(stoi(field));

        getline(ss, field, ';');
        types.push_back(field);

        getline(ss, field, ';');
        prices.push_back(stod(field));

        getline(ss, field, ';');
        statuses.push_back(static_cast<T>(stoi(field)));
    }

    inFile.close();

    //Вывод в табличной форме
    cout << "-------------------------------------------------------------------------" << endl;
    cout << "|  Номер комнаты  |  Тип комнаты    |  Цена           |  Статус         |" << endl;
    cout << "-------------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < roomNumbers.size(); ++i) {
        cout << "|  " << setw(15) << left << roomNumbers[i] << "|  " << setw(15) << left << types[i] << "|  "
            << setw(15) << left << prices[i] << "|  " << setw(15) << left;

        if (statuses[i] == 0) {
            cout << "Свободна";
        }
        else if (statuses[i] == 1) {
            cout << "Занята";
        }

        cout << "|" << endl;
    }

    cout << "-------------------------------------------------------------------------" << endl;
}

template <typename T>
void Room<T>::editRoom() {
    cout << "Введите номер комнаты для редактирования: ";
    int roomNumberToEdit;
    cin >> roomNumberToEdit;

    ifstream inFile(Room<T>::file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool roomFound = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, ';');
        int roomNumber = stoi(field);

        if (roomNumber != roomNumberToEdit) {
            lines.push_back(line);
        }
        else {
            roomFound = true;

            string existingType, statusString;
            double existingPrice;
            T existingStatus;

            getline(ss, existingType, ';');
            getline(ss, field, ';');
            existingPrice = stod(field);
            getline(ss, statusString, ';');
            existingStatus = static_cast<T>(stoi(statusString));

            cout << "\nТекущая информация для комнаты с номером " << roomNumber << ":" << endl;
            cout << "Тип комнаты: " << existingType << endl;
            cout << "Цена: " << existingPrice << endl;
            cout << "Статус: " << (existingStatus == 0 ? "Свободна" : "Занята") << endl << endl;

            cout << "Выберите новый тип комнаты:" << endl;
            cout << "1. Одноместная" << endl;
            cout << "2. Двуместная" << endl;
            cout << "3. Люкс" << endl;

            int choice;
            do {
                cout << "Введите номер выбранного типа: ";
                cin >> choice;
                if (cin.fail() || choice < 1 || choice > 3) {
                    cout << "Некорректный ввод. Повторите ввод." << endl;
                }
            } while (cin.fail() || choice < 1 || choice > 3);

            // Присваиваем тип комнаты в зависимости от выбора пользователя
            switch (choice) {
            case 1:
                existingType = "Одноместная";
                break;
            case 2:
                existingType = "Двуместная";
                break;
            case 3:
                existingType = "Люкс";
                break;
            }

            do {
                cout << "Введите новую стоимость: ";
                cin >> existingPrice;
                if (cin.fail() || existingPrice < 0) {
                    cout << "Стоимость номера должна быть больше 0. Повторите ввод: " << endl;
                }
                cin.ignore(100, '\n');
            } while (existingPrice <= 0);

            lines.push_back(to_string(roomNumber) + ";" + existingType + ";" + to_string(existingPrice) + ";" + to_string(existingStatus));
        }
    }

    inFile.close();

    if (!roomFound) {
        cout << "Комната с номером " << roomNumberToEdit << " не найдена." << endl;
        return;
    }

    saveToFile(lines);

    cout << "Информация о комнате с номером " << roomNumberToEdit << " успешно отредактирована." << endl;
}

template <typename T>
void Room<T>::saveToFile(const vector<string>& lines) {
    ofstream outFile(this->file);
    if (!outFile.is_open()) {
        cout << "Ошибка открытия файла для записи." << endl;
        return;
    }

    for (const string& updatedLine : lines) {
        outFile << updatedLine << endl;
    }

    outFile.close();
}

template <typename T>
void Room<T>::deleteRoom() {
    cout << "Введите номер комнаты для удаления: ";
    int roomNumberToDelete;
    cin >> roomNumberToDelete;

    ifstream inFile(Room<T>::file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool roomFound = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, ';');
        int roomNumber = stoi(field);

        if (roomNumber != roomNumberToDelete) {
            lines.push_back(line);
        }
        else {
            roomFound = true;
        }
    }

    inFile.close();

    if (!roomFound) {
        cout << "Комната с номером " << roomNumberToDelete << " не найдена." << endl;
        return;
    }

    ofstream outFile(Room<T>::file);
    if (!outFile.is_open()) {
        cout << "Ошибка открытия файла для записи." << endl;
        return;
    }

    for (const string& updatedLine : lines) {
        outFile << updatedLine << endl;
    }

    outFile.close();

    cout << "Комната с номером " << roomNumberToDelete << " удалена." << endl;
}

template <typename T>
void Room<T>::sortRooms() {
    vector<string> roomData;
    ifstream inFile(Room<T>::file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла для чтения." << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        roomData.push_back(line);
    }

    inFile.close();

    // Сортировка комнат по номерам (пузырьковая)
    for (size_t i = 0; i < roomData.size() - 1; ++i) {
        for (size_t j = 0; j < roomData.size() - 1 - i; ++j) {
            stringstream ss1(roomData[j]);
            stringstream ss2(roomData[j + 1]);

            string field1, field2;

            getline(ss1, field1, ';');
            getline(ss2, field2, ';');

            int roomNumber1 = stoi(field1);
            int roomNumber2 = stoi(field2);

            if (roomNumber1 > roomNumber2) {
                swap(roomData[j], roomData[j + 1]);
            }
        }
    }

    // Вывод отсортированных комнат
    cout << "-------------------------------------------------------------------------" << endl;
    cout << "|  Номер комнаты  |  Тип комнаты    |  Цена           |  Статус         |" << endl;
    cout << "-------------------------------------------------------------------------" << endl;

    for (const string& room : roomData) {
        stringstream ss(room);
        string field;

        while (getline(ss, field, ';')) {
            if (field == "0") {
                cout << "|  " << setw(15) << left << "Свободна";
            }
            else if (field == "1") {
                cout << "|  " << setw(15) << left << "Занята";
            }
            else {
                cout << "|  " << setw(15) << left << field;
            }
        }
        cout << "|" << endl;
    }

    cout << "-------------------------------------------------------------------------" << endl;
}

template <typename T>
void Room<T>::filterFreeRooms() {
    vector<string> freeRooms;
    ifstream inFile(Room<T>::file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла для чтения." << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, ';');
        getline(ss, field, ';');
        getline(ss, field, ';');

        //статус
        getline(ss, field, ';');
        T status = static_cast<T>(stoi(field));

        if (status == 0) {
            //замена 0 на "Свободна"
            size_t pos = line.rfind(';');
            line.replace(pos + 1, field.length(), "Свободна");
            freeRooms.push_back(line);
        }
    }

    inFile.close();

    //Вывод свободных комнат
    cout << "-------------------------------------------------------------------------" << endl;
    cout << "|  Номер комнаты  |  Тип комнаты    |  Цена           |  Статус         |" << endl;
    cout << "-------------------------------------------------------------------------" << endl;

    for (const string& room : freeRooms) {
        stringstream ss(room);
        string field;

        while (getline(ss, field, ';')) {
            cout << "|  " << setw(15) << left << field;
        }
        cout << "|" << endl;
    }

    cout << "-------------------------------------------------------------------------" << endl;
}

template <typename T>
void Room<T>::searchRoomByNumber() {
    cout << "Введите номер комнаты для поиска: ";
    int roomNumberToSearch;
    cin >> roomNumberToSearch;

    ifstream inFile(Room<T>::file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла для чтения." << endl;
        return;
    }

    bool roomFound = false;
    string line;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, ';');
        int roomNumber = stoi(field);

        if (roomNumber == roomNumberToSearch) {
            roomFound = true;

            cout << "-------------------------------------------------------------------------" << endl;
            cout << "|  Номер комнаты  |  Тип комнаты    |  Цена           |  Статус         |" << endl;
            cout << "-------------------------------------------------------------------------" << endl;

            stringstream roomInfo(line);
            while (getline(roomInfo, field, ';')) {
                if (field == "0") {
                    cout << "|  " << setw(15) << left << "Свободна";
                }
                else if (field == "1") {
                    cout << "|  " << setw(15) << left << "Занята";
                }
                else {
                    cout << "|  " << setw(15) << left << field;
                }
            }

            cout << "|" << endl;
            cout << "-------------------------------------------------------------------------" << endl;

            break;
        }
    }

    inFile.close();

    if (!roomFound) {
        cout << "Комната с номером " << roomNumberToSearch << " не найдена." << endl;
    }
}

template <typename T>
void Room<T>::editStatus(int roomNumberToEdit) {
    ifstream inFile(Room<T>::file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool roomFound = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, ';');
        int roomNumber = stoi(field);

        if (roomNumber != roomNumberToEdit) {
            lines.push_back(line);
        }
        else {
            roomFound = true;

            string existingType, statusString;
            double existingPrice;
            T existingStatus;

            getline(ss, existingType, ';');
            getline(ss, field, ';');
            existingPrice = stod(field);
            getline(ss, statusString, ';');
            existingStatus = static_cast<T>(stoi(statusString));

            existingStatus = static_cast<T>(1);

            lines.push_back(to_string(roomNumber) + ";" + existingType + ";" + to_string(existingPrice) + ";" + to_string(existingStatus));
        }
    }

    inFile.close();
    saveToFile(lines);
}

class Booking {
    int bookingID;
    int roomNumber;
    time_t bookingTime;
    string guestName;
    string guestPhone;
    static string file;
public:
    Booking() {
    }
    int bookRoom();
    int generateBookingID();
    void viewBookings() const;
};
