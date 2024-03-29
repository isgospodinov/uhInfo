/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "../mwnd.h"
#include "../proc.h"
#include <cairomm/context.h>
#include "hdwaret.h"

CDrArTempr::CDrArTempr(UIHWindow* uhiwnd, std::list<StresTestSession> *const mrkss, fp_lDASR ldafp) : mss(mrkss)
{
	  msbntpress = Gtk::GestureClick::create();
	  msbntpress->set_button();
	  msbntpress->signal_pressed().connect(sigc::mem_fun(*uhiwnd,ldafp));
	  add_controller(msbntpress);

	  set_margin(2);
}

void CDrArTempr::on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
  cr->save();
  cr->set_source_rgb(1.0, 1.0, 1.0);

  DrawStrings(cr,GetDurationString(),width,height);

  Cairo::Matrix matrix(-1.0 , 0.0, 0.0, -1.0, width, height); // axes flipping
  cr->transform(matrix);// (0,0) -> (width,height):(width,0) -> (0,height):(0,height) -> (width,0):...

  double xc = ((FULLAPPWNDMODE(width,height) ? ((double)width - (double)draw::xoffset) : (double)width) / ((double)uhiutil::calc::t_statistic_len - 1.0));

  cr->set_line_width(1.0);
  cr->set_dash(std::vector<double>{1.0}, 1);

  DrawAxis_XY(cr,width,height);      // X axis
  DrawAxis_XY(cr,width,height,true); // Y axis

  cr->unset_dash();
  cr->set_line_width(1.7);
  for(std::list<Draw_Item>::iterator dit = draw_temperatures.begin(); dit != draw_temperatures.end(); dit++)  {
             Gdk::RGBA rgba(*dit->DItName);
             Gdk::Cairo::set_source_rgba(cr,rgba);
             DrawActivity(cr,dit->DItem,xc,height,width);
  }

  if(uhiutil::draw::marck_strses && FULLAPPWNDMODE(width,height) && !mss->empty()) {
	  Glib::RefPtr<Pango::Layout> lt = create_pango_layout("");
	  lt->set_font_description(DA_DrawFont(false));
	  cr->set_source_rgb(1.0, 1.0, 1.0);
	  int dw = 0,dh = 0,ex;

	  for(StresTestSession s : *mss) {
		  s.drawing_request(cr,xc,height);

		  cr->save();
		  matrix.invert();
		  cr->transform(matrix);
		  ex = 0;

		  do {
			  if(!(s.stoptime != "" || ex)) continue;
		      lt->set_text((s.stoptime != "" && ex)? s.stoptime: s.startime);
		      lt->get_pixel_size(dw,dh);
		      cr->move_to(width - ((s.stoptime != "" && ex) ? ((xc * s.cn_endoffset) + uhiutil::draw::xoffset + (dw / 2)) :
				           ((xc * (s.cn_startoffset + (s.cn_endoffset ? s.cn_endoffset : 0))) + uhiutil::draw::xoffset + (dw / 2))),
				                                                                              height - dh - (uhiutil::draw::xoffset * 2 / 3));
		      lt->show_in_cairo_context(cr);
		  }while(ex++ < 1);
		  cr->restore();
	  }
  }

  cr->restore();
}

void CDrArTempr::SetUnsetDrawItem(const DRAWVECTORPLUS*const item, double *max, Glib::ustring SensorName, Glib::ustring SensorID, bool setflag)
{
	if(!draw_temperatures.size()) duration_total_time = std::chrono::duration<double>(0.0);

    if(setflag)  {
          draw_temperatures.push_back({item->dvc,item->dsn,SensorName,SensorID,max});

          if(draw_temperatures.size() == 1)
        	         start_time_point = std::chrono::system_clock::now();
    }
    else {
        for(std::list<Draw_Item>::iterator dit = draw_temperatures.begin(); dit != draw_temperatures.end(); dit++)  {
              if(item->dvc == dit->DItem) {
                draw_temperatures.erase(dit);
                offset_cx = 0;
                break;
            }
        }
    }
}

