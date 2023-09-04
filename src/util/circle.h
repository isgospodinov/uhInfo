/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "util.h"

struct Point {
	int cx = 0, cy = 0;
    bool dr = false;

    bool CheckingDotMatch(double x, double y, const int radius = uhiutil::draw::bp_radius) {
    	bool cr = (std::pow((cx - x),2) + std::pow((cy - y),2) <= std::pow(radius,2));

    	if(radius == uhiutil::draw::bp_radius)
    		                    dr = cr;

        return cr;
    }
};

#endif /* _CIRCLE_H_ */
