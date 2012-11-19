/*
 * fx_client_window.cpp
 *
 *  Description: Assemble gui widgets in main window
 *  Created on: Nov 3, 2012
 *      Author: nirin
 */

#include "fx_client_window.h"
#include "outgoing_request.h"
#include <gtkmm.h>
#include <glib.h>
#include <sigc++.h>
#include <signal.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <vector>

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;
using namespace boost::posix_time;

FxClientWindow::FxClientWindow()
: m_Table(24, 16, true),
  left_Table(20, 8, true),
  right_Table(20, 8, true),
  graph_Table(8, 2, true),
  graph_ctrl_Table(8, 2, true),
  graph_period_Table(1, 8, true),
  order_Table(4, 4, true),
  report_Table(2, 2, true),
  portfolio_Table(4, 4, true),
  buttom_Table(2, 8, true),
  symbol11_Combo(true),
  symbol12_Combo(true),
  symbol21_Combo(true),
  symbol22_Combo(true),
  indicator_Combo(true),
  buy_Button("BUY"),
  sell_Button("SELL"),
  history_Graph(),
  price_Datagrid(6, 6)
{
  set_size_request(900, 600);
  set_title("FxGuiClient Version 1.0.0");

  add(m_Table);
  m_Table.attach(left_Table, 0, 8, 0, 20);
  m_Table.attach(right_Table, 8, 16, 0, 20);
  m_Table.attach(buttom_Table, 0, 16, 20, 24);

  left_Table.attach(graph_ctrl_Table, 0, 8, 0, 8);
  left_Table.attach(graph_Table, 0, 8, 8, 20);

  //Gtk::Calendar cal;
  //cal.set_visible(true);
  //right_Table.attach(cal, 0, 8, 0, 8);

  right_Table.attach(portfolio_Table, 0, 8, 0, 8);
  right_Table.attach(order_Table, 0, 8, 8, 12);
  right_Table.attach(report_Table, 0, 8, 12, 20);

  graph_ctrl_Table.attach(symbol11_Combo, 0, 1, 0, 2);
  graph_ctrl_Table.attach(symbol12_Combo, 0, 1, 2, 4);
  graph_ctrl_Table.attach(go_Button, 1, 2, 0, 4);
  graph_ctrl_Table.attach(indicator_Combo, 0, 1, 6, 7);
  graph_ctrl_Table.attach(graph_period_Table, 0, 2, 7, 8);
  graph_period_Table.attach(one_week_Button, 0, 1, 0, 1);
  graph_period_Table.attach(one_month_Button, 1, 2, 0, 1);
  graph_period_Table.attach(three_months_Button, 2, 3, 0, 1);
  graph_period_Table.attach(one_year_Button, 3, 4, 0, 1);
  graph_Table.attach(history_Graph, 0, 2, 0, 8);

  order_Table.attach(symbol21_Combo, 0, 2, 0, 1);
  order_Table.attach(symbol22_Combo, 2, 4, 0, 1);
  order_Table.attach(src_amount_Entry, 0, 2, 1, 2);
  order_Table.attach(des_amount_Entry, 2, 4, 1, 2);
  order_Table.attach(buy_Button, 0, 2, 2, 4);
  order_Table.attach(sell_Button, 2, 4, 2, 4);

  buttom_Table.attach(cmd_Label, 0, 1, 0, 1);
  buttom_Table.attach(res_Label, 0, 1, 1, 2);
  buttom_Table.attach(cmd_Entry, 1, 6, 0, 1);
  buttom_Table.attach(res_Entry, 1, 6, 1, 2);
  buttom_Table.attach(submit_Button, 7, 8, 0, 1);

  report_Table.attach(price_Datagrid, 0, 2, 0, 2);
  portfolio_Table.attach(fx_Portfolio, 0, 4, 0, 4);

  cmd_Label.set_text("command : ");
  cmd_Entry.set_max_length(200);
  cmd_Entry.set_text("-- type command --");
  cmd_Entry.select_region(0, cmd_Entry.get_text_length());

  res_Label.set_text("result : ");
  res_Entry.set_max_length(200);
  res_Entry.set_text("");

  submit_Button.set_label("Submit");

  vector<string> symbols;
  symbols.push_back("USD");
  symbols.push_back("EUR");
  symbols.push_back("GBP");
  symbols.push_back("JPY");

  for(size_t i=0; i<symbols.size(); i++){
	  symbol11_Combo.append(symbols.at(i));
	  symbol12_Combo.append(symbols.at(i));
	  symbol21_Combo.append(symbols.at(i));
	  symbol22_Combo.append(symbols.at(i));
  }

  vector<string> indicators;
  indicators.push_back("SMA");
  indicators.push_back("RSI");
  for(size_t i=0; i<indicators.size(); i++){
	  indicator_Combo.append(indicators.at(i));
  }

  go_Button.set_label("GO");
  one_week_Button.set_label("1 week");
  one_month_Button.set_label("1 month");
  three_months_Button.set_label("3 months");
  one_year_Button.set_label("1 year");





  submit_Button.signal_clicked().connect(sigc::mem_fun(*this,
		  &FxClientWindow::on_command_submitted));
  one_week_Button.signal_clicked().connect(sigc::mem_fun(*this,
		  &FxClientWindow::on_one_week_button_clicked));
  one_month_Button.signal_clicked().connect(sigc::mem_fun(*this,
		  &FxClientWindow::on_one_month_button_clicked));
  three_months_Button.signal_clicked().connect(sigc::mem_fun(*this,
		  &FxClientWindow::on_three_months_button_clicked));
  one_year_Button.signal_clicked().connect(sigc::mem_fun(*this,
		  &FxClientWindow::on_one_year_button_clicked));
  buy_Button.signal_clicked().connect(sigc::mem_fun(*this,
		  &FxClientWindow::on_order_buy_button_clicked));
  sell_Button.signal_clicked().connect(sigc::mem_fun(*this,
		  &FxClientWindow::on_order_sell_button_clicked));

  show_all_children();
}

