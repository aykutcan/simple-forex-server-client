/*
 * FxDataGrid.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: nirin
 */

#include "price_datagrid.h"
#include "outgoing_request.h"
#include <cstdlib>
#include <iostream>
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <glib.h>
#include <cairo.h>
#include <gtkmm.h>
#include <gdkmm.h>
#include <glib.h>
#include <sigc++.h>
#include <signal.h>
#include <vector>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;


PriceDataGrid::PriceDataGrid(int rows, int cols)
: grid_Table(rows, cols, true),
  symbol_column(6),
  rate_column(6),
  change_column(6),
  bid_column(6),
  ask_column(6)
{
	add(grid_Table);
	vector<pair<string, string> > symbols;
	pair<string, string> symbol1("USD","JPY");
	pair<string, string> symbol2("USD","EUR");
	pair<string, string> symbol3("USD","GBP");
	pair<string, string> symbol4("USD","CHF");
	pair<string, string> symbol5("EUR","JPY");
	symbols.push_back(symbol1);
	symbols.push_back(symbol2);
	symbols.push_back(symbol3);
	symbols.push_back(symbol4);
	symbols.push_back(symbol5);

	symbol_header.set_text("Symbol");
	rate_header.set_text("Rate");
	change_header.set_text("Change");
	bid_header.set_text("Bid");
	ask_header.set_text("Ask");

	for(size_t i=0; i<symbols.size(); i++){
		string symbol = symbols.at(i).first  +"/"+ symbols.at(i).second;
		cout << symbol << endl;
		symbol_column.at(i) = new Gtk::Label();
		rate_column.at(i) = new Gtk::Label();
		change_column.at(i) = new Gtk::Label();
		bid_column.at(i) = new Gtk::Label();
		ask_column.at(i) = new Gtk::Label();

		symbol_column.at(i)->set_text(symbol);
		rate_column.at(i)->set_text(requestLatestRate(symbols.at(i).first,  symbols.at(i).second));
		change_column.at(i)->set_text(requestLatestChange(symbols.at(i).first,  symbols.at(i).second));
		bid_column.at(i)->set_text(requestLatestAsk(symbols.at(i).first,  symbols.at(i).second));
		ask_column.at(i)->set_text(requestLatestBid(symbols.at(i).first,  symbols.at(i).second));

	}

	grid_Table.attach(symbol_header, 0, 1, 0, 1);
	grid_Table.attach(rate_header, 1, 2, 0, 1);
	grid_Table.attach(change_header, 2, 3, 0, 1);
	grid_Table.attach(bid_header, 3, 4, 0, 1);
	grid_Table.attach(ask_header, 4, 5, 0, 1);

	for(size_t i=0; i<symbols.size(); i++){
		grid_Table.attach(*symbol_column.at(i), 0, 1, i+1, i+2);
		grid_Table.attach(*rate_column.at(i), 1, 2, i+1, i+2);
		grid_Table.attach(*change_column.at(i), 2, 3, i+1, i+2);
		grid_Table.attach(*bid_column.at(i), 3, 4, i+1, i+2);
		grid_Table.attach(*ask_column.at(i), 4, 5, i+1, i+2);
	}
}

PriceDataGrid::~PriceDataGrid() {
	// TODO Auto-generated destructor stub
}

string PriceDataGrid::requestLatestRate(string from, string to)
{
	string reply = OutgoingRequest::requestLatestRate(from, to);
	reply = reply.substr(0, 5);
	return reply;
}

string PriceDataGrid::requestLatestChange(string from, string to)
{
	string reply = OutgoingRequest::requestLatestChange(from, to);
	reply = reply.substr(0, 5);
	return reply;
}

string PriceDataGrid::requestLatestAsk(string from, string to)
{
	return "0.00";
}

string PriceDataGrid::requestLatestBid(string from, string to)
{
	return "0.00";
}


