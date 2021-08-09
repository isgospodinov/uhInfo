/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _DWERA_H_
#define _DWERA_H_

#include "util.h"
#include <iomanip>

class CHWindow;

class CDrawArea : public Gtk::DrawingArea
{
public:
  using StatPaint = enum class PaintMode {FREQP,COMPAREFREQP,USAGE,TEMPERATUREP};
  using TmpWndState = enum class DAWndState {NORMAL,FULL};
  using fp_lDASR = bool (CHWindow::*)(GdkEventButton*);
  using DRAWVECTOR = const std::vector<double>*;
  using TUDRAWVECTOR = std::array<double, uhiutil::calc::draw_cpu_statistic>;
    
  CDrawArea(CHWindow* uhiwnd,fp_lDASR ldafp,Dm dwm = Dm::TEMPERATUREDRAW,const TUDRAWVECTOR *dw_frec = nullptr,const TUDRAWVECTOR *dw_frec_cp = nullptr,const TUDRAWVECTOR *dw_usg = nullptr);
  virtual ~CDrawArea() = default;

  void Redraw();
  void SetUnsetDrawItem(DRAWVECTOR item, Glib::ustring ColorName ,bool setflag);
  void EraseAll() {draw_temperatures.clear();}

  static const Gtk::Switch *l_CPUModeSwitch,*l_CPUCompareSwitch;
  TmpWndState m_TmpWndCurrState = DAWndState::NORMAL;
private:
  using Draw_Item = struct {
      DRAWVECTOR DItem = nullptr;
      std::string DItName = "";
  };

  DRAWVECTOR tmpmon = nullptr;
  const TUDRAWVECTOR *valfreq = nullptr, *valfreqcmpr = nullptr, *valusg = nullptr;
  DrawMode DMode;

  std::chrono::system_clock::time_point start_time_point;
  std::chrono::duration<double> duration_total_time = std::chrono::duration<double>(0.0);

  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int xyc,int dwidth,int dheight,bool X = false) const;
  void DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,StatPaint pm = StatPaint::TEMPERATUREP) const ;
  void DrawStrings(const Cairo::RefPtr<Cairo::Context>& cr,std::string duration,int w,int h);
  std::string DurationTimeString(std::chrono::seconds sec) const;
  std::string GetDurationString();

  std::list<Draw_Item> draw_temperatures;
};

#endif // _DWERA_H_
