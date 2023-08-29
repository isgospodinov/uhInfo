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

struct Point {
	int cx = 0, cy = 0;
    bool dr = false;

    bool CheckingDotMatch(double x, double y, const int radius = draw::bp_radius) {
    	bool cr = (std::pow((cx - x),2) + std::pow((cy - y),2) <= std::pow(radius,2));

    	if(radius == draw::bp_radius)
    		                    dr = cr;

        return cr;
    }
};

struct StresTestSession{
	  int cn_startoffset = 0, cn_endoffset = 0;

	  void drawing_request(const Cairo::RefPtr<Cairo::Context>& cr, double xc, int h) const {
	      cr->save();
	      cr->begin_new_sub_path();
	      cr->set_source_rgb(1.0, 1.0, 1.0);

		  if(cn_startoffset) {
			  cr->unset_dash();
			  cr->set_dash(std::vector<double>{6}, 1);
	          cr->move_to((xc * (cn_startoffset + (cn_endoffset ? cn_endoffset : 0))) + draw::xoffset, draw::xoffset);
	          cr->line_to((xc * (cn_startoffset + (cn_endoffset ? cn_endoffset : 0))) + draw::xoffset, h - draw::xoffset);
	          cr->stroke();
		  }

		  if(cn_endoffset) {
			  cr->unset_dash();
			  cr->set_dash(std::vector<double>{18}, 1);
	          cr->move_to((xc * cn_endoffset) + draw::xoffset, draw::xoffset);
	          cr->line_to((xc * cn_endoffset) + draw::xoffset, h - draw::xoffset);
	          cr->stroke();
		  }

		  cr->restore();
	  }
};

struct ExtdPoint : Point
{
    bool StartStresTest(StresTestSession &sts,double &x, double &y) {
    	if(CheckingDotMatch(x, y, draw::tp_radius)) {
            if(!dr) {
	            for(int i = std::atoi((uhiutil::execmd("nproc")).c_str()) ; i > 0 ; i--) {
                    pIDs.push_back(std::atoi((uhiutil::execmd("while : ; do : ; done > /dev/null & echo $!")).c_str()));
	            }

	            dr = true;

	            sts.cn_startoffset  = 0;
	            sts.cn_endoffset = 0;
            }
            else {
            	StopStresTest(); //uhiutil::execmd("killall sh");
            }

    		return true;
    	}
    	else {
    		return false;
    	}
    }

    void StopStresTest() {
       if(!pIDs.empty()) {
    		for(std::list<int>::iterator it = pIDs.begin(); it != pIDs.end();) {
                uhiutil::execmd(("kill " + std::to_string(*it)).c_str());
                it = pIDs.erase(it);
    		}

           if(dr)
               dr = false;
       }
    }

    void drawing_request(const Cairo::RefPtr<Cairo::Context>& cr) const {
	       cr->save();
     	   cr->begin_new_sub_path();
		   cr->arc(cx , cy, draw::tp_radius, 0, 2 * M_PI);

	       if(!dr)cr->fill();
	       else {
	        	  cr->set_line_width(.5);
	        	  cr->stroke();
	       }
	       cr->arc(cx , cy, draw::tp_radius + 2, 0, 2 * M_PI);
	       cr->restore();
    }

    const bool Get_StresSessionState() const {return dr;}

    std::list<int> pIDs;
};

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
  StresTestSession mark_stres_session; // Stres test session data

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

	   void drawing_request(const Cairo::RefPtr<Cairo::Context>& cr) const {
		   if(draw_tr_condition()) {
			      cr->save();
		          cr->move_to(keypoint - (draw::xoffset / 2 + (draw::dofset)) ,draw::dofset);
		          cr->line_to(keypoint - draw::dofset ,draw::dofset);
		          cr->line_to(keypoint - draw::dofset ,(draw::xoffset / 2 + (draw::dofset)));
		          cr->close_path();
                  cr->fill();
                  cr->restore();
		   }
	   }
  } triangle; // utility

  Glib::RefPtr<Gtk::GestureClick> msbntpress;

  CDrArTempr() = default;
  CDrArTempr(UIHWindow* uhiwnd,fp_lDASR ldafp);
  virtual ~CDrArTempr() {EraseAll();}

  void SetAttentState(bool show) {show_msg_attention = show;}
  bool GetAttentState() const {return  show_msg_attention;}
  void SetUnsetDrawItem(const DRAWVECTORPLUS*const item, double *max, Glib::ustring SensorName, Glib::ustring SensorID, bool setflag);
  void EraseAll() {draw_temperatures.clear();tpoint.StopStresTest();mark_stres_session = {0, 0};}
  const bool HasActivities() const {return !draw_temperatures.empty();}
  std::string CheckingDotMatch(double x, double y) const;
  const bool Get_StresSessionState() const {return tpoint.Get_StresSessionState();}
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
