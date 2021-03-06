/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "mwnd.h"
#include "hdwera.h"
#include "proc.h"
#include <cairomm/context.h>

const Gtk::Switch *CDrawArea::l_CPUModeSwitch = nullptr,*CDrawArea::l_CPUCompareSwitch = nullptr;

CDrawArea::CDrawArea(UIHWindow* uhiwnd,fp_lDASR ldafp,Dm dwm,const TUDRAWVECTOR *dw_frec,const TUDRAWVECTOR *dw_frec_cp,const TUDRAWVECTOR *dw_usg) : DMode(dwm)
{
  set_size_request(draw::dwaw,draw::dwah);
  if(dwm == Dm::CPUDRAW) {
              valfreq = dw_frec;
              valusg = dw_usg;
              valfreqcmpr = dw_frec_cp;
  }
  else {
	  msbntpress = Gtk::GestureClick::create();
	  msbntpress->set_button(GDK_BUTTON_PRIMARY);
	  msbntpress->signal_pressed().connect(sigc::mem_fun(*uhiwnd,ldafp));
	  add_controller(msbntpress);

	  set_margin(2);
  }
  set_draw_func(sigc::mem_fun(*this, &CDrawArea::on_draw));

  set_expand();
}

void CDrawArea::DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X) const
{
	if(DMode == Dm::TEMPERATUREDRAW && !X) {
	   double cw = (dwidth - (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0)) / 2;

       for(int b = 1; b < (int) draw::uhi_draw_xscale ; b *= 2) {
           double cl =  cw; // centering
           int c = 0;

           cl -= ((cw / (double)b) * (b - 1)); // offset

		   while(c++ < b) {
		       crtx->move_to(cl + (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0),0);
			   crtx->line_to(cl + (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0),dheight);
			   cl += ((cw / ((double)b / 2)));
		   }
       }
	}
	else {
       int cnt = ((DMode == Dm::CPUDRAW  && !X) ? (double) dwidth / (double) draw::uhi_draw_xscale : (double) dheight / (double) draw::uhi_draw_yscale),up = cnt;
       while(cnt <= (X ? dheight : dwidth)) {
           crtx->move_to((X ? (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset - draw::dofset :  0) : cnt),(X ? cnt : 0));
           crtx->line_to((X ? dwidth : cnt),(X ? cnt : dheight));
           cnt += up;
       }
	}
   crtx->stroke();
}

void CDrawArea::DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth,StatPaint pm) const
{ 
  if(DMode == Dm::TEMPERATUREDRAW && (tmpmon ? (*tmpmon).size() : 0) < 2) return;

  crtx->move_to(FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0, (DMode == Dm::CPUDRAW ? (pm == StatPaint::USAGE ? (*valusg)[0] :  ( pm == StatPaint::FREQP ?  (*valfreq)[0] : (*valfreqcmpr)[0])) : (tmpmon ? (*tmpmon)[0] : 0)) * dheight);
  if(DMode == Dm::CPUDRAW) crtx->arc(0, (pm == StatPaint::USAGE ? (*valusg)[0] : (pm == StatPaint::FREQP ? (*valfreq)[0] : (*valfreqcmpr)[0])) * dheight, 1.1, 0, 2 * M_PI);
  for(long unsigned int br = 1; br < (DMode == Dm::CPUDRAW ? uhiutil::calc::draw_cpu_statistic : (tmpmon ? (((*tmpmon).size() < uhiutil::calc::t_statistic_len) ? (*tmpmon).size() : uhiutil::calc::t_statistic_len) : 0));br++) {
       if(DMode == Dm::CPUDRAW) 
           crtx->arc(atvy * br, (pm == StatPaint::USAGE ? (*valusg)[br] : (pm == StatPaint::FREQP ? (*valfreq)[br] : (*valfreqcmpr)[br])) * dheight, 1.1, 0, 2 * M_PI);
       else
    	   crtx->line_to(atvy * br + (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0),
    	               		                              ((tmpmon ? (*tmpmon)[br] : 0)) * ((*tmpmon)[br] * dheight >= uhiutil::cpu::max_cpu_t ? dheight - 1 : dheight));
  }
  crtx->stroke();
}

void CDrawArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
  cr->save();
  cr->set_source_rgb(1.0, 1.0, 1.0);
  if(DMode == Dm::TEMPERATUREDRAW)
          DrawStrings(cr,GetDurationString(),width,height);

  Cairo::Matrix matrix(-1.0 , 0.0, 0.0, -1.0, width, height); // axes flipping
  cr->transform(matrix);// (0,0) -> (width,height):(width,0) -> (0,height):(0,height) -> (width,0):...

  double xc = (DMode == Dm::CPUDRAW ? (double)width / ((double)uhiutil::calc::draw_cpu_statistic - 1.0) :
		  ((FULLAPPWNDMODE(width,height) ? ((double)width - (double)draw::xoffset) : (double)width) / ((double)uhiutil::calc::t_statistic_len - 1.0)));

  cr->set_line_width(1.0);
  cr->set_dash(std::vector<double>{1.0}, 1);

  DrawAxis_XY(cr,width,height);      // X axis
  DrawAxis_XY(cr,width,height,true); // Y axis

  cr->unset_dash();

  if(DMode == Dm::CPUDRAW) {
      bool l_cmsactive = l_CPUModeSwitch->get_active(),l_csactive = l_CPUCompareSwitch->get_active();
      cr->set_source_rgb(0.0, 0.75, 1.0);
      cr->set_line_width(1.7);
      DrawActivity(cr,xc,height,0,((l_cmsactive && CProc::m_CpuAltCalc) ? StatPaint::COMPAREFREQP : StatPaint::FREQP));

      cr->set_source_rgb(0.40, 0.80, 0.67);
      cr->set_line_width(1.4);
      DrawActivity(cr,xc,height,0,StatPaint::USAGE);
      
      if(CProc::m_CpuAltCalc && l_csactive) {
          cr->set_source_rgb(0.90, 0.80, 0.40);
          cr->set_line_width(1.2);
          DrawActivity(cr,xc,height,0,(l_cmsactive ? StatPaint::FREQP : StatPaint::COMPAREFREQP));
      }
  }
  else
      if(DMode == Dm::TEMPERATUREDRAW) {
          cr->set_line_width(1.7);
          for(std::list<Draw_Item>::iterator dit = draw_temperatures.begin(); dit != draw_temperatures.end(); dit++)  {
                           tmpmon = dit->DItem;
                           Gdk::RGBA rgba(dit->DItName);
                           Gdk::Cairo::set_source_rgba(cr,rgba);
                           DrawActivity(cr,xc,height,width);
          }
      }
  cr->restore();
}

void CDrawArea::SetUnsetDrawItem(DRAWVECTOR item, double *max, Glib::ustring ColorName, Glib::ustring SensorName, bool setflag)
{
	if(!draw_temperatures.size()) duration_total_time = std::chrono::duration<double>(0.0);

    if(setflag)  {
          Draw_Item di;
          di.DItem = item;
          di.DItName = ColorName;
          di.DItSensor = SensorName;
          di.sensormax = max;
          draw_temperatures.push_back(di);
          if(draw_temperatures.size() == 1) start_time_point = std::chrono::system_clock::now();
    }
    else {
        for(std::list<Draw_Item>::iterator dit = draw_temperatures.begin(); dit != draw_temperatures.end(); dit++)  {
            if(item == dit->DItem) {
                draw_temperatures.erase(dit);
                break;
            }
        }
    }
}

std::string CDrawArea::DurationTimeString(std::chrono::seconds sec) const
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

std::string CDrawArea::GetDurationString()
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

