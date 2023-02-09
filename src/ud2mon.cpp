/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "ud2mon.h"
#include "mwnd.h"
#include <cstring>

void Ud2mon::CallbackJob(GDBusObjectManager *objManager, GDBusObject *pObject,CallbackMode mode)
{
   const gchar *obj_path = g_dbus_object_get_object_path(pObject),*linkname = nullptr;
   if(!g_str_has_prefix(obj_path, sensors::idd_path)) 
                                       return;
   GDBusObject *pobj = nullptr;
   UDisksPartitionTable *partition = nullptr;
   UDisksBlock *blok_dev = nullptr;
   UDisksDrive *drive = nullptr;
   UDisksDriveAta *ata_drive = nullptr;
   bool dt[] = {false, false, false}; //dt[0]- optical, dt[1] - usb, dt[2] - dev_present
   unsigned int dcnt = 1;

   g_object_get(G_DBUS_OBJECT(pObject), "block", &blok_dev, "partition-table", &partition, nullptr);
   if(blok_dev)
        pobj = g_dbus_object_manager_get_object(objManager,udisks_block_get_drive(blok_dev));
   if(pobj) {
        g_object_get(pobj, "drive", &drive, "drive-ata", &ata_drive, nullptr);
        if(!drive) {
             if(ata_drive) g_clear_object(&ata_drive);
             if(partition) g_clear_object(&partition);
             g_object_unref(pobj);
             g_clear_object(&blok_dev);
             return;
        }

        media_detect(drive,dt);

        linkname = udisks_block_get_device(blok_dev);
        if(partition || (mode == CallbackMode::REMOVE)) {
             Gtk::TreeModel::Children children = m_wnd->m_refTreeModel->children();
             for(Gtk::TreeModel::Children::iterator iter = children.begin();iter != children.end(); dcnt++,iter++)
             {
                  if((*iter)[m_wnd->dColumns->sys_link] == linkname) {
                            dt[2]/*dev_present*/ = true;
                            if(mode == CallbackMode::ADD) break;
                            else { // CallbackMode::REMOVE
                                   iter = m_wnd->m_refTreeModel->erase(iter);
                                   if(iter == children.end()) break;
                            }
                  }

                  if(mode == CallbackMode::REMOVE && dt[2]/*dev_present*/ && iter != children.end()) (*iter)[m_wnd->dColumns->No] = dcnt;
             }

             if(mode == CallbackMode::ADD && !dt[2]/*dev_present*/) {
                    Gtk::TreeRow row = *(m_wnd->m_refTreeModel->append());
                    row[m_wnd->dColumns->No] = dcnt;
                    row[m_wnd->dColumns->sys_link] = linkname;
                    row[m_wnd->dColumns->device_name] = udisks_drive_get_model(drive);
                    row[m_wnd->dColumns->type] = ((dt[0]/*optical*/ ? "CD/DVD" :  (dt[1]/*usb*/ ? "Drive" : (partition ? "Disk" : "Unknown"))));
                    row[m_wnd->dColumns->specificity] = ((dt[0]/*optical*/ ? "coldplug removable" :  (dt[1]/*usb*/ ? "hotplug removable" : (partition ? "coldplug fixed" : "Unknown"))));
             }

             //if(CallbackMode::ADD && ata_drive && udisks_drive_ata_get_smart_supported(ata_drive) && udisks_drive_ata_get_smart_enabled(ata_drive)) {
             //TODO:Add detected device to monitoring chain appropriate way
             //}
        }
        if(ata_drive) g_clear_object(&ata_drive);
        g_clear_object(&drive);
        g_object_unref(pobj);
   }

   if(blok_dev) g_clear_object(&blok_dev);
   if(partition) g_clear_object(&partition);
}

unsigned int Ud2mon::GetSensorsDetectedNumb() const
{
    if(GetSensorNodesNumb())
              return monitoring.size();

    return 0;
}

