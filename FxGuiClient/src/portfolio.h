/*
 * portfolio.h
 *
 *  Description: Render portfolio section
 *  Created on: Nov 11, 2012
 *      Author: nirin
 */

#ifndef Portfolio_H_
#define Portfolio_H_

#include <vector>
#include <gtkmm.h>
#include <gdkmm.h>
#include <gtkmm/drawingarea.h>
#include <cairo.h>
#include <cairomm/context.h>
#include <glib.h>
#include <sigc++.h>
#include <signal.h>

using namespace std;

class Portfolio : public Gtk::Paned{
public:
	Portfolio();
	virtual ~Portfolio();
	void reflesh();

protected:
  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_currency); add(m_col_amount); add(m_col_percentage);}

    Gtk::TreeModelColumn<Glib::ustring> m_col_currency;
    Gtk::TreeModelColumn<double> m_col_amount;
    Gtk::TreeModelColumn<int> m_col_percentage;
  };

  ModelColumns m_Columns;

  //Child widgets:
  Gtk::VBox m_VBox;
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

};

#endif /* Portfolio_H_ */
