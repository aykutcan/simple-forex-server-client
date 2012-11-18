/*
 * SMA.h
 *
 *  Created on: Nov 14, 2012
 *      Author: nirin
 */

#ifndef SMA_H_
#define SMA_H_
#include "base_indicator.h"


class SMA : public BaseIndicator{
public:
	SMA();
	virtual ~SMA();
	virtual vector<pair<string,double> > calculate(vector<pair<string,double> > data);
};

#endif /* SMA_H_ */
