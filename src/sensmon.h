/*
 *    uhInfo
 *    Copyright (C) 2019
 */

#ifndef _SENSMON_H_
#define _SENSMON_H_

#include "uhirc.h"

class CHWindow;

using Sensor_statistic = struct _Sensor_statistic {
      _Sensor_statistic(bool sv) : visible(sv){}
      std::vector<double> t_statistic;
      std::string statistic_color = "";
      bool t_statistic_active = false;
      double max = 0.0;
      bool visible;
      void SetTemperature(double temperature){t_statistic.insert(t_statistic.begin(),temperature);if(t_statistic.size() > uhiutil::calc::t_statistic_len) t_statistic.erase((t_statistic.end() - 1));}
      void SetStatisticFeatures(bool StatStatus,std::string color){t_statistic_active = StatStatus;statistic_color = (StatStatus ? color : "");if(!t_statistic_active) t_statistic.clear();}
      void EraseStatistic(){SetStatisticFeatures(false,"");}
};

class CSensMon
{
public:
   virtual CDrawArea::DRAWVECTORPLUS SensorStatisticToggle(bool status,Glib::ustring color,Glib::ustring node,Glib::ustring sensor,Glib::ustring nodeid,int sensorid,double **max = nullptr) = 0;
   unsigned int visible_tmp_sens_count = 0;
protected:
   virtual void PopulateTemperatureSelection(CHWindow *m_wnd) = 0;
   const std::string Print_Value(const float value) const;
   bool SetVisiblity(std::string clabel,std::string nodeid) const;
   virtual ~CSensMon() = 0;
};

#endif // _SENSMON_H_
