#include "Core.h"
#include <string>
#include <sstream>
#include <algorithm>

namespace Core {

// "Регистрирует" нового пользователя и возвращает его ID.
std::string Core::RegisterNewUser(const std::string& aUserName)
{
    size_t newUserId = mUsers.size();
    mUsers[newUserId].name = aUserName;
    return std::to_string(newUserId);
}
// Запрос имени клиента по ID
std::string Core::GetUserName(const std::string& aUserId)
{
        const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend())
    {
        return "Error! Unknown User";
    }
    else
    {
        return userIt->second.name;
    }
}

std::string Core::AddValue(const std::string& aUserId, const std::string& value) {
    const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend()) {
        return "Error! Unknown User";
    } else {
        userIt->second.rub += std::stoi(value);
        return std::to_string(userIt->second.rub);
    }
}

std::string Core::CreateOffer(const std::string& aUserId, const std::string& offer, bool type) {
    const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend()) {
        return "Error! Unknown User";
    }
    std::istringstream s(offer);
    int usd, rub;
    s >> usd >> rub;
    if (usd <= 0 || rub <= 0) {
        return "Offer create reject";
    }
    Offer offerC({userIt->first, usd, rub, type});
    Offer* it;
    if (type) {
        allSellOffer.push_back(offerC);
        it = &allSellOffer.back();
    } else {
        allBuyOffer.push_back(offerC);
        it = &allBuyOffer.back();
    }
    AddOffer(it);
    FindIntersections();
    DebugServerDisplayOffers();
    return "Offer create";
}

std::string Core::GetBalance(const std::string& aUserId) {
    const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend()) {
        return "Error! Unknown User";
    }
    std::string result = userIt->second.name + " your balance now:\n" +
    "USD: " + std::to_string(userIt->second.usd) +
    "\nRUB: " + std::to_string(userIt->second.rub);
    return result;
}

std::string Core::GetOffers(const std::string& aUserId) {
    const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend()) {
        return "Error! Unknown User";
    }
    std::string result = userIt->second.name + " your offers now:\n";

    result += "SELL:\n";
    int count = 1;
    for (Offer* off : sellListUsers[userIt->first]) {
        result += std::to_string(count) + ") AMOUNT USD: " + std::to_string(off->usd) +
        " BY PRICE RUB: " + std::to_string(off->rub) + '\n';
        count++;
    }

    result += "BUY:\n";
    count = 1;
    for (Offer* off : buyListUsers[userIt->first]) {
        result += std::to_string(count) + ") BUY USD: " + std::to_string(off->usd) +
        " BY PRICE RUB: " + std::to_string(off->rub) + '\n';
        count++;
    }
    return result;
}

void Core::AddOffer(Offer* offer) {
    std::deque<Offer*>& offerList = offer->type ? sellList : buyList;
    std::map<size_t, std::deque<Offer*>>& offerListUsers = offer->type ? sellListUsers : buyListUsers;
    auto lamd = [offer](Offer* it_){
        if (offer->type) {
            return it_->rub >= offer->rub;
        }
        return it_->rub <= offer->rub;
    };

    std::deque<Offer*>::iterator it = std::find_if(offerList.begin(), offerList.end(), lamd);

    offerList.insert(it, offer);
    offerListUsers[offer->id].push_back(offer);
}

void Core::FindIntersections() {
    std::deque<Offer*>::iterator it_s = sellList.begin(),
                                it_b = buyList.begin();
    while (it_s != sellList.end() && it_b != buyList.end()) {
        auto &it_sell = (*it_s), &it_buy = (*it_b);
        if (it_sell->rub > it_buy->rub) {
            break;
        } else if (it_sell->id == it_buy->id) {
            it_b++;
        } else if (it_sell->usd > it_buy->usd) {
            int usdCloseOffer = it_buy->usd,
                rubCloseOffer = it_buy->usd * it_buy->rub;
            it_sell->usd -= usdCloseOffer;
            ChangeBalanceOffer(it_sell->id, it_buy->id, usdCloseOffer, rubCloseOffer);
            RemoveFromMarkOffer(buyListUsers[it_buy->id], it_b);
            it_b = buyList.erase(it_b);
        } else if (it_sell->usd < it_buy->usd) {
            int usdCloseOffer = it_sell->usd,
                rubCloseOffer = it_sell->usd * it_buy->rub;
            it_buy->usd -= usdCloseOffer;
            ChangeBalanceOffer(it_sell->id, it_buy->id, usdCloseOffer, rubCloseOffer);
            RemoveFromMarkOffer(sellListUsers[it_sell->id], it_s);
            it_s = sellList.erase(it_s);
        } else if (it_sell->usd == it_buy->usd) {
            int usdCloseOffer = it_sell->usd,
                rubCloseOffer = it_sell->usd * it_buy->rub;
            ChangeBalanceOffer(it_sell->id, it_buy->id, usdCloseOffer, rubCloseOffer);
            RemoveFromMarkOffer(sellListUsers[it_sell->id], it_s);
            RemoveFromMarkOffer(buyListUsers[it_buy->id], it_b);
            it_s = sellList.erase(it_s);
            it_b = buyList.erase(it_b);
        } else {
            break;
        }
    }
}

void Core::ChangeBalanceOffer(size_t id_sell, size_t id_buy, int usdCloseOffer, int rubCloseOffer) {
    mUsers[id_sell].usd -= usdCloseOffer;
    mUsers[id_sell].rub += rubCloseOffer;
    mUsers[id_buy].usd += usdCloseOffer;
    mUsers[id_buy].rub -= rubCloseOffer;
}

void Core::RemoveFromMarkOffer(std::deque<Offer*>& offerList, std::deque<Offer*>::iterator it) {
    std::deque<Offer*>::iterator it_ = std::find(offerList.begin(), offerList.end(), (*it));
    if (it_ != offerList.end()) {
        offerList.erase(it_);
    }
}

void Core::DebugServerDisplayOffers() {
    std::cout << '\n' << "SELL OFFERS" << '\n';
    for (Offer* off : sellList) {
        std::cout << off->id << ") USD:" << off->usd << " RUB:" << off->rub << '\n';
    }
    std::cout << '\n' << "BUY OFFERS" << '\n';
    for (Offer* off : buyList) {
        std::cout << off->id << ") USD:" << off->usd << " RUB:" << off->rub << '\n';
    }
    std::cout << '\n';
}

void Core::ClearAll() {
    mUsers.clear();
    sellListUsers.clear();
    buyListUsers.clear();
    sellList.clear();
    buyList.clear();
    allSellOffer.clear();
    allBuyOffer.clear();

    std::cout << "Server data wipe" << '\n';
}

Core& GetCore()
{
    static Core core;
    return core;
}

} // namespace Core