FxClientWindow::~FxClientWindow()
{
}

void FxClientWindow::redrawGraph(string data){
	history_Graph.setHistoryData(data);
	history_Graph.setIndicatorType(indicator_Combo.get_entry_text());
	history_Graph.queue_draw();
}

void FxClientWindow::on_one_week_button_clicked()
{
	string from = symbol11_Combo.get_entry_text();
	string to = symbol12_Combo.get_entry_text();
	string data = OutgoingRequest::requestMarketDataDuringPeriod(from, to, OutgoingRequest::OneWeek);
	redrawGraph(data);
}

void FxClientWindow::on_one_month_button_clicked()
{
	string from = symbol11_Combo.get_entry_text();
	string to = symbol12_Combo.get_entry_text();
	string data = OutgoingRequest::requestMarketDataDuringPeriod(from, to, OutgoingRequest::OneMonth);
	redrawGraph(data);
}

void FxClientWindow::on_three_months_button_clicked()
{
	string from = symbol11_Combo.get_entry_text();
	string to = symbol12_Combo.get_entry_text();
	string data = OutgoingRequest::requestMarketDataDuringPeriod(from, to, OutgoingRequest::ThreeMonths);
	redrawGraph(data);
}

void FxClientWindow::on_one_year_button_clicked()
{
	string from = symbol11_Combo.get_entry_text();
	string to = symbol12_Combo.get_entry_text();
	string data = OutgoingRequest::requestMarketDataDuringPeriod(from, to, OutgoingRequest::OneYear);
	redrawGraph(data);
}

void FxClientWindow::on_order_buy_button_clicked()
{
	string from = symbol21_Combo.get_entry_text();
	string to = symbol22_Combo.get_entry_text();
	string famount = src_amount_Entry.get_text();
	string reply = OutgoingRequest::enterOrderBuy(from, to, famount);
	res_Entry.set_text(reply);
}

void FxClientWindow::on_order_sell_button_clicked(){
	string from = symbol21_Combo.get_entry_text();
	string to = symbol22_Combo.get_entry_text();
	string famount = src_amount_Entry.get_text();
	string reply = OutgoingRequest::enterOrderSell(from, to, famount);
	res_Entry.set_text(reply);
	fx_Portfolio.reflesh();
}

void FxClientWindow::on_command_submitted()
{
	string cmd = cmd_Entry.get_text();
	string reply = OutgoingRequest::requestByCommandLine(cmd);
	res_Entry.set_text(reply);
}


