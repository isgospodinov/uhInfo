/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "mwnd.h"
#include <glibmm/i18n.h>

void CInitThread::FinishThreadAndClear()
{
    if(th_fstate.valid() && !th_fstate.get())
                              CLEAR_THREAD_CONDITIONS;
}

void CInitThread::CreateThread(CHWindow *m_wnd) 
{
    th_fstate = std::async(std::launch::async,[m_wnd](CHWindow *){ m_wnd->Init(m_wnd); return 0;},m_wnd);
}

void CInitThread::VeryFastQuitGuard() const
{
    if(th_fstate.valid())
          while(th_fstate.wait_for(std::chrono::milliseconds(50)) != std::future_status::ready); //Async init task/thread very fast quit guard
}

void CInitThread::PrepAndMakeThread(CHWindow *m_wnd,fp_DispNtf func)
{
   if(!th_fstate.valid()) {
        CreateThreadDispatcher();
        if(m_Dispatch)
               m_Connection = std::unique_ptr<sigc::connection>(&(CONNECTIONSET = m_Dispatch->connect(sigc::mem_fun(*m_wnd,func))));

        if(CONNECTIONSTATUS(m_Connection)) CreateThread(m_wnd);
   }
}

std::string CInitThread::MOBO_Info() const
{
   std::string mbname = uhiutil::execmd("cat /sys/class/dmi/id/board_name");
   uhiutil::end_intervals_remove(mbname);

   std::string vendor = uhiutil::execmd("cat /sys/class/dmi/id/board_vendor");
   uhiutil::end_intervals_remove(vendor);

   std::string bios = uhiutil::execmd("cat /sys/class/dmi/id/bios_version");
   uhiutil::end_intervals_remove(bios);

   std::string line(""),dpos("/sbin/lspci");
   if(!uhiutil::ExistenceVerification(dpos.c_str())) dpos = "lspci";
   std::istringstream inputstrm(uhiutil::execmd((dpos + " -D | grep 'Host bridge:'").c_str()));
   while(std::getline(inputstrm, line)) {
        if(uhiutil::newline(line,"Host bridge:",Direction::RIGHT)) {
            if(uhiutil::newline(line,"Host Bridge",Direction::LEFT)) {
                   if(uhiutil::newline(line,"Intel Corporation",Direction::RIGHT)) {
                       line = ("Intel " +  line);
                       if(!uhiutil::newline(line,"v5/6th",Direction::LEFT))
                             uhiutil::newline(line,"v6/7th",Direction::LEFT);
                   }
                   else
                	   if(uhiutil::newline(line,"[AMD",Direction::RIGHT))
                             line = ("AMD " +  line);
                   break;
            }
        }
   }

   return ( _("Mainboard: ") + mbname + "\n" + _("Vendor: ") + vendor + "\n" + _("BIOS ver. : ") + bios + (line.size() ? (std::string("\n") + std::string(_("Chipset : ")) + line) : ""));
}

std::string CInitThread::GetDevices(DeviceType dType) const
{
   std::string ret(""),dpos("/sbin/lspci");
   bool pass_counter = false;
   if(!uhiutil::ExistenceVerification(dpos.c_str())) dpos = "lspci";
   std::istringstream input(uhiutil::execmd((dpos + std::string( dType == DeviceType::AUDIO ? " -D | grep " : " | grep -Ei ") + (dType == DeviceType::AUDIO ? "'Audio device:'" : "'Network controller|Ethernet controller'")).c_str()));
   for(std::string line; std::getline(input, line);) {
        if(pass_counter) ret += "\n";
        if(!uhiutil::newline(line,(dType == DeviceType::AUDIO ? "Audio device:" : " controller:"),Direction::RIGHT)) {
             ret += "Unknown device";
             if(!pass_counter) pass_counter = true;
             continue;
        }
        else
            if(uhiutil::newline(line,"Realtek Semiconductor Co., Ltd.",Direction::RIGHT)) line = ("Realtek " +  line);
            else
            	if(uhiutil::newline(line,"Broadcom Inc. and subsidiaries",Direction::RIGHT)) line = ("Broadcom Inc. " +  line);

        uhiutil::newline(line,"(rev",Direction::LEFT);
        ret += line;
        if(!pass_counter) pass_counter = true;
   }

   return ret;
}

