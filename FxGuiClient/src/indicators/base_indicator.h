/*
 * base_indicator.h
 *
 *  Description: Base indicator class, all indicators will derived from this class
 *  Created on: Nov 12, 2012
 *      Author: nirin
 */

#ifndef Base_Indicator_H_
#define Base_Indicator_H_
#include <vector>
#include <iostream>

using namespace std;
const size_t kSMAPeriod = 7;

class BaseIndicator {
public:
	BaseIndicator();
	virtual ~BaseIndicator();
	virtual vector<pair<string,double> > calculate(vector<pair<string,double> > data);
};

#endif /* Base_Indicator_H_ */
