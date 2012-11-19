/*
 * SMA.h
 *
 *	Description: Smooth Moving Average
 *  Created on: Nov 14, 2012
 *      Author: nirin
 */

#ifndef Sma_H_
#define Sma_H_
#include "base_indicator.h"


class SMA : public BaseIndicator{
public:
	SMA();
	virtual ~SMA();
	virtual vector<pair<string,double> > calculate(vector<pair<string,double> > data);
};

#endif /* Sma_H_ */
