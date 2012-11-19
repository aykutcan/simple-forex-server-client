/*
 * FxIndicatorFactory.h
 *
 *  Description: Factory class to create indicators
 *  Created on: Nov 14, 2012
 *      Author: nirin
 */

#ifndef Indicator_Factory_H_
#define Indicator_Factory_H_

#include "base_indicator.h"

class IndicatorFactory {
public:
	IndicatorFactory();
	virtual ~IndicatorFactory();

	static BaseIndicator* createIndicator(string str);
};

#endif /* Indicator_Factory_H_ */
