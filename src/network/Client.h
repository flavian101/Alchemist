#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>

class Client {
public:
    Client(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context, const std::string& server, short port);
    bool authenticate(const std::string& username, const std::string& password);
    bool uploadModel(const std::string& username, const std::string& modelData);

private:
    boost::asio::io_context& io_context_;
    boost::asio::ssl::context& ssl_context_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
};

#endif // CLIENT_H
