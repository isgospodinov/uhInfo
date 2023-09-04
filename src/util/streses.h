/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _STRESES_H_
#define _STRESES_H_

#include "util.h"

struct StresTestSession{
	  int cn_startoffset = 0, cn_endoffset = 0;
	  unsigned int sID = 0;

	  const bool operator == (const StresTestSession s) const {return (cn_startoffset == s.cn_startoffset &&  cn_endoffset == s.cn_endoffset && sID == s.sID);}

	  void drawing_request(const Cairo::RefPtr<Cairo::Context>& cr, double xc, int h) const {
	      cr->save();
	      cr->begin_new_sub_path();
	      cr->set_source_rgb(1.0, 1.0, 1.0);
	      double xx = (xc * (cn_startoffset + (cn_endoffset ? cn_endoffset : 0))) + uhiutil::draw::xoffset;

		  if(cn_startoffset) {
			  cr->unset_dash();
			  cr->set_dash(std::vector<double>{6}, 1);
			  drawline(cr, xx, h,true);
		  }

		  if(cn_endoffset) {
			  cr->unset_dash();
			  cr->set_dash(std::vector<double>{18,4}, 1);
			  xx = (xc * cn_endoffset) + uhiutil::draw::xoffset;
			  drawline(cr, xx, h,false);
		  }

		  cr->restore();
	  }
private:
	  static constexpr unsigned int in{uhiutil::draw::xoffset / 5};

	  void drawcap(const Cairo::RefPtr<Cairo::Context>& cr, double x, bool f) const {
          cr->move_to(x, uhiutil::draw::xoffset  + (f ? in : 0));
          cr->line_to(x - in, uhiutil::draw::xoffset + (f ? 0 : in));
          cr->line_to(x + in, uhiutil::draw::xoffset + (f ? 0 : in));
          cr->close_path();
          cr->fill();
	  }

	  void drawline(const Cairo::RefPtr<Cairo::Context>& cr, double x, int h, bool f) const {
          cr->move_to(x, uhiutil::draw::xoffset);
          cr->line_to(x, h - uhiutil::draw::xoffset);
          cr->stroke();
          drawcap(cr, x, f);
	  }
};

#endif /* _STRESES_H_ */
