/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _PROCUN_H_
#define _PROCUN_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <list>
#include <gtkmm/progressbar.h>
#include <algorithm>
#include "proc.h"

using unit_calc_el = struct _unit_calc_el {
     int cpunit{0};
     LONG2INT cpuid_mem_total{0};
     LONG2INT cpuid_mem_idle{0}; 
                     
     _unit_calc_el() {init_calc_el();}
     void init_calc_el(){std::for_each(UnitData.begin(), UnitData.end(), [](CDrArCpu::TUDRAWVECTOR &el){el.fill(0.0);});}
     void set_data(double df,double du,double dfc) {
              for(int dc = (uhiutil::calc::draw_cpu_statistic - 1); dc > 0 ;  dc--) {
            	  UnitData[uhiutil::calc::bfr][dc] = UnitData[uhiutil::calc::bfr][dc - 1]; // frequency
            	  UnitData[uhiutil::calc::usg][dc] = UnitData[uhiutil::calc::usg][dc - 1]; // usage
            	  UnitData[uhiutil::calc::cfr][dc] = UnitData[uhiutil::calc::cfr][dc - 1]; // comparative frequency

              } 
              UnitData[uhiutil::calc::bfr][0] = df; // frequency
              UnitData[uhiutil::calc::usg][0] = du; // usage
              UnitData[uhiutil::calc::cfr][0] = dfc; // comparative frequency
     }
     const CDrArCpu::TUDRAWVECTOR *const GetStor(const int sn) const {return &UnitData[sn];}
private:
     // UnitData[0] - base /*cpuinfo*/ freq,UnitData[1] - comparative /*scaling_cur_freq*/ freq,UnitData[2] - cpu usage
     std::array<CDrArCpu::TUDRAWVECTOR, uhiutil::calc::dimension> UnitData;
};

class CProcUnits : public CProc
{
     std::list<unit_calc_el> units_calc_cahin; //unit`s chain
public:
     struct {
     	     CDrArCpu::TUDRAWVECTOR FqAvg,UsgAvg;
     	     void clear_cpufq_average_data(){FqAvg.fill(.0);UsgAvg.fill(.0);}
     	     void set_cpufq_average_data(double data,double udata) {
     	          for(int dc = (uhiutil::calc::draw_cpu_statistic - 1); dc > 0 ;  dc--) {
     	        	    FqAvg[dc] = FqAvg[dc - 1];
     	        	    UsgAvg[dc] = UsgAvg[dc - 1];
     	          }
     	          FqAvg[0] = data;
     	          UsgAvg[0] = udata;
     	     }
     }cpuFqAvg;

     virtual std::string ProcInfoInit() override;
     virtual void CalcFrecqUsage(Gtk::ProgressBar *pbF,Gtk::ProgressBar *pbU,std::list<cpu_chain_el> *units_ch = nullptr,bool bCpuAltCalc = false) override;
     void ClearCPUnitStatistic() {std::for_each(units_calc_cahin.begin(), units_calc_cahin.end(), [](unit_calc_el &cpunel){cpunel.init_calc_el();});}
     const std::list<unit_calc_el>* GetCalcChain() const {return &units_calc_cahin;}
};

#endif // _PROCUN_H_
