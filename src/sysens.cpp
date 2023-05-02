/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "sysens.h"
#include "mwnd.h"

CSysens::CSysens (CDrArVcore::VCORESBUNCH*const dvc) : ldvc(dvc)
{
   bool libopen = false;
   for(unsigned int l = 0; !libopen && l < SIZEOF(Libs); libopen = LibSensorsOpen(Libs[l]),l++);

   if(libopen && (sysens_init)(nullptr)) {LIBCLOSE(libsensh)}
}

CSysens::~CSysens()
{
    if(libsensh) {
          (sysens_cleanup)();
          LIBCLOSE(libsensh)
    }
}

unsigned int CSysens::GetSensorsDetectedNumb() const
{
    unsigned int nsensors = 0;

    if(GetSensorNodesNumb()) {
        for(Chip_node n : monitoring) {
             nsensors += n.sensors.size();
        }
    }

    return nsensors;
}

bool CSysens::LibSensorsOpen(const char * filename)
{
    if(!(libsensh = dlopen(filename, RTLD_LAZY))) return false;
    else {
        if(!(sysens_init = (sensors::fp_sInit) dlsym(libsensh, "sensors_init"))) {RETURN_FALSE;}

        if(!(sysens_cleanup = (sensors::fp_sClean) dlsym(libsensh, "sensors_cleanup"))) {RETURN_FALSE;}

        if(!(sysens_get_detected_chips = (sensors::fp_sChips) dlsym(libsensh, "sensors_get_detected_chips"))) {RETURN_FALSE;}

        if(!(sysens_get_features = (sensors::fp_sFeatures) dlsym(libsensh, "sensors_get_features"))) {RETURN_FALSE;}

        if(!(sysens_get_label = (sensors::fp_sLabel) dlsym(libsensh, "sensors_get_label"))) {RETURN_FALSE;}

        if(!(sysens_get_subfeature = (sensors::fp_subFeature) dlsym(libsensh, "sensors_get_subfeature"))) {RETURN_FALSE;}

        if(!(sysens_get_value = (sensors::fp_sValue) dlsym(libsensh, "sensors_get_value"))) {RETURN_FALSE;}
          
        return true;
    }

    return false;
}

void CSysens::SensorsDetect(bool *flag)
{

  if(libsensh) {
        sensors_chip_name const *chpname = nullptr;
        int chip_count = 0;
        while((chpname = (sysens_get_detected_chips)(nullptr, &chip_count)) != 0) {
             // -----------------check "sensors_get_detected_chips" for returned duplicated values if any !? -----------------
             bool continue_break = false;
             if(!monitoring.empty()) {
                 for(Chip_node n : monitoring) {
                       if(n.chip_name.cnip_prefix.find(std::string(chpname->prefix)) != std::string::npos &&
                    		                        n.chip_name.cnip_bus.type == chpname->bus.type && n.chip_name.cnip_bus.nr == chpname->bus.nr && n.chip_name.cnip_addr == chpname->addr) {
                            if(!continue_break) continue_break = true;
                            break;
                       }
                 }
                 if(continue_break)
                             continue;
             }
             // -----------------check "sensors_get_detected_chips" for returned duplicated values if any !? -----------------

             sensors_feature const *feature = nullptr;
             int feature_count = 0;

             bool pass_true = false;
             Chip_node chnd;
             chnd.chip_id = std::string(chpname->prefix) + std::to_string(chpname->bus.type) + std::to_string(chpname->bus.nr) + std::to_string(chpname->addr);

             while((feature = (sysens_get_features)(chpname, &feature_count)) != 0) {
                 std::string lbl((sysens_get_label)(chpname, feature));

                 sensors_subfeature_type subfeat_type = SENSORS_SUBFEATURE_UNKNOWN;

                 switch(feature->type) {
                        case SENSORS_FEATURE_IN:
                                subfeat_type = SENSORS_SUBFEATURE_IN_INPUT;
                                break;
                        case SENSORS_FEATURE_FAN: 
                                subfeat_type = SENSORS_SUBFEATURE_FAN_INPUT;
                                break;
                        case SENSORS_FEATURE_TEMP: 
                               subfeat_type = SENSORS_SUBFEATURE_TEMP_INPUT;
                               if(!*flag) *flag = true;
                               break;
                        case SENSORS_FEATURE_POWER: 
                                subfeat_type = SENSORS_SUBFEATURE_POWER_INPUT;
                                break;
                        case SENSORS_FEATURE_CURR:
                                subfeat_type = SENSORS_SUBFEATURE_CURR_INPUT;
                                break;
                        default: continue;
                 } // switch feature type
                 
                 sensors_subfeature const *subfeature = (sysens_get_subfeature)(chpname, feature, subfeat_type);
                 if(subfeature) {
                        double value = 0.0;
                        int rc = (sysens_get_value)(chpname, subfeature->number, &value);
                        if(!rc) {
                            if(value <= 0.0) continue;
                            if(!pass_true) pass_true = true;
                            bool visnode = SetVisiblity(lbl,chnd.chip_id);
                            chnd.sensors.push_back(Sensor_node(subfeature->number,lbl,feature->type,visnode,VCORECHECK));
                            if(!visnode) ++chnd.inactive_sensors_number;

                            if(chnd.sensors.back().is_Vcore) {
                            	   //TestPoint
                            	   ldvc->push_back(&chnd.sensors.back());
                            }


                        }
                 }

             } //while feature
            
             if(pass_true) {
                 chnd.chip_name.cnip_prefix = std::string(chpname->prefix);
                 chnd.chip_name.cnip_path = std::string(chpname->path);
                 chnd.chip_name.cnip_bus = chpname->bus;
                 chnd.chip_name.cnip_addr = chpname->addr;
                 monitoring.push_back(std::move(chnd));
             }
             pass_true = false;

        }//while chip
        if(monitoring.size() > 1) monitoring.sort();
  } //LibSensorsOpen 
}

