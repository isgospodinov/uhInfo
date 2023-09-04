/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "util.h"

struct Triangle{
    private:
	   int keypoint = 0;
	   bool visible = false;

	   const double calctr(int px1, int py1, int px2, int py2, int px3, int py3) const {
	      return std::abs((px1 * (py2 - py3) + px2 * (py3 - py1) + px3 * (py1 - py2)) / 2);
	   }
    public:
	   bool draw_tr_condition() const { return (keypoint && visible);}
	   void set_keypoint(int p) {keypoint = p;}
	   void set_visibility(bool  v) {visible = v;}

	   bool CheckingDotMatch(double tx, double ty) const {
		   if(draw_tr_condition()) {
		        double Tr = calctr(keypoint - (uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset)), uhiutil::draw::dofset, keypoint - uhiutil::draw::dofset, uhiutil::draw::dofset, keypoint - uhiutil::draw::dofset, (uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset)));
		        double CheckTr = calctr(tx, ty, keypoint - uhiutil::draw::dofset ,uhiutil::draw::dofset, keypoint - uhiutil::draw::dofset ,(uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset))) +
				    calctr(keypoint - (uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset)) ,uhiutil::draw::dofset, tx, ty, keypoint - uhiutil::draw::dofset ,(uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset))) +
                   calctr(keypoint - (uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset)) ,uhiutil::draw::dofset, keypoint - uhiutil::draw::dofset ,uhiutil::draw::dofset, tx, ty);

		        return (Tr == CheckTr);
		    }
		    else
			    return false;
	   }

	   void drawing_request(const Cairo::RefPtr<Cairo::Context>& cr) const {
		   if(draw_tr_condition()) {
			      cr->save();
		          cr->move_to(keypoint - (uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset)) ,uhiutil::draw::dofset);
		          cr->line_to(keypoint - uhiutil::draw::dofset ,uhiutil::draw::dofset);
		          cr->line_to(keypoint - uhiutil::draw::dofset ,(uhiutil::draw::xoffset / 2 + (uhiutil::draw::dofset)));
		          cr->close_path();
                  cr->fill();
                  cr->restore();
		   }
	   }
}; // utility button

#endif /* _TRIANGLE_H_ */
