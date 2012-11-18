/*
 * FxAccountDatum.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: nirin
 */

#include "account_datum.h"
using namespace std;


AccountDatum::AccountDatum() {
	// TODO Auto-generated constructor stub

}

AccountDatum::AccountDatum(string currency, double amount){
	this->currency = currency;
	this->amount = amount;
};


AccountDatum::~AccountDatum() {
	// TODO Auto-generated destructor stub
}
