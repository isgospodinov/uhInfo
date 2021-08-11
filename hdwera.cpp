/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#include "mwnd.h"
#include "hdwera.h"
#include "proc.h"
#include <cairomm/context.h>

const Gtk::Switch *CDrawArea::l_CPUModeSwitch = nullptr,*CDrawArea::l_CPUCompareSwitch = nullptr;

CDrawArea::CDrawArea(CHWindow* uhiwnd,fp_lDASR ldafp,Dm dwm,const TUDRAWVECTOR *dw_frec,const TUDRAWVECTOR *dw_frec_cp,const TUDRAWVECTOR *dw_usg) : DMode(dwm)
{
  set_size_request(uhiutil::draw::dwaw,uhiutil::draw::dwah);
  if(dwm == Dm::CPUDRAW) {
              valfreq = dw_frec;
              valusg = dw_usg;
              valfreqcmpr = dw_frec_cp;
  }
  else {
	  add_events(Gdk::BUTTON_PRESS_MASK);
	  signal_button_press_event().connect(sigc::mem_fun(*uhiwnd,ldafp));
  }
}

void CDrawArea::Redraw()
{
   get_window()->invalidate_rect(Gdk::Rectangle(0, 0, get_width(), get_height()), false);
}

void CDrawArea::DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int xyc,int dwidth,int dheight,bool X) const
{
  int cnt = xyc ;
  while(cnt < (X ? dheight : dwidth)) {
     crtx->move_to((X ? 0 : cnt),(X ? cnt : 0));
     crtx->line_to((X ? dwidth : cnt),( X ? cnt : dheight));
     cnt += xyc;
  }
  crtx->stroke();
}

void CDrawArea::DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,StatPaint pm) const
{ 
  if(DMode == Dm::TEMPERATUREDRAW && (tmpmon ? (*tmpmon).size() : 0) < 2) return;

  crtx->move_to(0, (DMode == Dm::CPUDRAW ? (pm == StatPaint::USAGE ? (*valusg)[0] :  ( pm == StatPaint::FREQP ?  (*valfreq)[0] : (*valfreqcmpr)[0]  )  ) : (tmpmon ? (*tmpmon)[0] : 0)) * dheight);
  if(DMode == Dm::CPUDRAW) crtx->arc(0, (pm == StatPaint::USAGE ? (*valusg)[0] : (pm == StatPaint::FREQP ? (*valfreq)[0] : (*valfreqcmpr)[0])) * dheight, 1.1, 0, 2 * M_PI);
  for(long unsigned int br = 1; br < (DMode == Dm::CPUDRAW ? uhiutil::calc::draw_cpu_statistic : (tmpmon ? (((*tmpmon).size() < uhiutil::calc::t_statistic_len) ? (*tmpmon).size() : uhiutil::calc::t_statistic_len) : 0)) ;br++) {
       if(DMode == Dm::CPUDRAW) 
           crtx->arc(atvy * br, (pm == StatPaint::USAGE ? (*valusg)[br] : (pm == StatPaint::FREQP ? (*valfreq)[br] : (*valfreqcmpr)[br])) * dheight, 1.1, 0, 2 * M_PI);
       else 
           crtx->line_to(atvy * br, ((tmpmon ? (*tmpmon)[br] : 0)) * dheight);
  }
  crtx->stroke(); 
}

