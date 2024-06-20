#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;

namespace Message {
// Отправка сообщения на сервер по шаблону.
void Send(
    tcp::socket& aSocket,
    const std::string& aId,
    const std::string& aRequestType,
    const std::string& aMessage);

// Возвращает строку с ответом сервера на последний запрос.
std::string Read(tcp::socket& aSocket);

}

