/*
 * SMA.cpp
 *
 *  Created on: Nov 14, 2012
 *      Author: nirin
 */

#include "sma.h"
#include <iostream>
#include <vector>

using namespace std;

SMA::SMA() {
	// TODO Auto-generated constructor stub

}

SMA::~SMA() {
	// TODO Auto-generated destructor stub
}

vector<pair<string,double> > SMA::calculate(vector<pair<string,double> > data){
	vector<pair<string,double> > indicator_data;
	double sum = 0;
	size_t i=0;

	while(i<kSMAPeriod - 1){
		sum += data.at(i).second;
		pair<string, double> pair(data.at(i).first, 0);
		indicator_data.push_back(pair);
		i++;
	}
	if(i==kSMAPeriod - 1){
		sum += data.at(i).second;
		pair<string, double> pair(data.at(i).first, sum/kSMAPeriod);
		indicator_data.push_back(pair);
		i++;
	}
	while(i<data.size()){
		sum = sum + data.at(i).second - data.at(i-kSMAPeriod).second;
		//cout << "data i : " << data.at(i) << endl;
		//cout << "sma : " << sum/kSMAPeriod << endl;
		pair<string, double> pair(data.at(i).first, sum/kSMAPeriod);
		indicator_data.push_back(pair);
		i++;
	}
	return indicator_data;
}
