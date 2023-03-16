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

   //TestPoint
   // m_CpuAltCalc = false;

   return init;
}

void CProcUnits::CalcFrecqUsage(Gtk::ProgressBar *pbF,Gtk::ProgressBar *pbU,std::list<cpu_chain_el> *units_ch,bool bCpuAltCalc)
{
    bool local_cd(m_ClearCalcData);
    double sum = .0;

    if(((pbF != nullptr) && (pbU != nullptr))) {
             if(local_cd) ClearCPUnitStatistic();
             CProc::CalcFrecqUsage(pbF,pbU);
    }
    else {
             std::string cline(""),cline_compare(""),fq_double("");
             double usgdc = 0.0,freqdc = 0.0,freqdc_compare = 0.0;
             std::istringstream input_ci{std::istringstream(uhiutil::execmd("grep 'cpu MHz' /proc/cpuinfo | awk -F ': ' '{print $2}'"))},
            		 input_sf{(m_CpuAltCalc ? uhiutil::execmd("cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq") : "")};

             std::list<cpu_chain_el>::iterator elV = units_ch->begin();
             for(std::list<unit_calc_el>::iterator cel = units_calc_cahin.begin(); cel != units_calc_cahin.end(); cel++,elV++) {   
             
                if(local_cd) cel->init_calc_el();

            	 std::getline(input_ci,cline);
            	 fq_double = cline;
            	 freqdc = FreqCalc(cline,CHECKBCF);
            	 if(!m_CpuAltCalc)
            		       sum += (uhiutil::cpu::native_fq_state ? freqdc : FreqCalc(fq_double,false,true));

            	 if(m_CpuAltCalc && std::getline(input_sf,cline_compare)) {
            		cline_compare = std::to_string((std::stod(cline_compare) / (double) 1000));
            		fq_double = cline_compare;
                    freqdc_compare = FreqCalc(cline_compare,!CHECKBCF);
                    sum += (uhiutil::cpu::native_fq_state ? freqdc_compare : FreqCalc(fq_double,false,true));
                }

            	UIBCF((*elV).cpuid_m_pbF,(CHECKBCF ? freqdc_compare : freqdc),(CHECKBCF ? cline_compare : cline));
            	UIBCF((*elV).cpuid_m_pbCF,(CHECKBCF ? freqdc : freqdc_compare),(CHECKBCF ? cline : cline_compare));

                std::string cd = "cat /proc/stat | grep cpu" + std::to_string((*cel).cpunit) + " | head -n1";
                std::string res  = uhiutil::execmd(cd.data());
                usgdc = UsageCalc(res,&(*cel).cpuid_mem_total,&(*cel).cpuid_mem_idle);
                UIBCF((*elV).cpuid_m_pbU,usgdc,res);

                (*cel).set_data(freqdc,usgdc,freqdc_compare);
             }

             std::string res = uhiutil::execmd("head -n1 /proc/stat");
             cpuFqAvg.set_cpufq_average_data(sum / (double) cpu_units,CProc::UsageCalc(res));
    }
    
    if(local_cd) m_ClearCalcData = false;
}