const std::string CInitThread::MemfDefs(std::string mfsp,int *const banks) const
{
	std::string retres(""),res = uhiutil::execmd(std::string(std::string("udevadm info -e | grep -e ") + mfsp + std::string("=")).c_str());
	   if(res.length() > 0) {
	       std::string tb("");
	       std::istringstream input(res);
	       for(;std::getline(input, tb);) {
	           uhiutil::newline(tb,mfsp.c_str(),Direction::RIGHT);
	           if(tb != "Unknown" && tb != "Not Specified" && ((int)retres.find(tb)) == -1) {
	        	   retres.append((((retres.length() > 0) ? "," : "") + tb));
	           }

        	   if(banks && tb != "Unknown" && tb != "Not Specified") (*banks)++;
	       }
	   }

	   return retres;
}

std::string CInitThread::Mem_Info() const
{
   std::string mem_path("/sys/devices/system/memory"),mem_size("");
   unsigned long long mem_val(0);
   if(uhiutil::ExistenceVerification(mem_path.c_str()) && 
                          uhiutil::ExistenceVerification((mem_path + "/block_size_bytes").c_str()) ) {
   
       mem_val = std::stoull(uhiutil::execmd(("cat " + mem_path + "/block_size_bytes").c_str()),nullptr,16);
       DIR *dirp = opendir(mem_path.c_str()); 
       unsigned int dir_counter = 0;
       struct dirent *cd = 0;
       
       while((cd = readdir(dirp)) != 0)  {
           mem_size = mem_path + "/"+ std::string(cd->d_name) + "/online";
           if(uhiutil::ExistenceVerification(mem_size.c_str()) && 
                          std::atoi((uhiutil::execmd(("cat " + mem_size).c_str())).c_str()) == 1) {
                       dir_counter++;
           }
       } 
       closedir(dirp);
       mem_val *= dir_counter;
       if(mem_val) {
       
           if(mem_val < KB1) {
               mem_size =  std::to_string((unsigned int) std::ceil(mem_val)) + " B";
           }
           else
               if(mem_val < MB1) {
                   mem_size =  std::to_string((unsigned int) std::ceil((float)mem_val / (float)KB1)) + " KB";
               }
               else
                   if(mem_val < GB1) {
                       mem_size =  std::to_string((unsigned int) std::ceil((float)mem_val / (float)MB1)) + " MB";
                   }
                   else {
                       mem_size =  std::to_string((unsigned int) std::ceil((float)mem_val / (float)GB1)) + " GB"; 
                   }
       }
       else
           mem_size =  "";
   } 

   int bk = 0;
   std::string csmts(""),rk("");
   mem_path = uhiutil::execmd("printenv PATH");
   uhiutil::end_intervals_remove(mem_path);
   std::istringstream tsin(mem_path);
   mem_path.clear();

   for(std::string line; std::getline(tsin, line,':'); ) {
      if(uhiutil::ExistenceVerification(std::string(line + "/udevadm").c_str())) {
    	   csmts = MemfDefs("CONFIGURED_SPEED_MTS",&bk);
    	   mem_path =  MemfDefs("PART_NUMBER");
    	   rk =  MemfDefs("RANK");
    	   break;
      }
   }
    
   return ((!mem_size.empty() ? _("Memory : ") + mem_size : "" ) + (!mem_path.empty() ? (std::string("\n") + _("Part No : ") + mem_path) : "") +
		                 (!csmts.empty() ? (std::string("\n") + _("Speed : ") + csmts + " MT/s") : "") + (bk ? (std::string("\n") + _("Populated banks : ") + std::to_string(bk)) : "") + (!rk.empty() ? (std::string("\n") + _("Ranks : ") + rk) : ""));
}