std::string CDrArTempr::DurationTimeString(std::chrono::seconds sec) const
{
    std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(sec);
    sec -= h;
    std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(sec);
    sec -= m;
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(sec);

    int hc = h.count(),mc = m.count(),sc = s.count();

    std::stringstream string_duration;
    string_duration.fill('0');

    if(hc)
        string_duration << h.count() << ":";
    else 
        string_duration << "0:";

    string_duration << std::setw(2);

    if(hc || mc)
        string_duration << m.count() << ":";
    else
         string_duration << "00:";

    string_duration << std::setw(2);

    if(hc || mc || sc)
        string_duration << s.count();
    else 
        string_duration << "00";

    return string_duration.str();
}

std::string CDrArTempr::GetDurationString()
{
   std::string dtsec("0:00:00");
   if(draw_temperatures.size()) {
       std::chrono::system_clock::time_point nowtime = std::chrono::system_clock::now();
       std::chrono::duration<double> elapsed_time = nowtime - start_time_point;
       start_time_point = nowtime;
       duration_total_time += elapsed_time;
       double result = duration_total_time.count();
       if(((int)result) > uhiutil::calc::resetime) {
           duration_total_time = std::chrono::duration<double>(0.0);
           result = duration_total_time.count();
       }

       dtsec = DurationTimeString(std::chrono::seconds((int) result));
   }

   return dtsec;
}

