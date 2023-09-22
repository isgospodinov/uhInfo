/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _UD2MON_H_
#define _UD2MON_H_

#include "dlg/smdialog.h"
#include <udisks/udisks.h>
#include <list>
#include "sensmon.h"

#if UDISKS2_CHECK_VERSION(2,10,0)

struct uhiUDisksDriveAta {
	uhiUDisksDriveAta(bool f) : is_ata(f) {}
	uhiUDisksDriveAta(UDisksDriveAta *const&a) : atadrv(a) {}
	virtual ~uhiUDisksDriveAta(){if(atadrv) g_clear_object(const_cast<UDisksDriveAta **>(&atadrv));}

	UDisksDriveAta *const atadrv = nullptr;
	bool is_ata = true;
};

struct uhiUDisksDriveNvme : public uhiUDisksDriveAta {
	uhiUDisksDriveNvme(UDisksNVMeController *const&n, bool f) : uhiUDisksDriveAta(f),nvmectrl(n) {}
	virtual ~uhiUDisksDriveNvme(){if(nvmectrl) g_clear_object(const_cast<UDisksNVMeController**>(&nvmectrl));}

	UDisksNVMeController *const nvmectrl = nullptr;
};

using Ud2_sens_node = struct _Ud2_sens_node : public Sensor_statistic{
	_Ud2_sens_node(std::shared_ptr<uhiUDisksDriveAta> drive,const char *mn,const char *dnm,bool vz) : Sensor_statistic(vz,false), ud2_model_name(mn), ud2_drv_id(dnm),
	    		  uhiDrive(drive) {}

      int index = -1;
      const char *ud2_model_name,*ud2_drv_id;
      const std::shared_ptr<const uhiUDisksDriveAta> uhiDrive; // generalized device
};

#else

using Ud2_sens_node = struct _Ud2_sens_node : public Sensor_statistic{
      _Ud2_sens_node(UDisksDriveAta *const atadrv,const char *mn,const char *dnm,bool vz) : Sensor_statistic(vz,false), ud2_model_name(mn), ud2_drv_id(dnm),
                ata_drive(new (UDisksDriveAta *const){atadrv},[](UDisksDriveAta *const *drv){if(drv){g_clear_object(const_cast<UDisksDriveAta **>(drv));delete drv;}}){}
      int index = -1;
      const char *ud2_model_name,*ud2_drv_id;
      std::shared_ptr<UDisksDriveAta *const> ata_drive;
};

#endif

class Ud2mon : public CSensMon
{
     using ud2mon_unique_ptr = std::unique_ptr<UDisksClient, std::function<void(UDisksClient*)>>;
     using CallbackMode = enum class CallbackMsgMode {ADD,REMOVE};

     static void addobj_callback(GDBusObjectManager *objManager, GDBusObject *pObject, gpointer cbData){((Ud2mon*)cbData)->CallbackJob(objManager,pObject,CallbackMode::ADD);}
     static void rmvobj_callback(GDBusObjectManager *objManager, GDBusObject *pObject, gpointer cbData){((Ud2mon*)cbData)->CallbackJob(objManager,pObject,CallbackMode::REMOVE);}
     void CallbackJob(GDBusObjectManager *objManager, GDBusObject *pObject, Ud2mon::CallbackMode mode);
public :
     friend class CSmDialog;
     unsigned int GetSensorsDetectedNumb() const;
     unsigned int GetSensorNodesNumb() const {return (Ud2Cl ? (monitoring.size() ? 1 : 0 ) : 0);}
     void SensorsDetect();
     std::string PrintDetectedSensors(Glib::RefPtr<Gtk::TextBuffer> txtbuff,const bool printmode,const bool blink_global_status);
     const bool GetPrintForceStatus() const {return dataPrint_forced;}
     void PrintForceExternal(bool pforce = true) {dataPrint_forced = pforce;}
     virtual void PopulateTemperatureSelection(CHWindow *m_wnd) override;
     virtual CDrArTempr::DRAWVECTORPLUS SensorStatisticToggle(bool status,Glib::ustring color,Glib::ustring node,Glib::ustring sensor,Glib::ustring nodeid,int sensorid,double **max = nullptr) override;
     void EraseStatisticsAll(){for(std::list<Ud2_sens_node>::iterator chn = monitoring.begin(); chn != monitoring.end(); chn++) {chn->EraseStatistic();}}

     virtual ~Ud2mon() override = default;

     Ud2mon(CHWindow *p_wnd) : m_wnd(p_wnd){}
     CHWindow *m_wnd = nullptr;
private:
    const ud2mon_unique_ptr Ud2Cl = ud2mon_unique_ptr(UDISKS_CLIENT(g_initable_new(UDISKS_TYPE_CLIENT,nullptr,nullptr,nullptr)), [](UDisksClient *ud2c){if(ud2c) g_object_unref(ud2c);});
    std::list<Ud2_sens_node> monitoring;
    long unsigned int inactive_dev_number = 0;
    bool get_ud2_print_status() const {return (inactive_dev_number == monitoring.size());}
    void media_detect(UDisksDrive *dr,bool m2[]) const;
    bool dataPrint_forced = false;
};

#endif // _UD2MON_H_
