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

  int offset_cx = 0;

  using Draw_Item = struct {
      DRAWVECTOR DItem = nullptr;
      const std::string* DItName = nullptr;
      std::string DItSensor = "",DItSensorID = "";
      double *sensormax = nullptr;

      mutable struct {
    	int cx = 0, cy = 0;
        bool dr = false;
      } wpoint;
  };

  struct {
    private:
	   int keypoint = 0;
	   bool visible = false;

	   double calctr(int px1, int py1, int px2, int py2, int px3, int py3) const {
	      return std::abs((px1 * (py2 - py3) + px2 * (py3 - py1) + px3 * (py1 - py2)) / 2);
	   }
    public:
	   bool draw_tr_condition() const { return (keypoint && visible);}
	   void set_keypoint(int p) {keypoint = p;}
	   void set_visibility(bool  v) {visible = v;}

	   bool CheckingDotMatch(double tx, double ty) const {
		   if(draw_tr_condition()) {
		        double Tr = calctr(keypoint - (draw::xoffset / 2 + (draw::dofset)), draw::dofset, keypoint - draw::dofset, draw::dofset, keypoint - draw::dofset, (draw::xoffset / 2 + (draw::dofset)));
		        double CheckTr = calctr(tx, ty, keypoint - draw::dofset ,draw::dofset, keypoint - draw::dofset ,(draw::xoffset / 2 + (draw::dofset))) +
				    calctr(keypoint - (draw::xoffset / 2 + (draw::dofset)) ,draw::dofset, tx, ty, keypoint - draw::dofset ,(draw::xoffset / 2 + (draw::dofset))) +
                   calctr(keypoint - (draw::xoffset / 2 + (draw::dofset)) ,draw::dofset, keypoint - draw::dofset ,draw::dofset, tx, ty);

		        return (Tr == CheckTr);
		    }
		    else
			    return false;

	   }

	   void draw_triangle(const Cairo::RefPtr<Cairo::Context>& cr) const {
		   if(draw_tr_condition()) {
		          cr->move_to(keypoint - (draw::xoffset / 2 + (draw::dofset)) ,draw::dofset);
		          cr->line_to(keypoint - draw::dofset ,draw::dofset);
		          cr->line_to(keypoint - draw::dofset ,(draw::xoffset / 2 + (draw::dofset)));
		          cr->close_path();
                  cr->fill();
		   }
	   }
  } triangle; // utility

  Glib::RefPtr<Gtk::GestureClick> msbntpress;

  CDrArTempr() = default;
  CDrArTempr(UIHWindow* uhiwnd,fp_lDASR ldafp);
  virtual ~CDrArTempr() = default;

  void SetAttentState(bool show) {show_msg_attention = show;}
  bool GetAttentState() const {return  show_msg_attention;}
  void SetUnsetDrawItem(const DRAWVECTORPLUS*const item, double *max, Glib::ustring SensorName, Glib::ustring SensorID, bool setflag);
  void EraseAll() {draw_temperatures.clear();}
  const bool HasActivities() const {return !draw_temperatures.empty();}
  std::string CheckingDotMatch(double x, double y) const;
private:
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
