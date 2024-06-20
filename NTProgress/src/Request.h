#pragma once

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Request {

std::string CreateSellOffer(tcp::socket& s, std::string id);
std::string CreateBuyOffer(tcp::socket& s, std::string id);
std::string GetOffers(tcp::socket& s, std::string id);
std::string GetBalance(tcp::socket& s, std::string id);
std::string ProcessRegistration(tcp::socket& aSocket);

}
