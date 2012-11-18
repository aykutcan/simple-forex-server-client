//============================================================================
// Name        : FxGuiClient.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

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
