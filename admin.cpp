#include "Header.h"
#include <conio.h>


int Admin::authorization() {
    system("cls");
    int flag = 0;
    cout << "--------------------------------------" << endl;
    cout << "|             АВТОРИЗАЦИЯ            |" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Введите логин: ";
    cin >> this->login;
    cout << "Введите пароль: ";
    cin >> this->password;
    if (this->login == "admin" && this->password == "admin") {
        return 0;
    }

    string current_login, current_password;
    ifstream in(this->file);

    while (getline(in, current_login, ';') && getline(in, current_password)) {
        if (current_login == this->login && current_password == this->password) {
            flag = 1;
            return 0;
        }
    }

    in.close();

    return 1;
}

void Admin::registration() {
    do {
        cout << "Введите логин: ";
        cin >> this->login;
        if (this->login.length() < 5 || this->login.length() > 15) {
            cout << "Логин должен быть от 5 до 15 символов. Повторите ввод: " << endl;
        }
        cin.ignore(100, '\n');
    } while (this->login.length() < 5 || this->login.length() > 15);
    do {
        cout << "Введите пароль: ";
        cin >> this->password;
        if (this->password.length() < 5 || this->password.length() > 15) {
            cout << "Пароль должен быть от 5 до 15 символов. Повторите ввод: " << endl;
        }
        cin.ignore(100, '\n');
    } while (this->password.length() < 5 || this->password.length() > 15);

    //encrypt();

    ifstream in(this->file);
    string new_login, new_password;
    bool exist = false;

    while (getline(in, new_login, ';') && getline(in, new_password)) {
        if (this->login == new_login) {
            exist = true;
            break;
        }
    }

    in.close();

    if (exist) {
        cout << "Ошибка! Данный логин занят." << endl;
    }
    else {
        // Регистрация нового пользователя
        ofstream out(this->file, ios::app);
        if (out.is_open()) {
            out << this->login << ";" << this->password << endl;
            cout << "Аккаунт успешно зарегистрирован!" << endl;
        }
        else {
            cout << "Ошибка регистрации" << endl;
        }
        out.close();
    }

    system("pause");
}

void Admin::viewInfo() const {
    vector<string> logins;
    vector<string> passwords;

    ifstream inFile(this->file);

    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find(';'); //поиск позиции разделителя
        if (pos != string::npos) { //наден ли разделитель
            logins.push_back(line.substr(0, pos)); //до разделителя
            passwords.push_back(line.substr(pos + 1)); //после
        }
    }

    inFile.close();

    // Вывод в табличной форме
    cout << "-------------------------------------" << endl;
    cout << "|  Админ          |  Пароль         |" << endl;
    cout << "-------------------------------------" << endl;

    for (size_t i = 0; i < logins.size(); ++i) {
        cout << "|  " << setw(15) << left << logins[i] << "|  " << setw(15) << left << passwords[i] << "|" << endl;
    }

    cout << "-------------------------------------" << endl;
}

