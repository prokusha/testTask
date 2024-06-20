#include "process.h"

#include <iostream>
#include <ctime>
#include <pqxx/internal/result_iterator.hxx>
#include <chrono>


void DB::createTable() {
    std::string query = "CREATE TABLE peoples ( \
		id SERIAL PRIMARY KEY, \
		full_name TEXT NOT NULL, \
		birth_day TEXT NOT NULL, \
		gender INTEGER NOT NULL);";
    sendQuery(std::move(query));
}

void DB::requestInsertPeople(people people_) {
    std::string gender = people_.gender == genders::MALE ? "1" : "0";
    std::string query = "INSERT INTO public.peoples( \
	id, full_name, birth_day, gender) \
	VALUES (DEFAULT, \'" + people_.name + "\', \'" + people_.birthday + "\', " + gender + ");";
    sendQuery(std::move(query));
}

void DB::requestInsertPeoples(std::vector<people> peoples_) {
    for (people p : peoples_) {
        requestInsertPeople(p);
    }
}

void DB::requestInsertPeoplesTime(std::vector<people> peoples_) {
    auto start = std::chrono::steady_clock::now();
    requestInsertPeoples(std::move(peoples_));
    auto end = std::chrono::steady_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() / 1e9 << " sec\n";
}

void DB::printData() {
    std::string query = "SELECT DISTINCT full_name, birth_day, gender \
	FROM public.peoples ORDER BY full_name;";
    pqxx::nontransaction pg_nontran(pg_connect);
    pqxx::result pg_result(pg_nontran.exec(query));
    std::cout << "TEST" << '\n';
    for (pqxx::const_result_iterator it = pg_result.begin(); it != pg_result.end(); ++it) {
        std::cout << "FULL_NAME = " << it[0].as<std::string>()
                  << " BIRTH_DAY = " << it[1].as<std::string>() << " GENDER = " << intToGender(it[2].as<int>())
                  << " AGE = " << dateToAge(it[1].as<std::string>()) << '\n';
    }
}

void DB::sendQuery(std::string query) {
    pqxx::work pg_work(pg_connect);
    pg_work.exec(query);
    pg_work.commit();
}

std::string DB::intToGender(int gender_id) {
    if (gender_id == 1) {
        return "Male";
    }
    return "Female";
}

int DB::dateToAge(std::string date) {
    std::vector<int> split_date;

    int pos = 0;
    while ((pos = date.find('-')) != std::string::npos) {
        std::string data = date.substr(0, pos);
        split_date.push_back(std::stoi(data));
        date.erase(0, pos + 1);
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);
    int age;

    age = (ltm->tm_year + 1900) - split_date[0];
    if(split_date[1] > ltm->tm_mon + 1)
        age--;
    else if ( (split_date[1] == ltm->tm_mon + 1) && (split_date[2] > ltm->tm_mday) )
        age--;

    return age;
}
