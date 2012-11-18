/*
 * FxService.h
 *
 *  Created on: Oct 28, 2012
 *      Author: nirin
 */

#ifndef FXSERVICE_H_
#define FXSERVICE_H_

#include <mysql++/mysql++.h>
#include "IncomingRequest.h"

using namespace std;
using namespace mysqlpp;


class FxService {
public:
	FxService();
	virtual ~FxService();
	string operateRequest(string request);

private:
	string operateRequestMarketData(string from, string to, string date, string start_date, string end_date);
	string operateRequestLatestRate(string from, string to);
	string operateRequestLatestChange(string from, string to);
	string operateRequestAccountData();
	double operateRequestAccountDatum(string currency);
	string operateUpdateAccountDatum(string currency, double famount);
	string operateEnterOrder(string buy_sell, string from, string to, double famount);
	string operateCancelOrder(string id);
	string operateIncomingRequest(const IncomingRequest& incomingRequest);

	Connection conn;
	void connectDB();
	StoreQueryResult loadDataTable(string sql);
	bool executeQuery(string sql);
};

#endif /* FXSERVICE_H_ */
