/*
 * IncomingRequest.h
 *
 *  Created on: Oct 25, 2012
 *      Author: nirin
 */

#ifndef INCOMINGREQUEST_H_
#define INCOMINGREQUEST_H_

#include <iostream>

using namespace std;

class IncomingRequest {
public:
	enum RequestType{RequestMarketData
		, RequestLatestRate
		, RequestLatestChange
		, RequestAccountData
		, RequestAccountDatum
		, EnterOrder
		, CancelOrder
		, None};

	RequestType request_type;
	string buy_sell;
	string from;
	string to;
	string date;
	string start_date;
	string end_date;
	double famount;
	string id;

	IncomingRequest();
	IncomingRequest(string request);
	virtual ~IncomingRequest();

private:
	void parseArguements(string request);
	string parseSpecificArgument(string request, string arg_name);
};

#endif /* INCOMINGREQUEST_H_ */
