/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _DWERAT_H_
#define _DWERAT_H_

#include <iomanip>
#include <chrono>
#include "hdwareuhi.h"

class UIHWindow;
namespace draw = uhiutil::draw;

class CDrArTempr : public CDrArUhi
{
public:
  using fp_lDASR = void (UIHWindow::*)(int, double, double);
  using DRAWVECTOR = const std::vector<double>*;
  using DRAWVECTORPLUS = struct {
	         DRAWVECTOR dvc;
	         const std::string* dsn;
  };

  Glib::RefPtr<Gtk::GestureClick> msbntpress;

  CDrArTempr(UIHWindow* uhiwnd,fp_lDASR ldafp);
  virtual ~CDrArTempr() = default;

  void SetUnsetDrawItem(const DRAWVECTORPLUS*const item, double *max, Glib::ustring SensorName, bool setflag);
  void EraseAll() {draw_temperatures.clear();}
  const bool HasActivities() const {return !draw_temperatures.empty();}
  double**const GetDAVcoreAccess() {return &DA_VcoreVal;}
private:
  using Draw_Item = struct {
      DRAWVECTOR DItem = nullptr;
      const std::string* DItName = nullptr;
      std::string DItSensor = "";
      double *sensormax = nullptr;
  };

  DRAWVECTOR tmpmon = nullptr;
  double *DA_VcoreVal = nullptr;

  std::chrono::system_clock::time_point start_time_point;
  std::chrono::duration<double> duration_total_time = std::chrono::duration<double>(0.0);

  virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
  void DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth = 0) const;
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

#endif // _DWERAT_H_
