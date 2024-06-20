#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include "Core.h"
#include "ServerSession.h"

int main()
{
    try
    {
        boost::asio::io_service io_service;
        static Core::Core core;

        ServerSession::server s(io_service);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