std::string CInitThread::OS_Info() const
{
   int posf = 0,pid = 0;
   std::string workbuffer("");
   std::string os(""),osname("Unknown"),osver("Unknown"),kern(""),sesid(""),sescheck(""),gccver(""),deskenv("");

   std::istringstream osninput(uhiutil::execmd("cat /etc/os-release | grep NAME="));
   for(;std::getline(osninput, osname);) {
       if(((int)osname.find("NAME=")) == 0 || ((int)osname.find("TAILS_PRODUCT_NAME=")) == 0) {
              uhiutil::newline(osname,"=",Direction::RIGHTSKIP);
              uhiutil::end_intervals_remove(osname);
              break;
       }
   }

   osver = uhiutil::execmd("cat /etc/os-release | grep VERSION_ID=");
   uhiutil::newline(osver,"VERSION_ID",Direction::RIGHT);
   uhiutil::end_intervals_remove(osver);
   os = osname + " " + osver;
   while((posf = os.find('"')) != -1) {os.replace(posf,1,"");}

   kern = uhiutil::execmd("uname -r");
   uhiutil::end_intervals_remove(kern);

   if((deskenv = uhiutil::execmd("printenv | grep XDG_CURRENT_DESKTOP")) == "")
                   pid = std::atoi((uhiutil::execmd("ps -C gnome-shell -o pid=")).c_str());

   if(deskenv != "" || pid) {
       if((deskenv.find("GNOME")) || (deskenv.find("gnome")) || pid) 
                                 deskenv = uhiutil::execmd("gnome-shell --version");

       uhiutil::end_intervals_remove(deskenv);
       if(pid) pid = 0;
   }
   else deskenv = "Unknown";

   if((sesid = uhiutil::execmd("printenv | grep XDG_SESSION_ID")) == "")
                   pid = std::atoi((uhiutil::execmd("ps -C Xorg -o pid=")).c_str());

   if(sesid != "" || pid) {
       if(sesid != "") {
           uhiutil::newline(sesid,"XDG_SESSION_ID",Direction::RIGHT);
           uhiutil::end_intervals_remove(sesid);
           sescheck = "loginctl show-session " + sesid + " -p Type";
           workbuffer = uhiutil::execmd(sescheck.c_str());
           uhiutil::newline(workbuffer,"Type",Direction::RIGHT);
           uhiutil::end_intervals_remove(workbuffer);
       }

       if(workbuffer == "x11" || pid) {
    	   std::istringstream Xin("/usr/libexec/Xorg:/usr/lib/xorg/Xorg:/usr/lib/Xorg:/usr/bin/Xorg:/usr/sbin/Xorg");
    	   while(std::getline(Xin, sesid,':')) {
    	   	   if(uhiutil::ExistenceVerification(sesid.c_str()))
    	   		                                               break;
    	   }
           sesid += " -version 2>&1 | grep \"X Server\"";
           uhiutil::newline((osver = uhiutil::execmd(sesid.c_str())),"X Server",Direction::RIGHT);
           uhiutil::end_intervals_remove(osver);
           workbuffer = "Xorg " + osver;
       }
       else workbuffer = "Wayland";
  }
  else workbuffer = "Wayland";

  uhiutil::newline((gccver = uhiutil::execmd( "gcc --version" )),"\n",Direction::LEFTSKIP);

  return (_("OS: ") + os + "\n" + _("Kernel: ") + kern + "\n" + _("Display Server: ") + workbuffer + "\n" + \
		                                              _("Desktop environment : ") + deskenv + (gccver.size() ? (std::string("\n") + std::string(_("Compiler: ")) + gccver) : ""));
}

void CInitThread::Init(CHWindow *plMw) const
{
    if(PTSMNG(pntProcessor))
    	PTSMNG(m_Label_CPU).set_text(PTSMNG(pntProcessor)->ProcInfoInit());

    PTSMNG(m_Label_MOBO).set_text(plMw->MOBO_Info());
    PTSMNG(m_Label_Memory).set_text(plMw->Mem_Info());

    if(PTSMNG(pMonitor)) {
    	 PTSMNG(pMonitor)->Init(); //Monitor(s)
    	 std::string print = PTSMNG(pMonitor)->Get_Ident();
         if(print != "") 
        	 PTSMNG(m_Label_Monitors).set_text(print);
    }

    PTSMNG(m_Label_Network).set_text(plMw->GetDevices(DeviceType::NETWORKRESOURSE));
    PTSMNG(m_Label_Audio).set_text(plMw->GetDevices(DeviceType::AUDIO));

    if(PTSMNG(pGpus))
    	PTSMNG(pGpus)->Gpus_Info_Init();

    PTSMNG(m_Label_OS).set_text(plMw->OS_Info());

    if(PTSMNG(pSysensors))
    	PTSMNG(pSysensors)->SensorsDetect(&PTSMNG(temperature_monitoring_enabled));

    if(PTSMNG(pUd2Manager))
    	PTSMNG(pUd2Manager)->SensorsDetect();

    if(m_Dispatch && CONNECTIONSTATUS(m_Connection)) m_Dispatch->emit();
}
