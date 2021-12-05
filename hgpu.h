/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _HGPUS_H_
#define _HGPUS_H_

//#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <gtkmm/comboboxtext.h>
#include <list>

using Vga_adapters = struct _vga_adapters {
      int number = 0;
      std::string adapter_name = "";
      std::string adapter_link = "";
      std::string adapter_set = "";
};

class CGpus
{
public:
   void Gpus_Info_Init();
   void GpuComboInit(Gtk::ComboBoxText &combo) const;
   std::string GpuStatus(int pos) const;
   std::string CurrentPcieLinkStatus(int pos = 0) const;
   unsigned int check_adapters() const {return m_vga_adapters.size();}
private:
   std::list<Vga_adapters> m_vga_adapters;
};

#endif // _HGPUS_H_
