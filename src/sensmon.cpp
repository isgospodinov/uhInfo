/*
 *    uhInfo
 *    Copyright (C) 2019
 */

#include "sensmon.h"

CSensMon::~CSensMon() {}

const std::string CSensMon::Print_Value(const float value) const
{
    float trnc = std::trunc(value);
    char bf[6] = {0,0,0,0,0,0};

    if(value < (trnc + 0.1))
         std::sprintf(bf,"%2.0f",trnc);
    else
        std::sprintf(bf,"%2.1f",value);

    return std::string(bf);
}

bool CSensMon::SetVisiblity(std::string clabel,std::string nodeid) const
{
   std::string cmd = "cat /home/" + uhiutil::GetUserName() + "/.uhInfo/uhInfo.cfg | grep " + "'" + clabel + ":" + nodeid + "'";
   cmd = uhiutil::execmd(cmd.c_str()); 
   if(cmd != "") return false;

   return true;
}