void Admin::delAccount() {
    cout << "Введите логин пользователя для удаления: ";
    string loginToDelete;
    cin >> loginToDelete;

    ifstream inFile(file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool userFound = false;

    while (getline(inFile, line)) {
        size_t pos = line.find(';');
        if (pos != string::npos) {
            string userLogin = line.substr(0, pos);
            if (userLogin != loginToDelete) {
                lines.push_back(line); //добавление в вектор
            }
            else {
                userFound = true;
            }
        }
    }

    inFile.close();

    if (!userFound) {
        cout << "Учётная запись с логином " << loginToDelete << " не найдена." << endl;
        return;
    }

    ofstream outFile(this->file);
    if (!outFile.is_open()) {
        cout << "Ошибка открытия файла для записи." << endl;
        return;
    }

    for (const string& updatedLine : lines) {
        outFile << updatedLine << endl;
    }

    outFile.close();

    cout << "Учётная запись с логином " << loginToDelete << " удалена." << endl;
}

void Admin::editAccount() {
    cout << "Введите логин для редактирования: ";
    string loginToEdit;
    cin >> loginToEdit;

    ifstream inFile(this->file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool userFound = false;

    while (getline(inFile, line)) {
        size_t pos = line.find(';');
        if (pos != string::npos) {
            string userLogin = line.substr(0, pos);
            if (userLogin != loginToEdit) {
                lines.push_back(line);
            }
            else {
                userFound = true;

                // Редактирование пароля
                string newPassword;
                do {
                    cout << "Введите новый пароль для " << userLogin << ": ";
                    cin >> newPassword;
                    if (newPassword.length() < 5 || newPassword.length() > 15) {
                        cout << "Пароль должен быть от 5 до 15 символов. Повторите ввод: " << endl;
                    }
                    cin.ignore(100, '\n');
                } while (newPassword.length() < 5 || newPassword.length() > 15);

                lines.push_back(userLogin + ";" + newPassword);
            }
        }
    }

    inFile.close();

    if (!userFound) {
        cout << "Учётная запись с логином " << loginToEdit << " не найден." << endl;
        return;
    }

    saveToFile(lines);

    cout << "Пароль для " << loginToEdit << " успешно отредактирован." << endl;
}

void Admin::saveToFile(const vector<string>& lines) {
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

void Admin::sortAccounts() const {
    vector<string> users;
    ifstream inFile(this->file);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла для чтения." << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        users.push_back(line);
    }

    inFile.close();

    // Сортировка пользователей по логинам (пузырьковая)
    for (size_t i = 0; i < users.size() - 1; ++i) {
        for (size_t j = 0; j < users.size() - 1 - i; ++j) {
            size_t pos1 = users[j].find(';');
            size_t pos2 = users[j + 1].find(';');

            string login1 = users[j].substr(0, pos1);
            string login2 = users[j + 1].substr(0, pos2);

            if (login1 > login2) {
                swap(users[j], users[j + 1]);
            }
        }
    }

    // Вывод отсортированных пользователей
    cout << "-------------------------------------" << endl;
    cout << "|  Логин          |  Пароль         |" << endl;
    cout << "-------------------------------------" << endl;
    for (const string& user : users) {
        size_t pos = user.find(';');
        string login = user.substr(0, pos);
        string password = user.substr(pos + 1);

        cout << "|  " << setw(15) << left << login << "|  " << setw(15) << left << password << "|" << endl;
      
    }
    cout << "-------------------------------------" << endl;
}

void main_menu_admin() {
    system("cls");
    int choice = 0;
    while (choice != 3) {
        system("cls");
        cout << "--------------------------------------------" << endl;
        cout << "|        ГЛАВНОЕ МЕНЮ АДМИНИСТРАТОРА        |" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "|  [1] Работа с учётными записями           |" << endl;
        cout << "|  [2] Работа с номерами гостиницы          |" << endl;
        cout << "|  [3] Выйти из аккаунта                    |" << endl;
        cout << "--------------------------------------------" << endl;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 3) {
            cout << "Некорректный ввод. Напишите значение от 1 до 3." << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> choice;
        }
        switch (choice) {
        case 1: {
            //выбор с админами или юзерами (просмотр, добавление, редактирование, поиск, сортировка, удаление)
            //функции ... 
            system("cls");
            choice = 0;
            while (choice != 3) {
                system("cls");
                cout << "--------------------------------------------" << endl;
                cout << "|        РАБОТА С УЧЁТНЫМИ ЗАПИСЯМИ        |" << endl;
                cout << "--------------------------------------------" << endl;
                cout << "|  [1] Работа с администраторами           |" << endl;
                cout << "|  [2] Работа с пользователями             |" << endl;
                cout << "|  [3] Назад                               |" << endl;
                cout << "--------------------------------------------" << endl;
                cin >> choice;
                while (cin.fail() || choice < 1 || choice > 3) {
                    cout << "Некорректный ввод. Напишите значение от 1 до 3." << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cin >> choice;
                }
                switch (choice) {
                case 1: {
                    adm_menu_admin();
                    break;
                }
                case 2: {
                    us_menu_admin();
                    break;
                }
                case 3: 
                    main_menu_admin(); 
                    break;
                }
            }
            break;
        }
        case 2: {
            rooms_menu();
            break;
        }
        case 3: {
            break;
        }
        }
    }
    return;
}

void adm_menu_admin() {
    system("cls");
    int choice = 0;
    while (choice != 6) {
        system("cls");
        cout << "--------------------------------------------" << endl;
        cout << "|          АККАУНТЫ АДМИНИСТРАТОРОВ         |" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "|  [1] Просмотр списка администраторов      |" << endl;
        cout << "|  [2] Добавить администратора              |" << endl;
        cout << "|  [3] Редактировать администратора         |" << endl;
        cout << "|  [4] Удалить администратора               |" << endl;
        cout << "|  [5] Сортировка администраторов           |" << endl;
        cout << "|  [6] Назад                                |" << endl;
        cout << "--------------------------------------------" << endl;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 6) {
            cout << "Некорректный ввод. Напишите значение от 1 до 6." << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> choice;
        }
        switch (choice) {
        case 1: {
            Admin admin;
            system("cls");
            admin.viewInfo();
            system("pause");
            break;
        }
        case 2: {
            Admin admin;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|      ДОБАВЛЕНИЕ АДМИНИСТРАТОРА      |" << endl;
            cout << "--------------------------------------" << endl;
            admin.registration();
            break; }
        case 3: {
            Admin admin;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|    РЕДАКТИРОВАНИЕ АДМИНИСТРАТОРА    |" << endl;
            cout << "--------------------------------------" << endl;
            admin.editAccount();
            system("pause");
            break;
        }
        case 4: {
            Admin admin;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|      УДАЛЕНИЕ   АДМИНИСТРАТОРА      |" << endl;
            cout << "--------------------------------------" << endl;
            admin.delAccount();
            system("pause");
            break;
        }
        case 5: {
            Admin admin;
            system("cls");
            cout << "-------------------------------------" << endl;
            cout << "|    СОРТИРОВКА   АДМИНИСТРАТОРОВ   |" << endl;
            cout << "-------------------------------------" << endl;
            admin.sortAccounts();
            system("pause");
            break;
        }
        case 6:
            break;
        }
    }
    return;
}

