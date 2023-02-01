/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "mwnd.h"
#include "hInfapp.h"

int main (int argc, char* argv[])
{
	   std::locale lc;
	   try{
		     lc = std::locale("en_US");
	     }
	     catch(std::runtime_error const&){
	         lc = std::locale(lc,"",std::locale::all);
	     }

	   std::locale::global(lc);

       std::string command = "ps -p " + std::to_string(getpid()) + " -o comm=";
       int res{std::string{"uhInfo\n"}.compare(uhiutil::execmd(command.data()))};
       if(res) return 0;
       else
          if(std::atoi((std::string(uhiutil::execmd("pgrep -c uhInfo"))).data()) != 1)  return 0;
          else {	
               Glib::RefPtr<ChInfApp> app = ChInfApp::create("org.gtkmm.uhInfo");
               return app->make_window_and_run<CHWindow>(argc, argv);
          }
       return 0;
}
