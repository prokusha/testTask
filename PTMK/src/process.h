#pragma once

#include <pqxx/pqxx>

enum class genders {
    MALE,
    FEMALE
};

struct people {
    std::string name;
    std::string birthday;
    genders gender;
};

class DB {
public:
    DB(std::string pg_settings) : pg_connect(pg_settings) {}
    void createTable();
    void requestInsertPeople(people people_);
    void requestInsertPeoples(std::vector<people> peoples_);
    void requestInsertPeoplesTime(std::vector<people> peoples_);
    void printData();

private:
    pqxx::connection pg_connect;

    void sendQuery(std::string query);
    std::string intToGender(int gender_id);
    int dateToAge(std::string date);
};