void CSysens::PrintDetectedSensors(Glib::RefPtr<Gtk::TextBuffer> txtbuff,const bool printmode,const bool blink_global_status,const bool advanced,unsigned int tmp_in_sens_count)
{
   static bool blink = true;

   if(!blink && !printmode) blink = true;

   double value = 0.0,ctemp = 0.0;
   sensors_chip_name cn;
   bool chipisset = false;
   Gtk::TextBuffer::iterator itxbf = txtbuff->get_iter_at_line(txtbuff->get_line_count());
   if(libsensh) {
       if(GetSensorNodesNumb()) {
             for(std::list<Chip_node>::iterator n = monitoring.begin(); n != monitoring.end(); n++) {//chip
                    if(n->get_node_print_status()) continue;
                    cn.prefix = (char*) n->chip_name.cnip_prefix.data();
	                cn.bus = n->chip_name.cnip_bus;
	                cn.addr = n->chip_name.cnip_addr;
	                cn.path = (char*) n->chip_name.cnip_path.data();
                    for(std::list<Sensor_node>::iterator sn =  n->sensors.begin(); sn != n->sensors.end(); sn++) {//sensors
                           if(!sn->visible || (printmode && !(SNTP(SENSORS_FEATURE_TEMP)) && !sn->is_Vcore) || (printmode && tmp_in_sens_count == 0) ||
                        		                                                              (!printmode && !advanced && SNTP(SENSORS_FEATURE_IN) && !sn->is_Vcore)) {
                        	   if(sn->is_Vcore) sn->max = .0;
                        	   continue;
                           }
                           (sysens_get_value)(&cn, sn->feature_number, &value);
                           if(!chipisset) {
                        	        itxbf = txtbuff->insert_with_tag(itxbf,"  " + n->chip_name.cnip_prefix + "\n",uhiutil::ui::max_tag);
                                    chipisset = true;
                           }

                           if(sn->t_statistic_active && (!blink_global_status || blink)) {itxbf = txtbuff->insert(itxbf," ");itxbf = txtbuff->insert_with_tag(itxbf,"      ",sn->statistic_color);itxbf = txtbuff->insert(itxbf," ");}                           
                           else itxbf = txtbuff->insert(itxbf,"        ");
                           
                           if(sn->t_statistic_active || (printmode && SNTP(SENSORS_FEATURE_IN) && sn->is_Vcore)) {
                        	   itxbf = txtbuff->insert_with_tag(itxbf,sn->label,
                        		                                   sn->t_statistic_active ? uhiutil::ui::active_tag : uhiutil::ui::max_tag);
                           }
                           else itxbf = txtbuff->insert(itxbf,sn->label);
                           
                           itxbf = txtbuff->insert(itxbf," : ");
                           
                           switch(sn->sntype) {
                                 case SENSORS_FEATURE_IN:
                                	 if(sn->t_statistic_active || (printmode && sn->is_Vcore)){
                                		 itxbf = txtbuff->insert_with_tag(itxbf,((std::to_string(value)).substr(0,5) + "V\n"),uhiutil::ui::active_tag);
                                		 if(sn->visible && sn->is_Vcore) {
                                			 sn->max = value;
                                			 if(sn->t_statistic_active) sn->SetTemperature(value/ uhiutil::calc::draw_Vcore_scaler);
                                		 }
                                		 else sn->max = .0;
                                	 }
                                	 else itxbf = txtbuff->insert(itxbf,((std::to_string(value)).substr(0,5) + "V\n"));
                                     break;
                                 case SENSORS_FEATURE_FAN: 
                                     itxbf = txtbuff->insert(itxbf,(std::to_string((int)value) + "rpm\n"));
                                     break;
                                 case SENSORS_FEATURE_TEMP: 
                                     if(sn->t_statistic_active) {
                                          ctemp = (value / (double) uhiutil::cpu::max_cpu_t);
                                          if(ctemp > 1.0) ctemp = 1.0;
                                          if(ctemp < 0.0) ctemp = 0.0;
                                          if(sn->max < value) sn->max = value;
                                          sn->SetTemperature(ctemp);
                                     }
                                      else
                                          if(sn->max != 0.0) sn->max = 0.0;
                                     itxbf = txtbuff->insert(itxbf,(Print_Value((float)value) + "°C" + (sn->t_statistic_active ? "" : "\n")));
                                     if(sn->t_statistic_active) itxbf = txtbuff->insert_with_tag (itxbf,("   max: " + (Print_Value((float)sn->max) + "°C" + "\n")),uhiutil::ui::max_tag);
                                     break;
                                 case SENSORS_FEATURE_POWER:
                                     itxbf = txtbuff->insert(itxbf,(Print_Value((float)value) + "W\n"));
                                     break;
                                 case SENSORS_FEATURE_CURR:
                                     itxbf = txtbuff->insert(itxbf,(Print_Value((float)value) + "A\n"));
                                     break;
                                 default: break;
                            }
                    }//sensors
                    chipisset = false;
             }//chip
       }
   }// libsensh

   if(printmode) blink = !blink;
}

