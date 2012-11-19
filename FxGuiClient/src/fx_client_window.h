/*
 * fx_client_window.h
 *
 *  Description: Assemble gui widgets in main window
 *  Created on: Nov 3, 2012
 *      Author: nirin
 */

#ifndef FXCLIENTWINDOW_H_
#define FXCLIENTWINDOW_H_

#include <gtkmm.h>
#include "history_graph.h"
#include "price_datagrid.h"
#include "portfolio.h"

using namespace std;
//#define FX_SERVER "localhost"
//#define PORT "8090"

class FxClientWindow : public Gtk::Window
{
public:
  FxClientWindow();
  virtual ~FxClientWindow();
//  enum { max_length = 1024*1024 };

protected:
  //Signal handlers:
  void on_command_submitted();
  void on_one_week_button_clicked();
  void on_one_month_button_clicked();
  void on_three_months_button_clicked();
  void on_one_year_button_clicked();
  void on_order_buy_button_clicked();
  void on_order_sell_button_clicked();

  void redrawGraph(string data);

  void on_checkbox_editable_toggled();
  void on_checkbox_visibility_toggled();
  void on_button_close();

  //Child widgets:
  Gtk::Table m_Table;
  Gtk::Table left_Table;
  Gtk::Table right_Table;

  Gtk::Table graph_Table;
  Gtk::Table graph_ctrl_Table;
  Gtk::Table graph_period_Table;
  Gtk::Table order_Table;
  Gtk::Table report_Table;
  Gtk::Table portfolio_Table;
  Gtk::Table buttom_Table;

  Gtk::Label cmd_Label;
  Gtk::Entry cmd_Entry;
  Gtk::Label res_Label;
  Gtk::Entry res_Entry;
  Gtk::Button submit_Button;

  Gtk::Button one_week_Button;
  Gtk::Button one_month_Button;
  Gtk::Button three_months_Button;
  Gtk::Button one_year_Button;

  Gtk::ComboBoxText symbol11_Combo;
  Gtk::ComboBoxText symbol12_Combo;
  Gtk::ComboBoxText symbol21_Combo;
  Gtk::ComboBoxText symbol22_Combo;
  Gtk::ComboBoxText indicator_Combo;

  Gtk::Button go_Button;
  Gtk::Entry src_amount_Entry;
  Gtk::Entry des_amount_Entry;
  Gtk::Button buy_Button;
  Gtk::Button sell_Button;

  HistoryGraph history_Graph;
  PriceDataGrid price_Datagrid;
  Portfolio fx_Portfolio;
};




#endif /* FXCLIENTWINDOW_H_ */
