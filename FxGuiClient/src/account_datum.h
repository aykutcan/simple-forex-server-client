/*
 * account_datum.h
 *
 *  Description: Data structure for account data
 *  Created on: Nov 11, 2012
 *      Author: nirin
 */

#ifndef Account_Datum_H_
#define Account_Datum_H_

#include <iostream>
using namespace std;

class AccountDatum {
public:
	AccountDatum();
	AccountDatum(string currency, double amount);
	virtual ~AccountDatum();

	string currency;
	double amount;
};


#endif /* Account_Datum_H_ */
