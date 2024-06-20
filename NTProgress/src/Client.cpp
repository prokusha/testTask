#include <iostream>
#include <boost/asio.hpp>

#include "Common.hpp"
#include "Message.h"
#include "Request.h"

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        // Мы предполагаем, что для идентификации пользователя будет использоваться ID.
        // Тут мы "регистрируем" пользователя - отправляем на сервер имя, а сервер возвращает нам ID.
        // Этот ID далее используется при отправке запросов.
        std::string my_id = Request::ProcessRegistration(s);

        while (true)
        {
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Create sell offer\n"
                         "2) Create buy offer\n"
                         "3) Get open offers\n"
                         "4) Get balance\n"
                         "9) Exit\n"
                         << std::endl;

            short menu_option_num;
            std::cin >> menu_option_num;
            switch (menu_option_num)
            {
                case 1:
                    std::cout << Request::CreateSellOffer(s, my_id);
                    break;
                case 2:
                    std::cout << Request::CreateBuyOffer(s, my_id);
                    break;
                case 3:
                    std::cout << Request::GetOffers(s, my_id);
                    break;
                case 4:
                    std::cout << Request::GetBalance(s, my_id);
                    break;
                case 9:
                {
                    exit(0);
                    break;
                }
                default:
                {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
