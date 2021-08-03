/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _HMONIT_H_
#define _HMONIT_H_

#include <string>

class CMonitor
{
public:
    void Init();
    std::string Get_Ident() const {return m_monitor;}

private:
    std::string m_monitor{""};

    bool get_edid_buffer(std::istringstream &ininput,std::string &mntr) const;
    char convert(const char *chek) const;
    std::string calculate(float Vsize,float Hsize) const;
    std::string calculate_dimension(const char *pchar,bool xrandr_edid = true) const;
    std::string GetMonitorName(const char *pName,bool xrandr_edid = true) const;
};

#endif // _HMONIT_H_
