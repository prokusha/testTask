#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <vector>

#include "Common.hpp"

namespace Core {

struct User {
    std::string name;
    int usd = 0;
    int rub = 0;
};

struct Offer {
    size_t id;
    int usd = 0;
    int rub = 0;
    bool type;
};

class Core
{
public:
    // "Регистрирует" нового пользователя и возвращает его ID.
    std::string RegisterNewUser(const std::string& aUserName);

    // Запрос имени клиента по ID
    std::string GetUserName(const std::string& aUserId);

    std::string AddValue(const std::string& aUserId, const std::string& value);

    std::string CreateOffer(const std::string& aUserId, const std::string& offer, bool type);

    std::string GetBalance(const std::string& aUserId);

    std::string GetOffers(const std::string& aUserId);

    void ClearAll();

private:
    // <UserId, UserName>
    std::map<size_t, User> mUsers;

    std::deque<Offer> allSellOffer;
    std::deque<Offer> allBuyOffer;

    std::map<size_t, std::deque<Offer*>> sellListUsers;
    std::map<size_t, std::deque<Offer*>> buyListUsers;

    std::deque<Offer*> sellList;
    std::deque<Offer*> buyList;

    void AddOffer(Offer* offer);
    void FindIntersections();
    void ChangeBalanceOffer(size_t id_sell, size_t id_buy, int usdCloseOffer, int rubCloseOffer);
    void RemoveFromMarkOffer(std::deque<Offer*>& offerList, std::deque<Offer*>::iterator it);

    void DebugServerDisplayOffers();

};

Core& GetCore();

} // namespace Core
