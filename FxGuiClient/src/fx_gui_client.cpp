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
  //Shows the window and returns when it is closed.
  Gtk::Main::run(window);

  return 0;
}
