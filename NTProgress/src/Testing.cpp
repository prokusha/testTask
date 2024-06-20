#include <iostream>
#include <boost/asio.hpp>
#include <stdexcept>

#include "Message.h"
#include "Request.h"
#include "Common.hpp"
#include "json.hpp"

#include "Core.h"
#include "ServerSession.h"

using boost::asio::ip::tcp;

class Client {
public:
    Client() : resolver(io_service),
               query(tcp::v4(), "127.0.0.1", std::to_string(port)),
               iterator(resolver.resolve(query)),
               s(io_service) {
        s.connect(*iterator);
    }
    tcp::socket& getSocket() {
        return s;
    }
    void setId(std::string id_) {
        id = id_;
    }
    std::string getId() {
        return id;
    }
private:
    std::string id;
    boost::asio::io_service io_service;
    tcp::resolver resolver;
    tcp::resolver::query query;
    tcp::resolver::iterator iterator;

    tcp::socket s;
};

class Server {
public:
    Server() : s(io_service) {
        io_service.run();
    }
private:
    boost::asio::io_service io_service;
    static Core::Core core;

    ServerSession::server s;
};

std::string getId(tcp::socket& aSocket, std::string name) {
    Message::Send(aSocket, "0", Requests::Registration, name);
    return Message::Read(aSocket);
}

void Check(std::string fi, std::string se, std::string name) {
    // std::cout << fi << std::endl << se << std::endl;
    if (fi == se) {
        return;
    }
    throw std::invalid_argument(name + " err");
}

void TestCreateOffer() {
    Client c;
    c.setId(getId(c.getSocket(), "1"));

    Message::Send(c.getSocket(), c.getId(), Requests::SellOffer, "10 50");
    Message::Read(c.getSocket());
    std::string testTemp = "1 your offers now:\n"
                           "SELL:\n"
                           "1) AMOUNT USD: 10 BY PRICE RUB: 50\n"
                           "BUY:\n"
                           "\n";

    Message::Send(c.getSocket(), c.getId(), Requests::GetOffers, "");
    Check(Message::Read(c.getSocket()), testTemp, "TestCreateOffer: SELL");

    Message::Send(c.getSocket(), c.getId(), Requests::SellOffer, "0 50");
    Check(Message::Read(c.getSocket()), "Offer create reject\n", "TestCreateOffer: ZERO");

    Message::Send(c.getSocket(), c.getId(), Requests::BuyOffer, "100 50");
    Message::Read(c.getSocket());
    testTemp = "1 your offers now:\n"
               "SELL:\n"
               "1) AMOUNT USD: 10 BY PRICE RUB: 50\n"
               "BUY:\n"
               "1) BUY USD: 100 BY PRICE RUB: 50\n"
               "\n";
    Message::Send(c.getSocket(), c.getId(), Requests::GetOffers, "");
    Check(Message::Read(c.getSocket()), testTemp, "TestCreateOffer: BUY");

    std::cout << "TestCreateOffer OK\n";

    Message::Send(c.getSocket(), c.getId(), Requests::ClearAll, "");
}

void TestCreateOffers() {
    Client c;
    c.setId(getId(c.getSocket(), "2"));

    Message::Send(c.getSocket(), c.getId(), Requests::SellOffer, "10 50");
    Message::Read(c.getSocket());
    Message::Send(c.getSocket(), c.getId(), Requests::SellOffer, "20 40");
    Message::Read(c.getSocket());
    Message::Send(c.getSocket(), c.getId(), Requests::SellOffer, "100 50");
    Message::Read(c.getSocket());
    std::string testTemp = "2 your offers now:\n"
                           "SELL:\n"
                           "1) AMOUNT USD: 10 BY PRICE RUB: 50\n"
                           "2) AMOUNT USD: 20 BY PRICE RUB: 40\n"
                           "3) AMOUNT USD: 100 BY PRICE RUB: 50\n"
                           "BUY:\n"
                           "\n";
    Message::Send(c.getSocket(), c.getId(), Requests::GetOffers, "");
    Check(Message::Read(c.getSocket()), testTemp, "TestCreateOffers");
    std::cout << "TestCreateOffers OK\n";
    Message::Send(c.getSocket(), c.getId(), Requests::ClearAll, "");
}

void TestTransaction() {
    Client c1;
    Client c2;
    Client c3;

    c1.setId(getId(c1.getSocket(), "1"));
    c2.setId(getId(c2.getSocket(), "2"));
    c3.setId(getId(c3.getSocket(), "3"));

    Message::Send(c1.getSocket(), c1.getId(), Requests::BuyOffer, "10 62");
    Message::Read(c1.getSocket());

    Message::Send(c2.getSocket(), c2.getId(), Requests::BuyOffer, "20 63");
    Message::Read(c2.getSocket());

    Message::Send(c3.getSocket(), c3.getId(), Requests::SellOffer, "50 61");
    Message::Read(c3.getSocket());

    std::string testTemp = "1 your balance now:\n"
                           "USD: 10\n"
                           "RUB: -620\n";
    Message::Send(c1.getSocket(), c1.getId(), Requests::GetBalance, "");
    Check(Message::Read(c1.getSocket()), testTemp, "TestTransaction: BALANCE 1");

    testTemp = "2 your balance now:\n"
               "USD: 20\n"
               "RUB: -1260\n";
    Message::Send(c2.getSocket(), c2.getId(), Requests::GetBalance, "");
    Check(Message::Read(c2.getSocket()), testTemp, "TestTransaction: BALANCE 2");
    testTemp = "3 your balance now:\n"
               "USD: -30\n"
               "RUB: 1880\n";
    Message::Send(c3.getSocket(), c3.getId(), Requests::GetBalance, "");
    Check(Message::Read(c3.getSocket()), testTemp, "TestTransaction: BALANCE 3");

    testTemp = "2 your offers now:\n"
               "SELL:\n"
               "1) AMOUNT USD: 20 BY PRICE RUB: 61\n"
               "BUY:\n"
               "\n";

    std::cout << "TestTransaction OK\n";
    Message::Send(c1.getSocket(), c1.getId(), Requests::ClearAll, "");
}

int main() {
    try {
        TestCreateOffer();
        TestCreateOffers();
        TestTransaction();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }
}
