#include "Header.h"
#include <conio.h>

string Booking::file = "booking.txt";

int Booking::generateBookingID() {
    // Считываем максимальный индекс из файла booking.txt
    ifstream bookingFile(this->file);
    int maxID = 0;
    int bookingID;
    char delimiter;

    while (bookingFile >> bookingID >> delimiter) {
        if (bookingID > maxID) {
            maxID = bookingID;
        }

        // Чтение до конца строки
        while (bookingFile.get(delimiter) && delimiter != '\n');
    }

    bookingFile.close();

    // Возвращаем максимальный индекс+1
    return maxID + 1;
}

int Booking::bookRoom() {
    int roomToBook;
    cout << "Введите номер комнаты для бронирования: ";
    cin >> roomToBook;

    // Проверка наличия комнаты
    ifstream roomsFile("rooms.txt");
    int roomStatus;
    bool roomFound = false;

    while (roomsFile >> this->roomNumber) {
        char delimiter;
        string roomType;
        double roomPrice;
        bool roomStatus;

        roomsFile >> delimiter; //Считываем разделитель
        getline(roomsFile, roomType, ';'); //Считываем тип комнаты
        roomsFile >> roomPrice >> delimiter >> roomStatus;

        roomStatus = roomStatus ? 1 : 0; //Преобразовываем булево значение в int

        if (this->roomNumber == roomToBook && roomStatus == 0) {
            roomFound = true;
            break;
        }
    }

    roomsFile.close();

    if (!roomFound) {
        cout << "Комната не существует или уже занята." << endl;
        return -1;
    }

    //Бронирование комнаты
    this->bookingID = generateBookingID();
    this->bookingTime = time(nullptr);

    //Запрос имени и телефона пользователя
    cout << "Введите ваше имя: ";
    cin.ignore(); //Очистка буфера ввода перед getline
    getline(cin, this->guestName);
    while(this->guestName.length() < 5) {
        cout << "Введите как минимум 5 символов: ";
        getline(cin, this->guestName);

    }

    cout << "Введите ваш телефон: ";
    getline(cin, this->guestPhone);
    while (this->guestPhone.length() < 5) {
        cout << "Введите как минимум 5 символов: ";
        getline(cin, this->guestPhone);

    }

    //Запись информации в файл
    ofstream bookingFile(this->file, ios::app);
    bookingFile << this->bookingID << ';' << this->roomNumber << ';' << this->guestName << ';' << this->guestPhone << ';' << this->bookingTime << endl;
    bookingFile.close();

    cout << "Бронирование успешно завершено. Номер комнаты: " << this->roomNumber << endl;

    return this->roomNumber;
}

void Booking::viewBookings() const {
    vector<int> bookingIDs;
    vector<int> roomNumbers;
    vector<string> guestNames;
    vector<string> guestPhones;
    vector<time_t> bookingTimes;

    ifstream inFile(Booking::file);

    if (!inFile.is_open()) {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, ';');
        bookingIDs.push_back(stoi(field));

        getline(ss, field, ';');
        roomNumbers.push_back(stoi(field));

        getline(ss, field, ';');
        guestNames.push_back(field);

        getline(ss, field, ';');
        guestPhones.push_back(field);

        getline(ss, field, ';');
        bookingTimes.push_back(stoi(field));
    }

    inFile.close();

    // Вывод в табличной форме
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    cout << "|  Номер брони | Номер комнаты |  Имя гостя           |  Телефон гостя     |  Время брони              |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < bookingIDs.size(); i++) {
        char buffer[26];
        ctime_s(buffer, sizeof buffer, &bookingTimes[i]);

        //Удаление символа новой строки из строки времени
        buffer[strlen(buffer) - 1] = '\0';

        cout << "|  " << setw(12) << left << bookingIDs[i] << "|  " << setw(13) << left << roomNumbers[i] << "|  "
            << setw(20) << left << guestNames[i] << "|  " << setw(18) << left << guestPhones[i] << "|  "
            << setw(25) << left << buffer << "|" << endl;
    }

    cout << "--------------------------------------------------------------------------------------------------------" << endl;
}
