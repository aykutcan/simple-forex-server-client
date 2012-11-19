/*
 * FxIndicator.cpp
 *
 *  Description: Base indicator class, all indicators will derived from this class
 *  Created on: Nov 12, 2012
 *      Author: nirin
 */

#include "base_indicator.h"
#include <iostream>
using namespace std;

BaseIndicator::BaseIndicator() {
	// TODO Auto-generated constructor stub

}

BaseIndicator::~BaseIndicator() {
	// TODO Auto-generated destructor stub
}

vector<pair<string,double> > BaseIndicator::calculate(vector<pair<string,double> > data){
	vector<pair<string,double> > a;
	return a;
}
