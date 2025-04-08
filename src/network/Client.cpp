#include "Client.h"
#include <iostream>

Client::Client(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context, const std::string& server, short port)
    : io_context_(io_context),
    ssl_context_(ssl_context),
    socket_(io_context, ssl_context) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::connect(socket_.lowest_layer(), resolver.resolve(server, std::to_string(port)));
    socket_.handshake(boost::asio::ssl::stream_base::client);
}

bool Client::authenticate(const std::string& username, const std::string& password) {
    std::string request = "AUTH " + username + " " + password + "\n";
    boost::asio::write(socket_, boost::asio::buffer(request));

    boost::asio::streambuf response;
    boost::asio::read_until(socket_, response, "\n");

    std::istream is(&response);
    std::string status;
    is >> status;

    return status == "AUTH_SUCCESS";
}

bool Client::uploadModel(const std::string& username, const std::string& modelData) {
    std::string request = "UPLOAD " + username + " " + modelData + "\n";
    boost::asio::write(socket_, boost::asio::buffer(request));

    boost::asio::streambuf response;
    boost::asio::read_until(socket_, response, "\n");

    std::istream is(&response);
    std::string status;
    is >> status;

    return status == "UPLOAD_SUCCESS";
}