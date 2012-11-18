/*
 * FxIndicatorFactory.h
 *
 *  Created on: Nov 14, 2012
 *      Author: nirin
 */

#ifndef INDICATORFACTORY_H_
#define INDICATORFACTORY_H_

#include "base_indicator.h"

class IndicatorFactory {
public:
	IndicatorFactory();
	virtual ~IndicatorFactory();

	static BaseIndicator* createIndicator(string str);
};

#endif /* INDICATORFACTORY_H_ */
