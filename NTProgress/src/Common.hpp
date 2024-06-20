#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string SellOffer = "SOff";
    static std::string BuyOffer = "BOff";
    static std::string GetOffers = "GOff";
    static std::string GetBalance = "GBal";
    static std::string ClearAll = "CA";
}

#endif //CLIENSERVERECN_COMMON_HPP