void CSysens::PopulateTemperatureSelection(CHWindow *m_wnd)
{
   if(libsensh) {
       Glib::RefPtr<Gtk::ListStore> pTM = m_wnd->ptRefTreeModel;
       int index = 0,indexlimit =  SIZEOF(clrID);
       visible_tmp_sens_count = 0;
       for(Chip_node n : monitoring)  {
              for(Sensor_node sn : n.sensors) {
                    if(sn.sntype == SENSORS_FEATURE_TEMP && sn.visible) {
                        Gtk::TreeModel::Row row = *(pTM->append());
                        row[m_wnd->tColumns->col_tcheck] = sn.t_statistic_active;
                        row[m_wnd->tColumns->tsensor_node] = n.chip_name.cnip_prefix;
                        row[m_wnd->tColumns->tsensor_name] = sn.label;
                        row[m_wnd->tColumns->tnode_id] = n.chip_id;
                        row[m_wnd->tColumns->tsensor_id] = sn.feature_number;
                        row[m_wnd->tColumns->color_name] = clrID[index++];
                        if(index == indexlimit) index = 0;
                        visible_tmp_sens_count ++;
                    }
              }
       }
   }
}

CDrArTempr::DRAWVECTORPLUS CSysens::SensorStatisticToggle(bool status,Glib::ustring color,Glib::ustring node,Glib::ustring sensor,Glib::ustring nodeid,int sensorid,double **max)
{
   if(libsensh) {
       for(std::list<Chip_node>::iterator n = monitoring.begin(); n != monitoring.end(); n++)  {
            if(Glib::ustring(n->chip_name.cnip_prefix) == node && (Glib::ustring(n->chip_id) == nodeid)) {
                for(std::list<Sensor_node>::iterator sn =  n->sensors.begin(); sn != n->sensors.end(); sn++) {
                    if(Glib::ustring(sn->label) == sensor && sn->feature_number == sensorid) {
                    	if(max) {
                            sn->SetStatisticFeatures(status,color);
                            *max = &sn->max;
                    	}
                        return {&sn->t_statistic,&sn->statistic_color};
                    }
                }
            }
       }
   }

   return {nullptr,nullptr};
}
