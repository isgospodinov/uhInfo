/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _PROC_H_
#define _PROC_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <list>
#include <gtkmm/progressbar.h>

using LONG2INT = long long int;
using cpu_chain_el = struct _cpu_chain;

class CProc
{
public :
    virtual std::string ProcInfoInit();
    virtual void CalcFrecqUsage(Gtk::ProgressBar *pbF,Gtk::ProgressBar *pbU,std::list<cpu_chain_el> *units_ch = nullptr,bool bCpuAltCalc = false);
    unsigned int Get_cpu_units() const {return cpu_units;}
    const double *const Get_PtrCpu_fqmax() {return &cpu_max_mhz;}
    double FreqCalc(std::string &Fq, bool bc = false, bool fast = false);

    static bool m_CpuAltCalc,m_lsCpu;
    bool m_ClearCalcData = false;
    std::string cpuname = "";

    virtual ~CProc() = default;
protected :
    double cpu_max_mhz = 0.0, cpu_min_mhz = 0.0;
    unsigned int cpu_units = 0;
    //cpu summary
    LONG2INT mem_total = 0,mem_idle = 0;

    std::string Cpu_microcodes();
public :
    double UsageCalc(std::string &Usge,LONG2INT *mem_tot = nullptr,LONG2INT *mem_id = nullptr);
};

#endif // _PROC_H_
