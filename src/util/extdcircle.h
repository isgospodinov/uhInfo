/*
 *    uhInfo
 *    Stress Test – Elimination of external Shell dependencies - 22-09-2026
 *    Copyright (C) 2023
 */

#ifndef _EXTDCIRCLE_H_
#define _EXTDCIRCLE_H_

#include "circle.h"
#include <thread>
#include <vector>
#include <cmath>

struct ExtdPoint : Point
{
    bool StartStresTest(std::list<StresTestSession> &sts, std::string st, double &x, double &y) {
    	if(CheckingDotMatch(x, y, uhiutil::draw::tp_radius)) {
            if(!dr) {
                dr = true;
                sts.push_back({st, "", 0, 0, (sts.empty() ? 1 : sts.back().sID + 1)});

                //Get the number of native hardware threads using C++
                unsigned int num_cores = std::thread::hardware_concurrency();
                if(num_cores == 0){
                	sts.pop_back();
                    StopStresTest();
                    return false;
                }

                // Launch native C++20 jthreads that heavily load the CPU cores
                for(unsigned int i = 0; i < num_cores; ++i) {
                    m_threads.push_back(std::jthread([](std::stop_token stop_token) {
                        volatile double d = 1234.56;
                        // Execute heavy trigonometry until a stop signal is requested
                        while(!stop_token.stop_requested()) {
                            d = std::sin(d) * std::cos(d);
                        }
                    }));
                }
            }
            else {
            	sts.back().stoptime = st;
            	StopStresTest();
            }

    		return true;
    	}
    	else {
    		return false;
    	}
    }

    void StopStresTest() {
        //Clearing the std::jthread vector automatically signals
        //a stop request and joins the threads
        m_threads.clear();
        
        if(dr)
            dr = false;
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

    //vector native C++20 threads
    std::vector<std::jthread> m_threads;
};

#endif /* _EXTDCIRCLE_H_ */
