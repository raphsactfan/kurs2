#include "Header.h"
#include <conio.h>

void User::viewInfo() const {
    vector<string> logins;
    vector<string> passwords;

    ifstream inFile(this->file);

    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find(';');
        if (pos != string::npos) {
            logins.push_back(line.substr(0, pos));
            passwords.push_back(line.substr(pos + 1));
        }
    }

    inFile.close();

    // Вывод в табличной форме
    cout << "-------------------------------------" << endl;
    cout << "|  Пользователь   |  Пароль         |" << endl;
    cout << "-------------------------------------" << endl;

    for (size_t i = 0; i < logins.size(); ++i) {
        cout << "|  " << setw(15) << left << logins[i] << "|  " << setw(15) << left << passwords[i] << "|" << endl;
    }

    cout << "-------------------------------------" << endl;
}

void menu_user() {
    system("cls");
    int choice = 0;
    while (choice != 6) {
        system("cls");
        cout << "--------------------------------------------" << endl;
        cout << "|         ГЛАВНОЕ МЕНЮ ПОЛЬЗОВАТЕЛЯ        |" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "|  [1] Просмотр списка номеров             |" << endl;
        cout << "|  [2] Бронирование номера                 |" << endl;
        cout << "|  [3] Сортировка номеров                  |" << endl;
        cout << "|  [4] Поиск номера                        |" << endl;
        cout << "|  [5] Фильтрация номеров                  |" << endl;
        cout << "|  [6] Выйти из аккаунта                   |" << endl;
        cout << "--------------------------------------------" << endl;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 6) {
            cout << "Некорректный ввод. Напишите значение от 1 до 5." << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> choice;
        }
        switch (choice) {
        case 1: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|             НОМЕРА ГОСТИНИЦЫ              |" << endl;
            cout << "--------------------------------------------" << endl;
            room.displayRooms();
            system("pause");
            break;
        }
        case 2: {
            Booking booking;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|            БРОНИРОВАНИЕ НОМЕРА           |" << endl;
            cout << "--------------------------------------------" << endl;
            Room<bool> room;
            room.displayRooms();
            int roomNum = booking.bookRoom();
            room.editStatus(roomNum);
            system("pause");
            break;
        }
        case 3: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|           СОРТИРОВКА НОМЕРОВ             |" << endl;
            cout << "--------------------------------------------" << endl;
            room.sortRooms();
            system("pause");
            break;
        }
        case 4: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|              ПОИСК НОМЕРА                |" << endl;
            cout << "--------------------------------------------" << endl;
            room.searchRoomByNumber();
            system("pause");
            break;
        }
        case 5: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|            СВОБОДНЫЕ НОМЕРА              |" << endl;
            cout << "--------------------------------------------" << endl;
            room.filterFreeRooms();
            system("pause");
            break;
        }
        case 6: {
            break;
        }
        }
    }
    return;
}
