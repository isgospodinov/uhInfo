/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _DWERA_H_
#define _DWERA_H_

#include "util.h"
#include <iomanip>
#include <chrono>

class UIHWindow;
namespace draw = uhiutil::draw;

class CDrawArea : public Gtk::DrawingArea
{
public:
  using StatPaint = enum class PaintMode {FREQP,COMPAREFREQP,USAGE,TEMPERATUREP};
  using TmpWndState = enum class DAWndState {NORMAL,FULL};
  using fp_lDASR = void (UIHWindow::*)(int, double, double);
  using DRAWVECTOR = const std::vector<double>*;
  using TUDRAWVECTOR = std::array<double, uhiutil::calc::draw_cpu_statistic>;

  Glib::RefPtr<Gtk::GestureClick> msbntpress;

  CDrawArea(UIHWindow* uhiwnd,fp_lDASR ldafp,Dm dwm = Dm::TEMPERATUREDRAW,const TUDRAWVECTOR *dw_frec = nullptr,const TUDRAWVECTOR *dw_frec_cp = nullptr,const TUDRAWVECTOR *dw_usg = nullptr);
  virtual ~CDrawArea() = default;

  void Redraw() {queue_draw();}
  void SetUnsetDrawItem(DRAWVECTOR item, double *max, Glib::ustring ColorName, Glib::ustring SensorName, bool setflag);
  void EraseAll() {draw_temperatures.clear();}
  double**const GetDAVcoreAccess() {return &DA_VcoreVal;}

  static const Gtk::Switch *l_CPUModeSwitch,*l_CPUCompareSwitch;
  TmpWndState m_TmpWndCurrState = DAWndState::NORMAL;
private:
  using Draw_Item = struct {
      DRAWVECTOR DItem = nullptr;
      std::string DItName = "",DItSensor = "";
      double *sensormax = nullptr;
  };

  DRAWVECTOR tmpmon = nullptr;
  const TUDRAWVECTOR *valfreq = nullptr, *valfreqcmpr = nullptr, *valusg = nullptr;
  DrawMode DMode;
  double *DA_VcoreVal = nullptr;

  std::chrono::system_clock::time_point start_time_point;
  std::chrono::duration<double> duration_total_time = std::chrono::duration<double>(0.0);

  void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
  void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X = false) const;
  void DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth = 0,StatPaint pm = StatPaint::TEMPERATUREP) const;
  void DrawStrings(const Cairo::RefPtr<Cairo::Context>& cr,std::string duration,int w,int h);
  std::string DurationTimeString(std::chrono::seconds sec) const;
  std::string GetDurationString();
  void DA_Text(Glib::RefPtr<Pango::Layout>& ly,int& dw,int& dh, std::string dt) const {ly->set_text(dt);ly->get_pixel_size(dw,dh);}
  const Pango::FontDescription DA_DrawFont(bool fd = true)  const { Pango::FontDescription font; font.set_family(draw::text_font_family);
		font.set_weight(fd ? Pango::Weight::BOLD : Pango::Weight::THIN);
		font.set_style(fd ? Pango::Style::ITALIC : Pango::Style::NORMAL);
		font.set_size((fd ? draw::dtxtmax : draw::dtxthin) * PANGO_SCALE);
		return font;
  }

  std::list<Draw_Item> draw_temperatures;
};

#endif // _DWERA_H_
