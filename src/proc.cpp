/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "proc.h"
#include "uhirc.h"

bool CProc::m_CpuAltCalc = true;
//TestPoint
bool CProc::m_lsCpu = !uhiutil::execmd("lscpu").empty();

std::string CProc::Cpu_microcodes()
{
   std::string cpumcd = uhiutil::execmd("cat /proc/cpuinfo | grep microcode"),mcodes("Unknown");                             

   if(cpumcd.length() > 0) {
       std::string mcod("");
       std::istringstream input(cpumcd);
       for(;std::getline(input, mcod);) {
           uhiutil::newline(mcod,":",Direction::RIGHT);
           if(((int)mcodes.find(mcod)) == -1) {
                if(mcodes == "Unknown") mcodes = "";
                mcodes.append((((mcodes.length() > 0) ? "," : "") + mcod));
           }
       }
   }
   
   return mcodes;
}

std::string CProc::ProcInfoInit()
{
	std::string cores("0"),threads("0"),min("0"),max("0");
	cpuname = uhiutil::execmd("cat /proc/cpuinfo | grep 'model name' | head -n1 | awk -F ': ' '{print $2}'");
	if(cpuname.rfind('\x0A') != std::string::npos) cpuname.pop_back();

	if(m_lsCpu) {
         cores = uhiutil::execmd("lscpu | grep 'Core(s) per socket:'");
         uhiutil::newline(cores,"Core(s) per socket:",Direction::RIGHT);
         uhiutil::end_intervals_remove((cores = uhiutil::start_intervals_remove(cores)));

         threads = uhiutil::execmd("lscpu | grep 'Thread(s) per core:'");
         uhiutil::newline(threads,"Thread(s) per core:",Direction::RIGHT);
         uhiutil::end_intervals_remove((threads = uhiutil::start_intervals_remove(threads)));
         cpu_units = sysconf(_SC_NPROCESSORS_ONLN);
         threads = std::to_string(cpu_units);

         min = uhiutil::execmd("lscpu | grep 'CPU min'");
         uhiutil::newline(min,":",Direction::RIGHT);
         uhiutil::end_intervals_remove((min = uhiutil::start_intervals_remove(min)));
         cpu_min_mhz = std::stod(min);

         max = uhiutil::execmd("lscpu | grep 'CPU max'");
         uhiutil::newline(max,":",Direction::RIGHT);
         uhiutil::end_intervals_remove((max = uhiutil::start_intervals_remove(max)));
         cpu_max_mhz = std::stod(max);
	}

   std::string cpu_mcodes = Cpu_microcodes();

   return  ("CPU name: " + cpuname + "\n" + "Microcode(s) : " + cpu_mcodes + "\n" + "CPU cores/threads: " + cores + " / " + threads + "\n" + "CPU min/max: " + \
                                               std::to_string(std::atoi(min.c_str())) + "MHz" + " / " + std::to_string(std::atoi(max.c_str())) + "MHz");
}

double CProc::FreqCalc(std::string &Fq, bool bc, bool fast)
{
      if(Fq.rfind('\x0A') != std::string::npos) Fq.pop_back();

      double dres = std::stod(Fq);

      if(dres > cpu_max_mhz) dres = cpu_max_mhz;
      else
         if(dres < cpu_min_mhz) dres = cpu_min_mhz;

      if(!fast) Fq = (bc ? "Compr.Freq. : " : "Freq. : " ) + std::to_string(std::atoi(Fq.data())) + " MHz";
         
      double val_max = (cpu_max_mhz / (double) 1000) - (((fast || uhiutil::cpu::native_fq_state) ?  0.0 : cpu_min_mhz) / (double) 1000);
      double val_min = ((((fast || uhiutil::cpu::native_fq_state) ?  0.0 : cpu_min_mhz) / (double) 1000));

      return (((dres / (double) 1000) - val_min) / val_max);
}

double CProc::UsageCalc(std::string &Usge,LONG2INT *mem_tot,LONG2INT *mem_id)
{
      LONG2INT sum_total = 0,idle_delta = 0,total_delta = 0,usage_delta = 0;
      LONG2INT idle = 0;
      unsigned int conter = 0;
      double usg = 0.0;

      if(Usge.rfind('\x0A') != std::string::npos) Usge.pop_back();

      std::istringstream input(Usge);
      for (std::string line; std::getline(input, line,' '); ) {
             if(std::isdigit(line[0])) {
                  if(conter == 3) idle = std::stoi(line);
                  sum_total += std::stoi(line);
                  conter++;
             }
      }

      idle_delta = idle -  ((mem_id == nullptr) ? mem_idle : *mem_id );
      total_delta = sum_total - ((mem_tot == nullptr) ? mem_total : *mem_tot);
      usage_delta = ((100 * (total_delta - idle_delta)) / total_delta);  
      Usge = ("Usage : " + std::to_string((int) usage_delta) + "%");
      usg = (double) usage_delta / (double) 100;

      if(usg > 1.0) usg = 1.0;
      else
          if(usg < 0.0) usg = 0.0;

      if(mem_tot) *mem_tot = sum_total;
      else mem_total = sum_total;
      
      if(mem_id) *mem_id = idle;
      else mem_idle = idle;

      return usg;
}

void CProc::CalcFrecqUsage(Gtk::ProgressBar *pbF,Gtk::ProgressBar *pbU,std::list<cpu_chain_el> *units_ch,bool bCpuAltCalc)
{
      std::string res("");
      double cpucfq = 0.0;

      std::istringstream instrm{std::istringstream(LSCPUSE ?
    		       uhiutil::execmd(std::string("lscpu -e=mhz | head -n" + std::to_string(cpu_units + 1) + " | tail -" + std::to_string(cpu_units)).c_str()) :
    		                                                                                   uhiutil::execmd("grep 'cpu MHz' /proc/cpuinfo | awk -F ': ' '{print $2}'"))};
      while(std::getline(instrm, res)) {
    	  cpucfq += std::stod(res);
      }
      res = std::to_string(cpucfq / cpu_units);
      UIBCF(pbF,FreqCalc(res),res);

      res  = uhiutil::execmd("head -n1 /proc/stat");
      UIBCF(pbU,UsageCalc(res),res);
}
