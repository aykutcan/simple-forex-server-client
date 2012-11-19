/*
 * outgoing_request.h
 *
 *  Description: Process and send request to server program
 *  Created on: Nov 10, 2012
 *      Author: nirin
 */

#ifndef Outgoing_Request_H_
#define Outgoing_Request_H_

#include "account_datum.h"
#include <iostream>
#include <vector>

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

#endif /* Outgoing_Request_H_ */
