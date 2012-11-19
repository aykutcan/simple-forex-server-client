/*
 * outgoint_request.cpp
 *
 *  Description: Process and send request to server program
 *  Created on: Nov 10, 2012
 *      Author: nirin
 */

#include "outgoing_request.h"
#include "account_datum.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/detail/ptree_utils.hpp>
#include <boost/property_tree/detail/file_parser_error.hpp>


using namespace std;
using namespace boost;
using namespace boost::algorithm;
using namespace boost::gregorian;
using boost::asio::ip::tcp;

OutgoingRequest::OutgoingRequest() {
	// TODO Auto-generated constructor stub

}

OutgoingRequest::~OutgoingRequest() {
	// TODO Auto-generated destructor stub
}

string OutgoingRequest::requestByCommandLine(string cmd){

	property_tree::ptree pt;
	property_tree::ini_parser::read_ini("/home/nirin/git/simple_forex/FxGuiClient/src/config.ini", pt);
	const int buffer_length = 1024*1024;//(const int)(pt.get<string>("connection.buffer_length"));
	string server = pt.get<string>("connection.server");
	string port = pt.get<string>("connection.port");

	char reply[buffer_length];
	try{
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(tcp::v4(), server, port);
		tcp::resolver::iterator iterator = resolver.resolve(query);
		tcp::socket s(io_service);
		s.connect(*iterator);

		boost::asio::write(s, boost::asio::buffer(cmd, cmd.length()));
		boost::system::error_code error;
		int len = s.read_some(boost::asio::buffer(reply), error);
		reply[len] = '\0';
	  }
	  catch (std::exception& e)
	  {
		std::cerr << "Exception: " << e.what() << "\n";
		return "Exception !!";
	  }
	string str(reply);
	return str;
}

string OutgoingRequest::requestMarketData(string from, string to, string start_date, string end_date){
	string cmd = "request-market-data -from %1% -to %2% -start %3% -end %4%";
	cmd = str(format(cmd) % from % to % start_date % end_date);
	return requestByCommandLine(cmd);
}

string OutgoingRequest::requestMarketDataDuringPeriod(string from, string to, RequestPeriod period){
	date end_date = day_clock::local_day();
	date start_date;

	weeks one_week(1);
	months one_month(1);
	months three_months(3);
	years one_year(1);

	switch(period){
		case OneWeek: 		start_date = end_date - one_week; break;
		case OneMonth: 		start_date = end_date - one_month; break;
		case ThreeMonths: 	start_date = end_date - three_months; break;
		case OneYear: 		start_date = end_date - one_year; break;
		default: start_date = end_date; break;
	}

	string start_date_str, end_date_str;
	start_date_str = to_iso_extended_string(start_date);
	end_date_str = to_iso_extended_string(end_date);

	return requestMarketData(from, to, start_date_str, end_date_str);
}

string OutgoingRequest::requestLatestRate(string from, string to){
	string cmd = "request-latest-rate -from %1% -to %2%";
	cmd = str(format(cmd) % from % to);
	return requestByCommandLine(cmd);
}

string OutgoingRequest::requestLatestChange(string from, string to){
	string cmd = "request-latest-change -from %1% -to %2%";
	cmd = str(format(cmd) % from % to);
	return requestByCommandLine(cmd);
}

string OutgoingRequest::requestLatestAsk(string from, string to){
	return "";
}

string OutgoingRequest::requestLatestBid(string from, string to){
	return "";
}

string OutgoingRequest::enterOrderBuy(string from, string to, string famount){
	string cmd = "enter-order -buysell BUY -from %1% -to %2% -famount %3%";
	cmd = str(format(cmd) % from % to % famount);
	return requestByCommandLine(cmd);
}

string OutgoingRequest::enterOrderSell(string from, string to, string famount){
	string cmd = "enter-order -buysell SELL -from %1% -to %2% -famount %3%";
	cmd = str(format(cmd) % from % to % famount);
	return requestByCommandLine(cmd);
}

vector<AccountDatum*> OutgoingRequest::deserializeAccountData(string str){
	vector<string> strs;
	boost::split(strs,str,boost::is_any_of("/"));
	vector<AccountDatum*> data;
	for(size_t i=0; i<strs.size(); i++){
		vector<string> elems;
		boost::split(elems,strs.at(i),boost::is_any_of(","));
		string currency = elems.at(0);
		double amount = lexical_cast<double>(elems.at(1));
		cout << currency << ": " << amount << endl;
		AccountDatum* d = new AccountDatum(currency, amount);
		data.push_back(d);
	}
	return data;
}

vector<AccountDatum*> OutgoingRequest::requestAccountData(){
	vector<AccountDatum*> data;
	string cmd = "request-account-data";
	string reply = requestByCommandLine(cmd);
	cout << reply << endl;
	data = deserializeAccountData(reply);
	return data;
}