void Ud2mon::SensorsDetect()
{
      Gtk::TreeRow row;
      unsigned int cnounter = 1;
      bool dt[] = {false,false}; //dt[0]- optical, dt[1] - usb
      GDBusObjectManager *Ud2Manager = nullptr;
      UDisksPartitionTable *partition = nullptr;
      UDisksBlock *blok_dev = nullptr;
      UDisksDrive *drive = nullptr;
      UDisksDriveAta *ata_drive = nullptr;
      GList *objects = nullptr, *obj_list= nullptr;
      const gchar *obj_path = nullptr,*ud2drvmodel = nullptr;
      GDBusObject *pobj = nullptr;

      if(Ud2Cl) {
          Ud2Manager = udisks_client_get_object_manager(Ud2Cl.get());
          if(Ud2Manager)
               objects = g_dbus_object_manager_get_objects(Ud2Manager);
      }
      for(obj_list = objects; obj_list != nullptr; obj_list = obj_list->next) {
          obj_path  = g_dbus_object_get_object_path(G_DBUS_OBJECT(obj_list->data));
          if(!g_str_has_prefix(obj_path, sensors::idd_path)) {
                  g_object_unref(obj_list->data);
                  continue;
          }
          g_object_get(G_DBUS_OBJECT(obj_list->data), "block", &blok_dev, "partition-table", &partition, nullptr);
          if(blok_dev)
                pobj = g_dbus_object_manager_get_object(Ud2Manager,udisks_block_get_drive(blok_dev));
          if(pobj) {
                g_object_get(pobj, "drive", &drive, "drive-ata", &ata_drive, nullptr);
                if(!drive) {
                    if(ata_drive) g_clear_object(&ata_drive);
                    if(partition) g_clear_object(&partition);
                    g_object_unref(pobj);
                    g_clear_object(&blok_dev);
                    continue;
                }
                
        	    media_detect(drive,dt);
                
                if(partition || dt[0]/*optical*/) {
                    row = *(m_wnd->m_refTreeModel->append());
                    row[m_wnd->dColumns->No] = cnounter++;
                    row[m_wnd->dColumns->sys_link] = udisks_block_get_device(blok_dev);
                    row[m_wnd->dColumns->device_name] = (ud2drvmodel = udisks_drive_get_model(drive));
                    row[m_wnd->dColumns->type] = ((dt[0]/*optical*/ ? "CD/DVD" :  (dt[1]/*usb*/ ? "Drive" : (partition ? "Disk" : "Unknown"))));
                    row[m_wnd->dColumns->specificity] = ((dt[0]/*optical*/ ? "coldplug removable" :  (dt[1]/*usb*/ ? "hotplug removable" : (partition ? "coldplug fixed" : "Unknown"))));
                    if(ata_drive && udisks_drive_ata_get_smart_supported(ata_drive) && udisks_drive_ata_get_smart_enabled(ata_drive)) {
                          const char *drid = udisks_drive_get_serial(drive);
                          bool visnode = SetVisiblity(std::string(ud2drvmodel),std::string(drid));
                          monitoring.push_back(std::move(Ud2_sens_node(ata_drive,ud2drvmodel,drid,visnode)));
                          if(!visnode) ++inactive_dev_number;
                    }
                    if(ud2drvmodel) ud2drvmodel = nullptr;
                }
                g_clear_object(&drive);
                g_object_unref(pobj);
                dt[0] = dt[1] = false;
          }
          if(obj_path) obj_path = nullptr;
          if(blok_dev) g_clear_object(&blok_dev);
          if(partition) g_clear_object(&partition);
          g_object_unref(obj_list->data);
      }
      g_list_free(objects);

      if(Ud2Manager) {
          g_signal_connect(Ud2Manager, "object-added", G_CALLBACK(Ud2mon::addobj_callback), this);
          g_signal_connect(Ud2Manager, "object-removed", G_CALLBACK(Ud2mon::rmvobj_callback), this);
      }

}

