#include "ServerSession.h"
#include "Common.hpp"
#include "Core.h"

using boost::asio::ip::tcp;

namespace ServerSession {

// session //

tcp::socket& session::socket()
{
    return socket_;
}

void session::start()
{
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
{
    if (!error)
    {
        data_[bytes_transferred] = '\0';

        // Парсим json, который пришёл нам в сообщении.
        auto j = nlohmann::json::parse(data_);
        auto reqType = j["ReqType"];

        std::string reply = "Error! Unknown request type";
        if (reqType == Requests::Registration)
        {
            // Это реквест на регистрацию пользователя.
            // Добавляем нового пользователя и возвращаем его ID.
            reply = Core::GetCore().RegisterNewUser(j["Message"]);
        } else if (reqType == Requests::SellOffer) {
            reply = Core::GetCore().CreateOffer(j["UserId"], j["Message"], true) + '\n';
        } else if (reqType == Requests::BuyOffer) {
            reply = Core::GetCore().CreateOffer(j["UserId"], j["Message"], false) + '\n';
        } else if (reqType == Requests::GetOffers) {
            reply = Core::GetCore().GetOffers(j["UserId"]) + '\n';
        } else if (reqType == Requests::GetBalance) {
            reply = Core::GetCore().GetBalance(j["UserId"]) + '\n';
        } else if (reqType == Requests::ClearAll) {
            Core::GetCore().ClearAll();
        }

        boost::asio::async_write(socket_,
            boost::asio::buffer(reply, reply.size()),
            boost::bind(&session::handle_write, this,
                boost::asio::placeholders::error));
    }
    else
    {
        delete this;
    }
}

void session::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&session::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        delete this;
    }
}

// server //

void server::handle_accept(session* new_session,
    const boost::system::error_code& error)
{
    if (!error)
    {
        new_session->start();
        new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
    }
    else
    {
        delete new_session;
    }
}

} // namespace ServerSession
