#include "Request.h"
#include "Common.hpp"
#include "Message.h"

#include <iostream>

namespace Request {

std::string CreateSellOffer(tcp::socket& s, std::string id) {
    std::cout << "Input the amount of USD currency and the value in RUB for the sale: \n";
    std::string usd, rub;
    std::cin >> usd >> rub;
    std::string value = usd + ' ' + rub;
    Message::Send(s, id, Requests::SellOffer, value);
    return Message::Read(s);
}

std::string CreateBuyOffer(tcp::socket& s, std::string id) {
    std::cout << "Input the amount of USD currency and the value in RUB to purchase: \n";
    std::string usd, rub;
    std::cin >> usd >> rub;
    std::string value = usd + ' ' + rub;
    Message::Send(s, id, Requests::BuyOffer, value);
    return Message::Read(s);
}

std::string GetOffers(tcp::socket& s, std::string id) {
    Message::Send(s, id, Requests::GetOffers, "");
    return Message::Read(s);
}
std::string GetBalance(tcp::socket& s, std::string id) {
    Message::Send(s, id, Requests::GetBalance, "");
    return Message::Read(s);
}

// "Создаём" пользователя, получаем его ID.
std::string ProcessRegistration(tcp::socket& aSocket)
{
    std::string name;
    std::cout << "Hello! Enter your name: ";
    std::cin >> name;

    // Для регистрации Id не нужен, заполним его нулём
    Message::Send(aSocket, "0", Requests::Registration, name);
    return Message::Read(aSocket);
}

}
