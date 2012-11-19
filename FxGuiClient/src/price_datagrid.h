/*
 * price_datagrid.h
 *
 *  Description: Render price data grid; latest, change, bid, ask
 *  Created on: Nov 6, 2012
 *      Author: nirin
 */

#ifndef Price_Datagrid_H_
#define Price_Datagrid_H_

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

using namespace std;

class PriceDataGrid : public Gtk::Paned{
public:
	PriceDataGrid(int rows, int cols);
	virtual ~PriceDataGrid();

private:
	int rows;
	int cols;

	Gtk::Table grid_Table;
	Gtk::Label symbol_header;
	Gtk::Label rate_header;
	Gtk::Label change_header;
	Gtk::Label ask_header;
	Gtk::Label bid_header;

	vector<Gtk::Label*> symbol_column;
	vector<Gtk::Label*> rate_column;
	vector<Gtk::Label*> change_column;
	vector<Gtk::Label*> ask_column;
	vector<Gtk::Label*> bid_column;
	string requestLatestRate(string from, string to);
	string requestLatestChange(string from, string to);
	string requestLatestAsk(string from, string to);
	string requestLatestBid(string from, string to);

};

#endif /* Price_Datagrid_H_ */
