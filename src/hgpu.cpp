/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "hgpu.h"
#include "mwnd.h"

void CGpus::Gpus_Info_Init()
{
   std::string speed(""),device(""),swidth("/sbin/lspci");
   Vga_adapters adapter;

   if(!uhiutil::ExistenceVerification(swidth.c_str())) swidth = "lspci";
   std::string vgadapters = uhiutil::execmd((swidth + " -D | grep 'VGA compatible controller:'").c_str());
   vgadapters.append(uhiutil::execmd((swidth + " -D | grep 'Display controller:'").c_str()));
   std::istringstream input(vgadapters);
   for(std::string line; std::getline(input, line);){
          uhiutil::end_intervals_remove(line);
          device = line;
          if(uhiutil::newline(line,"VGA compatible controller:",Direction::RIGHT))
                  uhiutil::newline(device,"VGA compatible controller:",Direction::LEFT);
          else
              if(uhiutil::newline(line,"Display controller:",Direction::RIGHT))
                        uhiutil::newline(device,"Display controller:",Direction::LEFT);
              else continue;
          
          uhiutil::newline((adapter.adapter_name = line),"(rev",Direction::LEFT);
          adapter.adapter_link = "/sys/bus/pci/devices/" + device;

          speed = "cat " + adapter.adapter_link + "/max_link_speed";
          uhiutil::end_intervals_remove((speed = uhiutil::execmd(speed.c_str())));
          swidth = "cat " + adapter.adapter_link + "/max_link_width";
          uhiutil::end_intervals_remove((swidth = uhiutil::execmd(swidth.c_str())));

          device = "cat " + adapter.adapter_link + "/uevent | grep DRIVER";
          device = uhiutil::execmd(device.c_str());
          uhiutil::end_intervals_remove(device);
          if(uhiutil::newline(device,"DRIVER",Direction::RIGHT)) {
                 if(device == "nvidia") {
                       line = uhiutil::execmd("cat /sys/module/nvidia/version");
                       uhiutil::end_intervals_remove(line);
                       device.append("  " + line);
                 }
          }

          adapter.adapter_set = ("PCIE link max : x" + swidth + " / "  + speed + "\n" + "Driver : " + device);
          adapter.number = m_vga_adapters.size() + 1;
          m_vga_adapters.push_back(adapter);
   }
}

std::string CGpus::CurrentPcieLinkStatus(int pos) const
{
     std::string ret("");
     for(Vga_adapters adp : m_vga_adapters){
              if((adp.number - 1) == pos) {
                    std::string speed = "cat " + adp.adapter_link + "/current_link_speed";
                    uhiutil::end_intervals_remove((speed = uhiutil::execmd(speed.c_str())));
                    std::string width = "cat " + adp.adapter_link + "/current_link_width";
                    uhiutil::end_intervals_remove((width = uhiutil::execmd(width.c_str())));
                    ret =  ("x" + width + " / " + speed);
                    break;
              }
     }

     return ret;
}

std::string CGpus::GpuStatus(int pos) const
{
     std::string ret(""),mntind("");
     for(Vga_adapters adp : m_vga_adapters){
             if((adp.number - 1) == pos) {
                  ret = adp.adapter_set;
                  unsigned int height,width;
                  uhiutil::GetDesktopSize(&height,&width);
                  if(height > 0 && width > 0) ret.append(std::string("\n") + _("Desktop resolution: ") + std::to_string(width) + "x" + std::to_string(height));
                  break;
              }
     }
     return ret;
}

void CGpus::GpuComboInit(Gtk::ComboBoxText &combo) const
{
    for(Vga_adapters adp : m_vga_adapters){
           combo.append(adp.adapter_name);
    }

    if(m_vga_adapters.size())
           combo.set_active(0);
}