void CDrawArea::DrawStrings(const Cairo::RefPtr<Cairo::Context>& cr,std::string duration,int w,int h)
{
	  int width = 0,height = 0,dtxt = 0;
	  const int scale = uhiutil::cpu::max_cpu_t / draw::uhi_draw_yscale;
	  Glib::RefPtr<Pango::Layout> layout = create_pango_layout(duration.c_str());
	  Pango::FontDescription font = DA_DrawFont();

	  layout->set_font_description(font);
	  cr->move_to(draw::dofset,draw::dofset);
	  layout->show_in_cairo_context(cr); // duration

	  if(FULLAPPWNDMODE(w,h) && DA_VcoreVal && *DA_VcoreVal && !(duration == "0:00:00")) {
		  cr->save();
		  cr->set_source_rgb(0.8, 0.2, 0.4);
		  DA_Text(layout, width , height,"\nVcore : " + (std::to_string(*DA_VcoreVal)).substr(0,5) + "V");
		  DADRAWTEXT(cr, layout, draw::dofset,0); // Vcore
		  cr->restore();
	  }

	  layout->unset_font_description();
	  font = DA_DrawFont(false);
	  layout->set_font_description(font);

	  for(int br  = 0; br <= (int) draw::uhi_draw_yscale ;
			  ((m_TmpWndCurrState == DAWndState::NORMAL && br < (int) draw::uhi_draw_yscale) ?
			    	                    br = (int) draw::uhi_draw_yscale : br++), dtxt = scale * br) {
	       DA_Text(layout, width, height, std::to_string(dtxt) + " ??");
	       DADRAWTEXT(cr, layout, (FULLAPPWNDMODE(w,h)) ? w - (draw::xoffset + width + draw::dofset) : w - (width + draw::dofset),
	    		(br == (int) draw::uhi_draw_yscale ? draw::dofset :
	    				((h - ((h / draw::uhi_draw_yscale) * br)) - (height + draw::dofset / 2)))); // temperature points
	  }

	  DA_Text(layout, width, height,
			  std::to_string((int)std::ceil(((double)uhiutil::calc::t_statistic_len - 1) * (double)((double)uhiutil::timer_interval / (double)1000))) + " s");
	  DADRAWTEXT(cr, layout, draw::dofset,h - (height + draw::dofset)); // "page" max time duration

	  if(m_TmpWndCurrState == DAWndState::FULL) {
	       double tcvr = 0.0;
	       dtxt = 1;
	       cr->save();
	       for(std::list<Draw_Item>::const_iterator dit = draw_temperatures.begin(); dit != draw_temperatures.end(); dit++,dtxt++)  {
	              if(dit->DItem && ((*dit->DItem).size() >= 2)) {
	                     Gdk::Cairo::set_source_rgba(cr,Gdk::RGBA{dit->DItName});
	                     tcvr = ((*dit->DItem)[0] * (double) uhiutil::cpu::max_cpu_t);

	                     DA_Text(layout, width, height, (std::to_string(tcvr).substr(0,STRCOND(tcvr,5,4)) + " ??").c_str());
	                     DADRAWTEXT(cr, layout,w - (((FULLAPPWND(w,h)) ? draw::xoffset  - draw::dofset : width  + draw::dofset)),
	                    		 tcvr >= uhiutil::cpu::max_cpu_t ? draw::dofset :
	                    		                         ((h - ((*dit->DItem)[0] * (h + draw::dofset))) - (height + draw::dofset)) );  // current temperature

	                     DA_Text(layout, width, height,
	                    		 dit->DItSensor + "  MAX : "+ (dit->sensormax ? (std::to_string(*dit->sensormax).substr(0,STRCOND(tcvr,5,4)) + " ??") : "N/A"));
	                     DADRAWTEXT(cr,layout,w / 2 - width / 2,(height * dtxt) - draw::dofset); // temperature source

	              }
	       }
	       cr->restore();
	       if(FULLAPPWNDMODE(w,h)) {
	 	       DA_Text(layout, width , height,
	 	   	   std::to_string((int)std::ceil((((double)uhiutil::calc::t_statistic_len - 1) / (double)2) * (double)((double)uhiutil::timer_interval / (double)1000))) + " s");
	 	       DADRAWTEXT(cr, layout, ((w - draw::xoffset) / 2) + draw::dofset ,h - (height + draw::dofset)); // half time
	       }

	  }
}
