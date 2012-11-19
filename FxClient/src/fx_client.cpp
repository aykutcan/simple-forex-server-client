/*
 * fx_client.cpp
 *
 *  Description: fx client program
 *  Created on: Oct 23, 2012
 *      Author: nirin
 *
 *  Modified from the following file
 *  	blocking_tcp_echo_client.cpp
 *  	~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 		Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
 * 		Distributed under the Boost Software License, Version 1.0. (See accompanying
 * 		file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1024*1024 };

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    s.connect(*iterator);

    using namespace std; // For strlen.
    for(;;){
		std::cout << "Enter message: ";
		char request[max_length];
		std::cin.getline(request, max_length);
		size_t request_length = strlen(request);
		boost::asio::write(s, boost::asio::buffer(request, request_length));

		char reply[max_length];
		boost::system::error_code error;
		s.read_some(boost::asio::buffer(reply), error);
		std::cout << "Reply is: " << reply << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
