#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

#include "process.h"

void usage() {
    std::cerr << "Usage: exec [1|2|3|4|5]";
}

std::string getConnectSetting() {
    std::ifstream in;
    in.open("pg_config.txt");
    if (in.is_open()) {
        std::string settings = "", line;
        while (std::getline(in, line)) {
            settings += line + ' ';
        }
        in.close();
        return settings;
    }
    std::cerr << "config (db_config.txt) file not exist \n";
    throw 0;
}

std::vector<people> getPeoplesF(int n) {
    std::srand(std::time(nullptr));
    std::vector<people> peoples(n);
    std::vector<std::vector<std::string>> temp = {
    {"Roman", "Mikhail", "Dmitry", "Nikolai", "Ivan", "Gordey", "Savva", "Vladimir", "Timofey", "Leo"},
    {"Feinstein", "Fal", "Faladin", "Fali", "Faller", "Falman", "Familiya", "Fanbukanov", "Farrakhov", "Febrelev"},
    {"Fedorov", "Petrov", "Romanov", "Artyomov", "Daniilov", "Alexandrov", "Yaroslavov", "Davidov", "Vladimirov", "Grigoriev"}};

    for (int i = 0; i < n; ++i) {
        int rand_name = std::rand() % 10;
        int rand_lastname = std::rand() % 10;
        int rand_midlname = std::rand() % 10;
        std::string fio = temp[1][rand_lastname] + ' ' + temp[0][rand_name] + ' ' + temp[2][rand_midlname] + "ich";
        std::string year = std::to_string(1960 + std::rand() % 50);
        int r = std::rand() % 12;
        std::string mouth = r >= 10 ? std::to_string(r) : "0" + std::to_string(r);
        int rr = 28;
        if (r != 2 && r != 7 && r != 8 && r % 2) {
            rr += 3;
        } else if (r == 7 || r == 8) {
            rr += 2;
        }
        std::string day = std::to_string(std::rand() % rr);
        peoples[i] = {fio, year + "-" + mouth + "-" + day, genders::MALE};
    }

    return peoples;
}

std::vector<people> getPeoples(int n) {
    std::srand(std::time(nullptr));
    std::vector<people> peoples(n);

    std::vector<std::vector<std::string>> temp = {
    {"Vera", "Xenia", "Milana", "Veronica", "Alexandra", "Margarita", "Safia", "Eva", "Yasmina", "Valeria"},
    {"Roman", "Mikhail", "Dmitry", "Nikolai", "Ivan", "Gordey", "Savva", "Vladimir", "Timofey", "Leo"},
    {"Korolev", "Mitrofanov", "Terentyev", "Diakonov", "Zaitsev", "Naumov", "Vinogradov", "Vasilyev", "Egorov", "Popov"},
    {"Fedorov", "Petrov", "Romanov", "Artyomov", "Daniilov", "Alexandrov", "Yaroslavov", "Davidov", "Vladimirov", "Grigoriev"}};

    bool pref_male = true;
    for (int i = 0; i < n; ++i) {
        int rand_name = std::rand() % 10;
        int rand_lastname = std::rand() % 10;
        int rand_midlname = std::rand() % 10;
        std::string fio;
        if (pref_male) {
            fio = temp[2][rand_lastname] + "a " + temp[0][rand_name] + ' ' + temp[3][rand_midlname] + "na";
            pref_male = false;
        } else {
            fio = temp[2][rand_name] + ' ' + temp[1][rand_name] + ' ' + temp[3][rand_midlname] + "ich";
            pref_male = true;
        }
        std::string year = std::to_string(1960 + std::rand() % 50);
        int r = std::rand() % 12;
        std::string mouth = r >= 10 ? std::to_string(r) : "0" + std::to_string(r);
        int rr = 28;
        if (r != 2 && r != 7 && r != 8 && r % 2) {
            rr += 3;
        } else if (r == 7 || r == 8) {
            rr += 2;
        }
        std::string day = std::to_string(std::rand() % rr);
        peoples[i] = {fio, year + "-" + mouth + "-" + day, (pref_male ? genders::MALE : genders::FEMALE)};
    }

    return peoples;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage();
        return -1;
    }
    const std::string_view mode(argv[1]);
    DB db(getConnectSetting());

    if (mode == "1") {
        db.createTable();
    } else if (mode == "2") {

        if (argc != 5) {
            std::cerr << "args not valid \n";
            return -1;
        }

        genders gender = std::string_view(argv[4]) == "Male" ? genders::MALE : genders::FEMALE;
        people people = {argv[2], argv[3], gender};

        db.requestInsertPeople(people);
    } else if (mode == "3") {
        db.printData();
    } else if (mode == "4") {
        db.requestInsertPeoples(std::move(getPeoples(1000000)));
    } else if (mode == "5") {
        db.requestInsertPeoplesTime(getPeoplesF(100));
    } else {
        usage();
        return -1;
    }
}
