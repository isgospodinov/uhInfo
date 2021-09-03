/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "mwnd.h"

std::string CProcUnits::ProcInfoInit()
{
   std::string init = CProc::ProcInfoInit();
   //Meaningfulness verification.One core,one thread.No sense.
   if(cpu_units > 1) {
       unit_calc_el unit_el;
       std::string sclpath("");
       for(unit_el.cpunit = 0 ; (unsigned int) unit_el.cpunit < cpu_units ; unit_el.cpunit++){
           units_calc_cahin.push_back(unit_el);
           if(m_CpuAltCalc) {
               sclpath = "/sys/devices/system/cpu/cpu" + std::to_string(unit_el.cpunit) + "/cpufreq/scaling_cur_freq";
               if(!uhiutil::ExistenceVerification(sclpath.c_str())) m_CpuAltCalc = false;
           }
       }
   }

   // m_CpuAltCalc = false;

   return init;
}

void CProcUnits::CalcFrecqUsage(Gtk::ProgressBar *pbF,Gtk::ProgressBar *pbU,std::list<cpu_chain_el> *units_ch,bool bCpuAltCalc)
{
    bool local_cd(m_ClearCalcData);

    if(((pbF != nullptr) && (pbU != nullptr))) {
             if(local_cd) ClearCPUnitStatistic();
             CProc::CalcFrecqUsage(pbF,pbU);
    }
    else {
             std::istringstream input;
             std::string cline(""),cline_compare("");
             double usgdc = 0.0,freqdc = 0.0,freqdc_compare = 0.0;
             
	         input = std::istringstream(uhiutil::execmd("cat /proc/cpuinfo  | grep \"cpu MHz\""));
             std::list<cpu_chain_el>::iterator elV = units_ch->begin();
             for(std::list<unit_calc_el>::iterator cel = units_calc_cahin.begin(); cel != units_calc_cahin.end(); cel++,elV++) {   
             
                if(local_cd) cel->init_calc_el();

            	 std::getline(input,cline);
            	 freqdc = FreqCalc(cline,CHECKBCF);

            	 if(m_CpuAltCalc) {
                    cline_compare = "cat /sys/devices/system/cpu/cpu" + std::to_string((*cel).cpunit) + "/cpufreq/scaling_cur_freq";
                    cline_compare = "cpu MHz\t: " + std::to_string((std::stod(uhiutil::execmd(cline_compare.data())) / (double) 1000));
                    freqdc_compare = FreqCalc(cline_compare,!CHECKBCF);
                }

            	UIBCF((*elV).cpuid_m_pbF,(CHECKBCF ? freqdc_compare : freqdc),(CHECKBCF ? cline_compare : cline));
            	UIBCF((*elV).cpuid_m_pbCF,(CHECKBCF ? freqdc : freqdc_compare),(CHECKBCF ? cline : cline_compare));

                std::string cd = "cat /proc/stat | grep cpu" + std::to_string((*cel).cpunit);
                std::string res  = uhiutil::execmd(cd.data());
                usgdc = UsageCalc(res,&(*cel).cpuid_mem_total,&(*cel).cpuid_mem_idle);
                UIBCF((*elV).cpuid_m_pbU,usgdc,res);

                (*cel).set_data(freqdc,usgdc,freqdc_compare);
             }
    }
    
    if(local_cd) m_ClearCalcData = false;
}
