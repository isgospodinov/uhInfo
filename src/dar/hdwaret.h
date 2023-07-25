/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _DWERAT_H_
#define _DWERAT_H_

#include <iomanip>
#include <chrono>
#include "hdwarev.h"

class UIHWindow;
namespace draw = uhiutil::draw;

class CDrArTempr : public CDrArVcore
{
public:
  using fp_lDASR = void (UIHWindow::*)(int, double, double);
  using DRAWVECTORPLUS = struct {
	         DRAWVECTOR dvc;
	         const std::string* dsn;
  };

  using Draw_Item = struct {
      DRAWVECTOR DItem = nullptr;
      const std::string* DItName = nullptr;
      std::string DItSensor = "";
      double *sensormax = nullptr;
  };

  Glib::RefPtr<Gtk::GestureClick> msbntpress;

  CDrArTempr() = default;
  CDrArTempr(UIHWindow* uhiwnd,fp_lDASR ldafp);
  virtual ~CDrArTempr() = default;

  void SetAttentState(bool show) {show_msg_attention = show;}
  bool GetAttentState() const {return  show_msg_attention;}
  void SetUnsetDrawItem(const DRAWVECTORPLUS*const item, double *max, Glib::ustring SensorName, bool setflag);
  void EraseAll() {draw_temperatures.clear();}
  const bool HasActivities() const {return !draw_temperatures.empty();}
  bool CheckingDotMatch(double &x, double &y) const {if (((bluepoint.cx - x) * (bluepoint.cx - x)) + ((bluepoint.cy - y) * (bluepoint.cy - y)) <= (draw::bp_radius * draw::bp_radius)) return true; else return false;}
private:
  bool show_msg_attention = false;
  std::chrono::system_clock::time_point start_time_point;
  std::chrono::duration<double> duration_total_time = std::chrono::duration<double>(0.0);

  struct {int cx = 0, cy = 0;} bluepoint;

  virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
  void DrawStrings(const Cairo::RefPtr<Cairo::Context>& cr,std::string duration,int w,int h);
  std::string DurationTimeString(std::chrono::seconds sec) const;
  std::string GetDurationString();

  std::list<Draw_Item> draw_temperatures;
};

#endif // _DWERAT_H_
