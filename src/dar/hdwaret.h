/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _DWERAT_H_
#define _DWERAT_H_

#include <iomanip>
#include <chrono>
#include "hdwarev.h"
#include "../util/extdcircle.h"
#include "../util/triangle.h"

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

  int offset_cx = 0;

  using Draw_Item = struct {
      DRAWVECTOR DItem = nullptr;
      const std::string* DItName = nullptr;
      std::string DItSensor = "",DItSensorID = "";
      double *sensormax = nullptr;

      mutable Point wpoint; // changing color
  };

  ExtdPoint tpoint;     // Starting stres test
  std::list<StresTestSession> *const mss; // Stres test session data

  Triangle triangle; // Utility button

  Glib::RefPtr<Gtk::GestureClick> msbntpress;

  CDrArTempr() = default;
  CDrArTempr(UIHWindow* uhiwnd, std::list<StresTestSession> *const mrkss, fp_lDASR ldafp);
  virtual ~CDrArTempr() {EraseAll();}

  void SetAttentState(bool show) {show_msg_attention = show;}
  bool GetAttentState() const {return  show_msg_attention;}
  void SetUnsetDrawItem(const DRAWVECTORPLUS*const item, double *max, Glib::ustring SensorName, Glib::ustring SensorID, bool setflag);
  void EraseAll() {draw_temperatures.clear();tpoint.StopStresTest();mss->clear();}
  const bool HasActivities() const {return !draw_temperatures.empty();}
  std::string CheckingDotMatch(double x, double y) const;
  const bool Get_StresSessionState() const {return tpoint.Get_StresSessionState();}
  std::string GetDurationString();
private:
  bool show_msg_attention = false;
  std::chrono::system_clock::time_point start_time_point;
  std::chrono::duration<double> duration_total_time = std::chrono::duration<double>(0.0);

  virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
  void DrawStrings(const Cairo::RefPtr<Cairo::Context>& cr,std::string duration,int w,int h);
  std::string DurationTimeString(std::chrono::seconds sec) const;
  
  std::list<Draw_Item> draw_temperatures;
};

#endif // _DWERAT_H_
