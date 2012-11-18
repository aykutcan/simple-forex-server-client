/*
 * FxService.cpp
 *
 *  Created on: Oct 28, 2012
 *      Author: nirin
 */

#include <vector>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <mysql++/mysql++.h>
#include "fx_service.h"

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace mysqlpp;

FxService::FxService() {
        // TODO Auto-generated constructor stub

}

FxService::~FxService() {
        // TODO Auto-generated destructor stub
}



void FxService::connectDB()
{
        if(conn == NULL)
                conn = Connection(false);
        if(!conn.connected())
                conn.connect("coredb", "localhost", "root", "staccato");
}

StoreQueryResult FxService::loadDataTable(string sql)
{
        connectDB();
        Query query = conn.query(sql.c_str());
        StoreQueryResult res = query.store();
        return res;
}

bool FxService::executeQuery(string sql)
{
        connectDB();
    Query query = conn.query();
    query << sql;
    return query.execute();
}

string FxService::operateRequestMarketData(string from, string to, string date,
                string start_date, string end_date)
{
        if(!from.empty() && !to.empty() && !date.empty())
        {
                string value = "";
                try{
                        string sql = "select date, rate from tblExchangeRate where `from` = '%1%' and `to` = '%2%' and date = '%3%'";
                        sql = str(format(sql) % from % to % date);
                        StoreQueryResult res = loadDataTable(sql);
                        if(res.size()>0)
                                res[0]["rate"].to_string(value);
                }catch(const Exception& er){
                        cerr << "Error: " << er.what() << endl;
                }
                if(value.empty()) value = "No Data !!";
                return value;
        }
        else if(!from.empty() && !to.empty() && !start_date.empty() && !end_date.empty()){
                string value = "", date = "", rate = "";
                try{
                        string sql = "select date, rate from tblExchangeRate where `from` = '%1%' and `to` = '%2%' "
                                        "and date >= '%3%' and date <= '%4%'";
                        sql = str(format(sql) % from % to % start_date % end_date);
                        StoreQueryResult res = loadDataTable(sql);
                        vector<string> data;
                        for(size_t i=0; i<res.size(); i++){
                                res[i]["date"].to_string(date);
                                res[i]["rate"].to_string(rate);
                                data.push_back(date+":"+rate);
                        }
                        value = boost::algorithm::join(data, ",");
                        return value;
                }catch(const Exception& er){
                        cerr << "Error: " << er.what() << endl;
                }
                if(value.empty()) value = "No Data !!";
                return value;
        }
        else{
                return "Invalid Command !!";
        }
}

string FxService::operateRequestLatestRate(string from, string to)
{
        if(!from.empty() && !to.empty())
        {
                string value = "";
                try{
                        string sql = "select rate from tblExchangeRate "
                                        "where `from` = '%1%' and `to` = '%2%' "
                                        "order by date desc "
                                        "limit 0,1";
                        sql = str(format(sql) % from % to);
                        StoreQueryResult res = loadDataTable(sql);
                        if(res.size()>0)
                                res[0]["rate"].to_string(value);
                }catch(const Exception& er){
                        cerr << "Error: " << er.what() << endl;
                }
                if(value.empty()) value = "No Data !!";
                return value;
        }
        else{
                return "Invalid Command !!";
        }
}

string FxService::operateRequestLatestChange(string from, string to)
{
        if(!from.empty() && !to.empty())
        {
                string value = "";
                try{
                        string sql = "select rate from tblExchangeRate "
                                        "where `from` = '%1%' and `to` = '%2%' "
                                        "order by date desc "
                                        "limit 0,2";
                        sql = str(format(sql) % from % to);
                        StoreQueryResult res = loadDataTable(sql);
                        if(res.size()==0)
                                value = "0";
                        else if(res.size()==1)
                                res[0]["rate"].to_string(value);
                        else if(res.size()==2){
                                double r0, r1, change;
                                r0 = res[0]["rate"];
                                r1 = res[1]["rate"];
                                change = r0-r1;
                                value = lexical_cast<string>(change);
                        }

                }catch(const Exception& er){
                        cerr << "Error: " << er.what() << endl;
                }
                if(value.empty()) value = "No Data !!";
                return value;
        }
        else{
                return "Invalid Command !!";
        }
}

