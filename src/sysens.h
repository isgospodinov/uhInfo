/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _SYSENS_H_
#define _SYSENS_H_

#include "sensmon.h"
#include "dlg/smdialog.h"
#include "dar/hdwarev.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <list>
#include <dlfcn.h>

using namespace uhiutil::sdef;

using sensors_chip_name_mod = struct _sensors_chip_name_mod {
	std::string cnip_prefix;
	sensors_bus_id cnip_bus;
	int cnip_addr;
	std::string cnip_path;
};

using Sensor_node = struct _Sensor_node : public Sensor_statistic{
      _Sensor_node(int fn,std::string l,sensors_feature_type sft,bool sv,bool vc) : Sensor_statistic(sv,vc),feature_number(fn),label(l),sntype(sft){}
      int feature_number;
      std::string label;
	  sensors_feature_type sntype;
};

using Chip_node = struct _Chip_node {
      long unsigned int inactive_sensors_number = 0;
      sensors_chip_name_mod chip_name;
      std::string chip_id = "";
      std::list<Sensor_node> sensors;
      bool get_node_print_status() const {return (inactive_sensors_number == sensors.size());}
      bool operator < (const _Chip_node cpnd) {return (chip_name.cnip_prefix < cpnd.chip_name.cnip_prefix);}
      void EraseStatistics(){for(std::list<Sensor_node>::iterator sn =  sensors.begin(); sn != sensors.end(); sn++) {sn->EraseStatistic();}}
};

class CSysens : public CSensMon
{
public :
    CSysens();
    virtual ~CSysens() override;

    friend class CSmDialog;
    unsigned int GetSensorNodesNumb() const {return monitoring.size();}
    unsigned int GetSensorsDetectedNumb() const;
    void SensorsDetect(bool *flag);
    virtual void PopulateTemperatureSelection(CHWindow *m_wnd) override;
    void PrintDetectedSensors(Glib::RefPtr<Gtk::TextBuffer> txtbuff,const bool printmode,const bool blink_global_status,const bool advanced,unsigned int tmp_in_sens_count);
    void EraseStatisticsAll(){for(std::list<Chip_node>::iterator chn = monitoring.begin(); chn != monitoring.end(); chn++) {chn->EraseStatistics();}}
    virtual CDrArTempr::DRAWVECTORPLUS SensorStatisticToggle(bool status,Glib::ustring color,Glib::ustring node,Glib::ustring sensor,Glib::ustring nodeid,int sensorid,double **max = nullptr) override;
private:
    // ------------- Skipping libsensors runtime dependency -------------
    sensors::fp_sInit      sysens_init;
    sensors::fp_sClean     sysens_cleanup;
    sensors::fp_sChips     sysens_get_detected_chips;
    sensors::fp_sFeatures  sysens_get_features;
    sensors::fp_sLabel     sysens_get_label;
    sensors::fp_subFeature sysens_get_subfeature;
    sensors::fp_sValue     sysens_get_value;
    // ------------- Skipping libsensors runtime dependency -------------

    void *libsensh = nullptr;
    std::list<Chip_node> monitoring;
    bool LibSensorsOpen(const char * filename);
};

#endif // _SYSENS_H_
