#include <cstdlib>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <thread>
#include <iostream>     //without this, std::cout is unknown

using namespace std::chrono;
using boost::asio::ip::tcp;

//Chih-Yuan Yang: this server class is merely a wraper of an io_context class and a socket class.
class server
{
  public:
    //Chih-Yuan Yang: boost::asio::io_service is typedef of io_context
    server(boost::asio::io_service &io_service, short port_number);

  private:
    void do_accept();
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    short port_number;
};

//Chih-Yuan Yang: This function is used by a thread.
void receive_socket(short port_number);

//2024/6/25 Chih-Yuan Yang: It is better to use two seperate threads to receive images and send reports
//because the image socket is unstable and needs to re-connect frequently.
void report_results(short port_number);