std::string Ud2mon::PrintDetectedSensors(Glib::RefPtr<Gtk::TextBuffer> txtbuff,const bool printmode,const bool blink_global_status)
{
   static bool blink = true;

   if(!blink && !printmode) blink = true;   

   int KelvTemp = 0.0;
   double ctemp = 0.0;
   std::string bufer("");
   Gtk::TextBuffer::iterator itxbf = txtbuff->get_iter_at_line(txtbuff->get_line_count());
   if(GetSensorNodesNumb() && !get_ud2_print_status()) {
	   bufer.append("  " + std::string{sensors::nud2} + '\n');
        if(printmode)
        	itxbf = txtbuff->insert(itxbf,"  " + std::string{sensors::nud2} + '\n');
        for(std::list<Ud2_sens_node>::iterator it =  monitoring.begin(); it != monitoring.end(); it++) {
              if(!it->visible) continue;
              if(Ud2Cl && it->ata_drive) {
                    KelvTemp = udisks_drive_ata_get_smart_temperature(*it->ata_drive) - 273.15;
                    if(KelvTemp > 0) {
                          if(it->t_statistic_active) {
                               ctemp = (KelvTemp / (double) uhiutil::cpu::max_cpu_t);
                               if(ctemp > 1.0) ctemp = 1.0;
                               if(ctemp < 0.0) ctemp = 0.0;
                               if(it->max < KelvTemp) it->max = KelvTemp;
                               it->SetTemperature(ctemp);
                               if(printmode) {
                                     if(!blink_global_status || blink) {itxbf = txtbuff->insert(itxbf," ");itxbf = txtbuff->insert_with_tag(itxbf,"      ",it->statistic_color);itxbf = txtbuff->insert(itxbf," ");}
                                     else 
                                         itxbf = txtbuff->insert(itxbf,"        ");
                                     
                                     itxbf = txtbuff->insert_with_tag(itxbf,std::string(it->ud2_model_name),uhiutil::ui::active_tag);                                     
                                     itxbf = txtbuff->insert(itxbf," : " + std::to_string(KelvTemp) + "°C");                                                                                                               
                                     itxbf = txtbuff->insert_with_tag(itxbf,("   max: " + Print_Value((float)it->max) + "°C\n"),uhiutil::ui::max_tag);                                     
                               }
                          }
                          else {
                               if(it->max != 0.0) it->max = 0.0;
                               bufer.append("        " + std::string(it->ud2_model_name) + " : " + std::to_string(KelvTemp) + "°C\n");
                               if(printmode)
                                     itxbf = txtbuff->insert(itxbf,("        " + std::string(it->ud2_model_name) + " : " + std::to_string(KelvTemp) + "°C\n"));
                          }
                    }
              }
         }
   }

   if(printmode) blink = !blink;

   return bufer;
}

void Ud2mon::PopulateTemperatureSelection(CHWindow *m_wnd)
{
   Glib::RefPtr<Gtk::ListStore> pTM = m_wnd->ptRefTreeModel;
   int index = SIZEOF(clrID) - 1;
   visible_tmp_sens_count = 0;
   for(Ud2_sens_node sn : monitoring) {
           if(sn.visible) {
                  Gtk::TreeModel::Row row = *(pTM->append());
                  row[m_wnd->tColumns->col_tcheck] = sn.t_statistic_active;
                  row[m_wnd->tColumns->tsensor_node] = sensors::nud2;
                  row[m_wnd->tColumns->tsensor_name] = sn.ud2_model_name;
                  row[m_wnd->tColumns->tnode_id] = sn.ud2_drv_id;
                  row[m_wnd->tColumns->tsensor_id] = sn.index;
                  row[m_wnd->tColumns->color_name] = clrID[index--];
                  if(index == 0) index = SIZEOF(clrID) - 1;
                  visible_tmp_sens_count ++;
           }
   }
}

CDrArTempr::DRAWVECTORPLUS Ud2mon::SensorStatisticToggle(bool status,Glib::ustring color,Glib::ustring node,Glib::ustring sensor,Glib::ustring nodeid,int sensorid,double **max)
{
   for(std::list<Ud2_sens_node>::iterator sn =  monitoring.begin(); sn != monitoring.end(); sn++) {
          if(Glib::ustring(sn->ud2_model_name) == sensor && Glib::ustring(sn->ud2_drv_id) == nodeid) {
        	    if(max) {
                      sn->SetStatisticFeatures(status,color);
                      *max = &sn->max;
        	    }
                return {&sn->t_statistic,&sn->statistic_color};
          }
    }

   return {nullptr,nullptr};
}

inline void Ud2mon::media_detect(UDisksDrive *dr,bool m2[]) const
{
	//m2[0]- optical, m2[1] - usb
    const gchar *const *mcd = udisks_drive_get_media_compatibility(dr);
    if(mcd) {
        int cp = 0;
        while(mcd[cp] && std::strlen(mcd[cp])) {
             if(g_strstr_len(mcd[cp], -1, "optical")) {
	             m2[0] = true;
	             break;
             }
             ++cp;
        }
    }

    if(g_strrstr(udisks_drive_get_connection_bus(dr),"usb")) m2[1] = true;
}