void CDrArTempr::DrawStrings(const Cairo::RefPtr<Cairo::Context>& cr,std::string duration,int w,int h)
{
	  int width = 0,height = 0,dtxt = 0,mv = 0;
	  const int scale = uhiutil::cpu::max_cpu_t / draw::uhi_draw_yscale;
	  triangle.set_keypoint((FULLAPPWNDMODE(w,h) ? w : 0));
	  Glib::RefPtr<Pango::Layout> layout = create_pango_layout(duration.c_str());
	  layout->set_font_description(DA_DrawFont());
	  cr->move_to(draw::dofset,draw::dofset);
	  layout->show_in_cairo_context(cr); // duration
	  layout->get_pixel_size(width,height);
	  mv = height + draw::dofset;

	  layout->unset_font_description();
	  layout->set_font_description(DA_DrawFont(true,14));

	  if(FULLAPPWNDMODE(w,h) && HasVFSensors() && !(duration == "0:00:00") && draw::more_info) {
		  cr->save();
		  for(Sensor_node *di : dwVCF)  {
			   if(di->visible) {
					cr->save();
					cr->set_source_rgb(di->is_Vcore ? 0.8 : 0.3, 0.2, di->is_Vcore ? 0.4 : 0.9);
			        DA_Text(layout, width , height,di->label + " : " + (di->is_Vcore ? (std::to_string(di->max)).substr(0,5) : (std::to_string((int)di->max)).substr(0,5)) + (di->is_Vcore ? "V" : "rpm"));
			        DADRAWTEXT(cr, layout, draw::dofset,mv); // Vcore or Fan
			        mv += height;
			        cr->restore();
			   }
		  }
		  cr->restore();
	  }

	  if(m_TmpWndCurrState != DAWndState::FULL && show_msg_attention) {
	       DA_Text(layout, width, height, (!HasActivities() ? _("Empty selection") : _("Color change in action")));
	       DADRAWTEXT(cr,layout,(w / 2) - width / 2, h - (h / 2));
	  }

	  layout->unset_font_description();
	  layout->set_font_description(DA_DrawFont(false));

	  for(int br  = 0; br <= (int) draw::uhi_draw_yscale ;
			  ((m_TmpWndCurrState == DAWndState::NORMAL && br < (int) draw::uhi_draw_yscale) ?
			    	                    br = (int) draw::uhi_draw_yscale : br++), dtxt = scale * br) {
	       DA_Text(layout, width, height, std::to_string(dtxt) + " °");
	       DADRAWTEXT(cr, layout, (FULLAPPWNDMODE(w,h)) ? w - (draw::xoffset + width + draw::dofset) : w - (width + draw::dofset),
	    		(br == (int) draw::uhi_draw_yscale ? draw::dofset :
	    				((h - ((h / draw::uhi_draw_yscale) * br)) - (height + draw::dofset / 2)))); // temperature points
	  }

	  DA_Text(layout, width, height,
			  std::to_string((int)std::ceil(((double)uhiutil::calc::t_statistic_len - 1) * (double)((double)uhiutil::timer_interval / (double)1000))) + " s");
	  DADRAWTEXT(cr, layout, draw::dofset,h - (height + draw::dofset)); // "page" max time duration

	  if(m_TmpWndCurrState == DAWndState::FULL) {
	       double tcvr = 0.0;
	       int correction_cx = 0;
	       dtxt = 1;
	       cr->save();
	       for(std::list<Draw_Item>::const_iterator dit = draw_temperatures.begin(); dit != draw_temperatures.end(); dit++,dtxt++)  {
	              if(dit->DItem && ((*dit->DItem).size() >= 2)) {
	                     Gdk::Cairo::set_source_rgba(cr,Gdk::RGBA{*dit->DItName});
	                     tcvr = ((*dit->DItem)[0] * (double) uhiutil::cpu::max_cpu_t);

	                     DA_Text(layout, width, height, (std::to_string(tcvr).substr(0,STRCOND(tcvr,5,4)) + " °").c_str());
	                     DADRAWTEXT(cr, layout,w - (((FULLAPPWND(w,h)) ? draw::xoffset  - draw::dofset : width  + draw::dofset)),
	                    		 tcvr >= uhiutil::cpu::max_cpu_t ? draw::dofset :
	                    		                         ((h - ((*dit->DItem)[0] * (h + draw::dofset))) - (height + draw::dofset)) );  // current temperature

	                     DA_Text(layout, width, height,
	                    		 dit->DItSensor + "  MAX : "+ (dit->sensormax ? (std::to_string(*dit->sensormax).substr(0,STRCOND(tcvr,5,4)) + " °") : "N/A"));
	                     DADRAWTEXT(cr,layout,w / 2 - width / 2,(height * dtxt) - draw::dofset); // temperature source

	                     offset_cx = ((offset_cx <= 0) ? ((w / 2 - width / 2) - 11) : (std::min((w / 2 - width / 2) - 11, offset_cx)));
	                     correction_cx = ((correction_cx <= 0) ? ((w / 2 - width / 2) - 11) : (std::min((w / 2 - width / 2) - 11, correction_cx)));

			             cr->save();
					     cr->set_source_rgb(1.0, 1.0, 1.0);
					     bool dp = dit->wpoint.dr;
					     dit->wpoint = {(int) offset_cx,(int)(((height * dtxt) - draw::dofset) + 6),false};
					     cr->arc(dit->wpoint.cx, dit->wpoint.cy, draw::bp_radius - (!dp ? 0 : 2), 0, 2 * M_PI);
					     cr->fill();
					     cr->restore();
	              }
	       }

	       if(!(offset_cx == correction_cx))
	                offset_cx = std::max(offset_cx, correction_cx);

	       cr->restore();
	       if(FULLAPPWNDMODE(w,h)) {
	 	       DA_Text(layout, width , height,
	 	   	   std::to_string((int)std::ceil((((double)uhiutil::calc::t_statistic_len - 1) / (double)2) * (double)((double)uhiutil::timer_interval / (double)1000))) + " s");
	 	       DADRAWTEXT(cr, layout, ((w - draw::xoffset) / 2) + draw::dofset ,h - (height + draw::dofset)); // half time
	       }

           triangle.drawing_request(cr);

           tpoint = {(int)(w / 2), (int)(((height * dtxt) + draw::dofset) + 8), tpoint.dr, tpoint.pIDs}; //tpoint = {(int) (w - (draw::xoffset / 4 + (draw::dofset))), (int)((draw::xoffset / 2 + (draw::dofset)) + 19), tpoint.dr,tpoint.pIDs};
           tpoint.drawing_request(cr);
	  }
}

std::string CDrArTempr::CheckingDotMatch(double x, double y) const
{
	std::string res = "";
	for(std::list<Draw_Item>::const_iterator di = draw_temperatures.begin(); di != draw_temperatures.end(); di++)  {
		if(di->wpoint.CheckingDotMatch(x, y)) {
			 res = di->DItSensorID;
			 break;
		}
	}

	return res;
}
