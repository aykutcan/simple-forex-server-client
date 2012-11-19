/*
 * fx_gui_client.h
 *
 *  Description: Main program
 *  Created on: Nov 3, 2012
 *      Author: nirin
 */

#include "fx_client_window.h"
#include <gtkmm/main.h>

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  FxClientWindow window;
  Gtk::Main::run(window);
  return 0;
}
