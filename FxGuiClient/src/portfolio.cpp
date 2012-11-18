/*
 * FxPortfolio.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: nirin
 */

#include "portfolio.h"
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
#include<boost/range/numeric.hpp>

using namespace std;
using namespace boost;

Portfolio::Portfolio()
{
	  add(m_VBox);
	  m_ScrolledWindow.add(m_TreeView);
	  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	  m_VBox.pack_start(m_ScrolledWindow);
	  m_refTreeModel = Gtk::ListStore::create(m_Columns);
	  m_TreeView.set_model(m_refTreeModel);

	  //Add the TreeView's view columns:
	  //m_TreeView.append_column("Currency", m_Columns.m_col_currency);
	  //m_TreeView.append_column("Amount", m_Columns.m_col_amount);
	  //m_TreeView.append_column_numeric("Amount", m_Columns.m_col_amount,
	  //        "%010d" /* 10 digits, using leading zeroes. */);
	  //Display a progress bar instead of a decimal number:
	  //Gtk::CellRendererProgress* cell = Gtk::manage(new Gtk::CellRendererProgress);

	  //cout << cols_count << endl;
	  //Gtk::CellRendererProgress* cell = Gtk::manage(new Gtk::CellRendererProgress);
	  //int cols_count = m_TreeView.append_column("Percentage", *cell);

	  reflesh();
}

void Portfolio::reflesh()
{
	  //Fill the TreeView's model
      m_refTreeModel->clear();
      m_TreeView.remove_all_columns();

	  //Add the TreeView's view columns:
	  m_TreeView.append_column("Currency", m_Columns.m_col_currency);
	  m_TreeView.append_column("Amount", m_Columns.m_col_amount);

	  vector<AccountDatum*> accountData = OutgoingRequest::requestAccountData();
	  vector<double> in_yens;
	  vector<double> percentages;
	  double sum;

	  for(size_t i=0; i<accountData.size(); i++)
	  {
		  string symbol = accountData.at(i)->currency;
		  double rate = lexical_cast<double>(OutgoingRequest::requestLatestRate(symbol, "JPY"));
		  double in_yen = accountData.at(i)->amount * rate;
		  in_yens.push_back(in_yen);
	  }

	  sum = boost::accumulate(in_yens, 0);

	  for(size_t i=0; i<accountData.size(); i++)
	  {
		  double percentage = in_yens.at(i)/sum * 100;
		  cout << percentage << endl;
		  percentages.push_back(percentage);
	  }

	  Gtk::TreeModel::Row row;
	  for(size_t i=0; i<accountData.size(); i++){
		  row = *(m_refTreeModel->append());
		  row[m_Columns.m_col_currency] = accountData.at(i)->currency;
		  row[m_Columns.m_col_amount] = accountData.at(i)->amount;
		  row[m_Columns.m_col_percentage] = percentages.at(i);
	  }

	  Gtk::CellRendererProgress* cell = Gtk::manage(new Gtk::CellRendererProgress);
	  int cols_count = m_TreeView.append_column("Percentage", *cell);
	  Gtk::TreeViewColumn* pColumn = m_TreeView.get_column(cols_count - 1);
	  if(pColumn)
	  {
	    pColumn->add_attribute(cell->property_value(), m_Columns.m_col_percentage);
	  }

	  //Make all the columns reorderable:
	  //This is not necessary, but it's nice to show the feature.
	  //You can use TreeView::set_column_drag_function() to more
	  //finely control column drag and drop.
	  for(guint i = 0; i < 2; i++)
	  {
	    Gtk::TreeView::Column* pColumn = m_TreeView.get_column(i);
	    pColumn->set_reorderable();
	  }

	  show_all_children();
}

Portfolio::~Portfolio() {
	// TODO Auto-generated destructor stub
}