bool CDrawArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation(); 
  int width = allocation.get_width(),height = allocation.get_height();
  cr->set_source_rgb(1.0, 1.0, 1.0);
  if(DMode == Dm::TEMPERATUREDRAW)
          DrawStrings(cr,GetDurationString(),width,height);

  Cairo::Matrix matrix(-1.0 , 0.0, 0.0, -1.0, width, height); // axes flipping
  cr->transform(matrix);// (0,0) -> (width,height):(width,0) -> (0,height):(0,height) -> (width,0):...

  double xc = (double)width / (double)uhiutil::draw::uhi_draw_xscale;
  int yc = (double)height / (double)uhiutil::draw::uhi_draw_yscale;

  cr->set_line_width(1.0);
  cr->set_dash(std::vector<double>{1.0}, 1);

  DrawAxis_XY(cr,xc,width,height);
  DrawAxis_XY(cr,yc,width,height,true);

  cr->unset_dash();

  if(DMode == Dm::CPUDRAW) {
      xc = (double)width / ((double)uhiutil::calc::draw_cpu_statistic - 1.0);
      bool l_cmsactive = l_CPUModeSwitch->get_active(),l_csactive = l_CPUCompareSwitch->get_active();

      cr->set_source_rgb(0.0, 0.75, 1.0);
      cr->set_line_width(1.7);
      DrawActivity(cr,xc,height,((l_cmsactive && CProc::m_CpuAltCalc) ? StatPaint::COMPAREFREQP : StatPaint::FREQP));

      cr->set_source_rgb(0.40, 0.80, 0.67);
      cr->set_line_width(1.4);
      DrawActivity(cr,xc,height,StatPaint::USAGE);
      
      if(CProc::m_CpuAltCalc && l_csactive) {
          cr->set_source_rgb(0.90, 0.80, 0.40);
          cr->set_line_width(1.2);
          DrawActivity(cr,xc,height,(l_cmsactive ? StatPaint::FREQP : StatPaint::COMPAREFREQP));
      }
  }
  else
      if(DMode == Dm::TEMPERATUREDRAW) {
          xc = ((double)width / ((double)uhiutil::calc::t_statistic_len - 1.0));
          cr->set_line_width(1.7);
          for(std::list<Draw_Item>::iterator dit = draw_temperatures.begin(); dit != draw_temperatures.end(); dit++)  {
                           tmpmon = dit->DItem;
                           Gdk::RGBA rgba(dit->DItName);
                           Gdk::Cairo::set_source_rgba(cr,rgba);
                           DrawActivity(cr,xc,height);
          }
      }

  return true;
}

void CDrawArea::SetUnsetDrawItem(DRAWVECTOR item, Glib::ustring ColorName ,bool setflag)
{
    if(!draw_temperatures.size()) duration_total_time = std::chrono::duration<double>(0.0);

    if(setflag)  {
          Draw_Item di;
          di.DItem = item;
          di.DItName = ColorName;
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
  int width = 0,height = 0; 
  Pango::FontDescription font;
  Glib::RefPtr<Pango::Layout> layout = create_pango_layout(duration.c_str());

  font.set_family(uhiutil::draw::text_font_family);
  font.set_weight(Pango::WEIGHT_BOLD);
  font.set_style(Pango::STYLE_ITALIC);
  font.set_size(uhiutil::draw::dtxtmax * PANGO_SCALE);
  layout->set_font_description(font);
  cr->move_to(uhiutil::draw::dofset,uhiutil::draw::dofset);
  layout->show_in_cairo_context(cr); // duration

  layout->unset_font_description();
  font.set_weight(Pango::WEIGHT_THIN);
  font.set_style(Pango::STYLE_NORMAL);
  font.set_size(uhiutil::draw::dtxthin * PANGO_SCALE);
  layout->set_font_description(font);

  layout->set_text(std::to_string(uhiutil::cpu::max_cpu_t) + " °");
  layout->get_pixel_size(width,height);
  cr->move_to(w - (width + uhiutil::draw::dofset),uhiutil::draw::dofset);
  layout->show_in_cairo_context(cr);// max temperature

  layout->set_text("0");
  layout->get_pixel_size(width,height);
  cr->move_to(w - (width + uhiutil::draw::dofset),h - (height + uhiutil::draw::dofset));
  layout->show_in_cairo_context(cr); // start point

  if(m_TmpWndCurrState == DAWndState::FULL) {
      for (int br  = 1; br < (int)uhiutil::draw::uhi_draw_yscale ; br++) {
	     layout->set_text(std::to_string((uhiutil::cpu::max_cpu_t / uhiutil::draw::uhi_draw_yscale) * br) + " °");
	     layout->get_pixel_size(width,height);
	     cr->move_to(w - (width + uhiutil::draw::dofset),( (h - ((h / uhiutil::draw::uhi_draw_yscale) * br)) - (height + uhiutil::draw::dofset / 2) ) );
	     layout->show_in_cairo_context(cr);
      }
  }

  layout->set_text(std::to_string((int)((uhiutil::calc::t_statistic_len - 1) * (float)((float)uhiutil::timer_interval / (float)1000)) + 1) + " s");
  layout->get_pixel_size(width,height);
  cr->move_to(uhiutil::draw::dofset,h - (height + uhiutil::draw::dofset));
  layout->show_in_cairo_context(cr); // "page" max time duration*/
}