string FxService::operateUpdateAccountDatum(string from, double famount)
{
        string sql = "update tblFxAccount set amount = %1% where currency = '%2%'";
        sql = str(format(sql) % famount % from);
        if(executeQuery(sql))
                return "Update Account Datum Complete !";
        else
                return "Update Account Datum Error !";
}

string FxService::operateEnterOrder(string buy_sell, string from, string to, double famount)
{
        ptime timestamp = second_clock::local_time();
        string sql = "insert into tblFxOrder(buy_sell, `from`, `to`, `famount`, `timestamp`) values('%1%' , '%2%' , '%3%', %4%, '%5%')";
        sql = str(format(sql) % buy_sell % from % to % famount % timestamp);
        if(executeQuery(sql))
                cout << "Enter Order Complete !" << endl;
        else
                cout << "Enter Order Error !" << endl;

        if(buy_sell == "SELL"){
                //get account balance of from
                double balance_from = operateRequestAccountDatum(from);
                double balance_to = operateRequestAccountDatum(to);

                //check famount < from
                if(famount > balance_from)
                        return "Not enough balance !!";
                else
                {
                        double rate = lexical_cast<double>(operateRequestLatestRate(from, to));
                        balance_from = balance_from - famount;
                        balance_to = balance_to + (rate * famount);
                        operateUpdateAccountDatum(from, balance_from);
                        operateUpdateAccountDatum(to, balance_to);
                        return "Balance Account Complete !!";
                }
        }
        return "Underconstruction !!";
}

double FxService::operateRequestAccountDatum(string currency)
{
        double amount = 0;
        try{
                string sql = "select amount from tblFxAccount where currency = '%1%'";
                sql = str(format(sql) % currency);
                StoreQueryResult res = loadDataTable(sql);
                if(res.size()>0)
                        amount = res[0]["amount"];
        }catch(const Exception& er){
                cerr << "Error: " << er.what() << endl;
        }
        return amount;
}

string FxService::operateRequestAccountData()
{
        string value = "";
        try{
                string sql = "select currency, amount from tblFxAccount ";
                StoreQueryResult res = loadDataTable(sql);
                string currency;
                double amount;
                vector<string> tuples;
                string tuple = "%1%,%2%";
                for(size_t i = 0; i<res.size(); i++){
                        res[i]["currency"].to_string(currency);
                        amount = res[i]["amount"];
                        cout << str(format(tuple) % currency % amount) << endl;
                        tuples.push_back(str(format(tuple) % currency % amount));
                }
                value = boost::algorithm::join(tuples, "/");
        }catch(const Exception& er){
                cerr << "Error: " << er.what() << endl;
        }
        if(value.empty()) value = "No Data !!";
        return value;
}

string FxService::operateCancelOrder(string id)
{
        string query = "update tblFxOrder set cancel_flag = 1 where id = %s{0}";
        //query = format(query, id);
        string result = "this is cancel order";//execute(query);
        return result;
}


string FxService::operateIncomingRequest(const IncomingRequest& incomingRequest)
{
        switch(incomingRequest.request_type){
                case IncomingRequest::RequestMarketData:
                        return operateRequestMarketData(incomingRequest.from, incomingRequest.to, incomingRequest.date
                                , incomingRequest.start_date, incomingRequest.end_date);
                case IncomingRequest::RequestLatestRate:
                        return operateRequestLatestRate(incomingRequest.from, incomingRequest.to);
                case IncomingRequest::RequestLatestChange:
                        return operateRequestLatestChange(incomingRequest.from, incomingRequest.to);
                case IncomingRequest::RequestAccountData:
                        return operateRequestAccountData();
                case IncomingRequest::RequestAccountDatum:
                        return lexical_cast<string>(operateRequestAccountDatum(incomingRequest.from));
                case IncomingRequest::EnterOrder:
                        cout<<incomingRequest.buy_sell;
                        return operateEnterOrder(incomingRequest.buy_sell, incomingRequest.from, incomingRequest.to, incomingRequest.famount);
                case IncomingRequest::CancelOrder:
                        return operateCancelOrder(incomingRequest.id);
                default: return "incorrect request!!";
        }
}

string FxService::operateRequest(string request)
{
        IncomingRequest incomingRequest = IncomingRequest(request);
        return operateIncomingRequest(incomingRequest);
}

