/*
 * FxRequest.h
 *
 *  Created on: Nov 10, 2012
 *      Author: nirin
 */

#ifndef FXREQUEST_H_
#define FXREQUEST_H_

//#define FX_SERVER "localhost"
//#define PORT "8090"

#include <iostream>
#include "account_datum.h"
#include <vector>

//const int max_length = 1024*1024;

using namespace std;
class OutgoingRequest {
public:
	OutgoingRequest();
	virtual ~OutgoingRequest();

	enum RequestPeriod{OneWeek, OneMonth, ThreeMonths, OneYear};

	static string requestMarketData(string from, string to, string start_date, string end_date);
	static string requestMarketDataDuringPeriod(string from, string to, RequestPeriod period);
	static string requestLatestRate(string from, string to);
	static string requestLatestChange(string from, string to);
	static string requestLatestAsk(string from, string to);
	static string requestLatestBid(string from, string to);
	static string requestByCommandLine(string cmd);
	static string enterOrderBuy(string from, string to, string famount);
	static string enterOrderSell(string from, string to, string famount);
	static vector<AccountDatum*> requestAccountData();
	static vector<AccountDatum*> deserializeAccountData(string str);
};

#endif /* FXREQUEST_H_ */
