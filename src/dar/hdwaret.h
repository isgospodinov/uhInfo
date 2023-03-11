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
using Sensor_node = struct _Sensor_node;

class CDrArTempr : public CDrArUhi
{
public:
  using fp_lDASR = void (UIHWindow::*)(int, double, double);
  using DRAWVECTOR = const std::vector<double>*;
  using VCORESBUNCH = std::list<Sensor_node*>;
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

  using TmpWndState = enum class DAWndState{NORMAL,FULL};

  Glib::RefPtr<Gtk::GestureClick> msbntpress;

  CDrArTempr() = default;
  CDrArTempr(UIHWindow* uhiwnd,fp_lDASR ldafp);
  virtual ~CDrArTempr() = default;

  TmpWndState m_TmpWndCurrState = DAWndState::NORMAL;

  void SetAttentState(bool show) {show_msg_attention = show;}
  bool GetAttentState() const {return  show_msg_attention;}
  void SetUnsetDrawItem(const DRAWVECTORPLUS*const item, double *max, Glib::ustring SensorName, bool setflag);
  void EraseAll() {draw_temperatures.clear();}
  const bool HasActivities() const {return !draw_temperatures.empty();}
  VCORESBUNCH*const GetDAVcoreAccess() {return &draw_Vcores;}
  const bool HasVCores() const {return !draw_Vcores.empty();}
protected:
  DRAWVECTOR tmpmon = nullptr;
  virtual void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X = false) const override;
  void DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth = 0) const;

  void DA_Text(Glib::RefPtr<Pango::Layout>& ly,int& dw,int& dh, std::string dt) const {ly->set_text(dt);ly->get_pixel_size(dw,dh);}
private:
  VCORESBUNCH draw_Vcores;
  bool show_msg_attention = false;

  std::chrono::system_clock::time_point start_time_point;
  std::chrono::duration<double> duration_total_time = std::chrono::duration<double>(0.0);

  virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
  void DrawStrings(const Cairo::RefPtr<Cairo::Context>& cr,std::string duration,int w,int h);
  std::string DurationTimeString(std::chrono::seconds sec) const;
  std::string GetDurationString();

  std::list<Draw_Item> draw_temperatures;
};

#endif // _DWERAT_H_
