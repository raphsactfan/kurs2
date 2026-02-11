#include "Header.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice = 0;
    while (choice != 3) {
        system("cls");
        cout << "--------------------------------------------" << endl;
        cout << "|               ГЛАВНОЕ МЕНЮ               |" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "|  [1] Войти как пользователь              |" << endl;
        cout << "|  [2] Зарегистрироваться как пользователь |" << endl;
        cout << "|  [3] Войти как администратор             |" << endl;
        cout << "|  [4] Выход из программы                  |" << endl;
        cout << "--------------------------------------------" << endl;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 4) {
            cout << "Некорректный ввод. Напишите значение от 1 до 4." << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> choice;
        }
        switch (choice) {
        case 1: {
            User user;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|             АВТОРИЗАЦИЯ            |" << endl;
            cout << "--------------------------------------" << endl;
            if (user.authorization() == 0) {
                cout << "Вход пользователя успешно выполнен." << endl;
                system("pause");
                menu_user();
            }
            else {
                cout << "\nНеправильный логин или пароль." << endl;
                system("pause");
            }
            choice = 0;
            break;
        }
        case 2: {
            User user;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|             РЕГИСТРАЦИЯ            |" << endl;
            cout << "--------------------------------------" << endl;
            user.registration();
            break;
        }
        case 3: {
            Admin admin;
            system("cls");
            cout << "--------------------------------------" << endl;
            cout << "|             АВТОРИЗАЦИЯ            |" << endl;
            cout << "--------------------------------------" << endl;
            if (admin.authorization() == 0) {
                cout << "Вход администратора успешно выполнен." << endl;
                system("pause");
                main_menu_admin();
            }
            else {
                cout << "\nНеправильный логин или пароль." << endl;
                system("pause");
            }
            choice = 0;
            break;
        }
        case 4: {
            exit(0);
            break;
        }
        }
    }
    return 0;
}
