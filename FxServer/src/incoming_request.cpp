/*
 * IncomingRequest.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: nirin
 */

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include "incoming_request.h"
#include <vector>

using namespace std;
using namespace boost;
using namespace boost::algorithm;

IncomingRequest::IncomingRequest()
{
        // TODO Auto-generated constructor stub
}

IncomingRequest::IncomingRequest(string request)
{
        size_t pos = request.find(" ");
        string cmd = request.substr(0, pos);

        if(cmd == "request-market-data")
                request_type = RequestMarketData;
        else if(cmd == "request-latest-rate")
                request_type = RequestLatestRate;
        else if(cmd == "request-latest-change")
                request_type = RequestLatestChange;
        else if(cmd == "request-account-data")
                request_type = RequestAccountData;
        else if(cmd == "request-account-datum")
                request_type = RequestAccountDatum;
        else if(cmd == "enter-order")
                request_type = EnterOrder;
        else if(cmd == "cancel-order")
                request_type = CancelOrder;
        else
                request_type = None;

        cout << "request-type : " << request_type << endl;
        parseArguements(request);
}

IncomingRequest::~IncomingRequest() {
        // TODO Auto-generated destructor stub
}

string IncomingRequest::parseSpecificArgument(string request, string arg_name){
        string from_arg = "", from_val="";
        boost::regex from_arg_regex(str(format("-%1%\\s[a-zA-Z0-9.-]+") % arg_name));
        boost::regex from_val_regex("\\s[a-zA-Z0-9.-]+");

        try{
                boost::sregex_iterator it_arg(request.begin(), request.end(), from_arg_regex);
                boost::sregex_iterator end;
                if(it_arg != end) from_arg = it_arg->str();
                boost::sregex_iterator it_val(from_arg.begin(), from_arg.end(), from_val_regex);
                if(it_val != end) {
                        from_val = it_val->str();
                        trim(from_val);
                }
        }catch(std::exception& e){
                cout << "something wrong here" << endl;
        }
        return from_val;
}

void IncomingRequest::parseArguements(string request){
        try{
                this->from = parseSpecificArgument(request, "from");
                this->to = parseSpecificArgument(request, "to");
                this->date = parseSpecificArgument(request, "date");
                this->buy_sell = parseSpecificArgument(request, "buysell");
                this->famount = atof(parseSpecificArgument(request, "famount").c_str());
                this->id = parseSpecificArgument(request, "id");
                this->start_date = parseSpecificArgument(request, "start");
                this->end_date = parseSpecificArgument(request, "end");
        }
        catch(std::exception& e){
                cout << e.what() << endl;
        }
}

