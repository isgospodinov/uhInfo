/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _EXTDCIRCLE_H_
#define _EXTDCIRCLE_H_

#include "circle.h"

struct ExtdPoint : Point
{
    bool StartStresTest(std::list<StresTestSession> &sts, double &x, double &y) {
    	if(CheckingDotMatch(x, y, uhiutil::draw::tp_radius)) {
            if(!dr) {
	            for(int i = std::atoi((uhiutil::execmd("nproc")).c_str()) ; i > 0 ; i--) {
                    pIDs.push_back(std::atoi((uhiutil::execmd("while : ; do : ; done > /dev/null & echo $!")).c_str()));
	            }

	            dr = true;

	            sts.push_back({0, 0, (sts.empty() ? 1 : sts.back().sID + 1)});
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
		   cr->arc(cx , cy, uhiutil::draw::tp_radius, 0, 2 * M_PI);

	       if(!dr)cr->fill();
	       else {
	        	  cr->set_line_width(.5);
	        	  cr->stroke();
	       }
	       cr->arc(cx , cy, uhiutil::draw::tp_radius + 2, 0, 2 * M_PI);
	       cr->restore();
    }

    const bool Get_StresSessionState() const {return dr;}

    std::list<int> pIDs;
};

#endif /* _EXTDCIRCLE_H_ */