void us_menu_admin() {
    system("cls");
    int choice = 0;
    while (choice != 6) {
        system("cls");
        cout << "--------------------------------------------" << endl;
        cout << "|          АККАУНТЫ ПОЛЬЗОВАТЕЛЕЙ           |" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "|  [1] Просмотр списка пользователей        |" << endl;
        cout << "|  [2] Добавить пользователя                |" << endl;
        cout << "|  [3] Редактировать пользователя           |" << endl;
        cout << "|  [4] Удалить пользователя                 |" << endl;
        cout << "|  [5] Сортировка пользователей             |" << endl;
        cout << "|  [6] Назад                                |" << endl;
        cout << "--------------------------------------------" << endl;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 6) {
            cout << "Некорректный ввод. Напишите значение от 1 до 6." << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> choice;
        }
        switch (choice) {
        case 1: {
            User user;
            system("cls");
            user.viewInfo();
            system("pause");
            break;
            break;
        }
        case 2: {
            User user;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|      ДОБАВЛЕНИЕ ПОЛЬЗОВАТЕЛЯ       |" << endl;
            cout << "--------------------------------------" << endl;
            user.registration();
            break; }
        case 3: {
            User user;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|    РЕДАКТИРОВАНИЕ ПОЛЬЗОВАТЕЛЯ     |" << endl;
            cout << "--------------------------------------" << endl;
            user.editAccount();
            system("pause");
            break;
        }
        case 4: {
            User user;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|      УДАЛЕНИЕ   ПОЛЬЗОВАТЕЛЯ       |" << endl;
            cout << "--------------------------------------" << endl;
            user.delAccount();
            system("pause");
            break;
        }
        case 5: {
            User user;
            system("cls");
            cout << "-------------------------------------" << endl;
            cout << "|    СОРТИРОВКА   ПОЛЬЗОВАТЕЛЕЙ     |" << endl;
            cout << "-------------------------------------" << endl;
            user.sortAccounts();
            system("pause");
            break;
        }
        case 6:
            break;
        }
    }
    return;
}

void rooms_menu() {
    system("cls");
    int choice = 0;
    while (choice != 9) {
        system("cls");
        cout << "--------------------------------------------" << endl;
        cout << "|             РАБОТА С ДАННЫМИ             |" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "|  [1] Просмотр списка номеров             |" << endl;
        cout << "|  [2] Просмотр заказов                    |" << endl;
        cout << "|  [3] Добавление номера                   |" << endl;
        cout << "|  [4] Редактирование номера               |" << endl;
        cout << "|  [5] Удаление номера                     |" << endl;
        cout << "|  [6] Сортировка номеров                  |" << endl;
        cout << "|  [7] Поиск номеров                       |" << endl;
        cout << "|  [8] Фильтрация номеров                  |" << endl;
        cout << "|  [9] Выход                               |" << endl;
        cout << "--------------------------------------------" << endl;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 9) {
            cout << "Некорректный ввод. Напишите значение от 1 до 9." << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> choice;
        }
        switch (choice) {
        case 1: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|             НОМЕРА ГОСТИНИЦЫ             |" << endl;
            cout << "--------------------------------------------" << endl;
            room.displayRooms();
            system("pause");
            break;
        }
        case 2: {
            Booking booking;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|             АКТИВНЫЕ ЗАКАЗА              |" << endl;
            cout << "--------------------------------------------" << endl;
            booking.viewBookings();
            system("pause");
            break;
        }
        case 3: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|            ДОБАВЛЕНИЕ НОМЕРА             |" << endl;
            cout << "--------------------------------------------" << endl;
            room.addRoom();
            break;
        }
        case 4: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|          РЕДАКТИРОВАНИЕ НОМЕРА           |" << endl;
            cout << "--------------------------------------------" << endl;
            room.editRoom();
            system("pause");
            break;
        }
        case 5: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|             УДАЛЕНИЕ НОМЕРА              |" << endl;
            cout << "--------------------------------------------" << endl;
            room.deleteRoom();
            system("pause");
            break;
        }
        case 6: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|            СОРТИРОВКА НОМЕРОВ            |" << endl;
            cout << "--------------------------------------------" << endl;
            room.sortRooms();
            system("pause");
            break;
        }
        case 7: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|                ПОИСК НОМЕРА              |" << endl;
            cout << "--------------------------------------------" << endl;
            room.searchRoomByNumber();
            system("pause");
            break;
        }
        case 8: {
            Room<bool> room;
            system("cls");
            cout << "--------------------------------------------" << endl;
            cout << "|             СВОБОДНЫЕ НОМЕРА             |" << endl;
            cout << "--------------------------------------------" << endl;
            room.filterFreeRooms();
            system("pause");
            break;
        }
        case 9: {
            break;
        }
        }
    }
    return;
}
