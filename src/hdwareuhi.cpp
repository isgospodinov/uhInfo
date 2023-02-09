/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "hdwareuhi.h"
#include "uhirc.h"

CDrArUhi::CDrArUhi() : m_TmpWndCurrState(DAWndState::NORMAL)
{
	set_size_request(draw::dwaw,draw::dwah);
    set_draw_func(sigc::mem_fun(*this, &CDrArUhi::on_draw_area));
    set_expand();
}

void CDrArUhi::DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,DMode dm,bool X) const
{
	if(dm == DMode::TEMPERATUREDRAW && !X) {
	   double cw = (dwidth - (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0)) / 2;

       for(int b = 1; b < (int) draw::uhi_draw_xscale ; b *= 2) {
           double cl =  cw; // centering
           int c = 0;

           cl -= ((cw / (double)b) * (b - 1)); // offset

		   while(c++ < b) {
		       crtx->move_to(cl + (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0),0);
			   crtx->line_to(cl + (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0),dheight);
			   cl += ((cw / ((double)b / 2)));
		   }
       }
	}
	else {
       int cnt = ((dm == DMode::CPUDRAW  && !X) ? (double) dwidth / (double) draw::uhi_draw_xscale : (double) dheight / (double) draw::uhi_draw_yscale),up = cnt;
       while(cnt <= (X ? dheight : dwidth)) {
           crtx->move_to((X ? (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset - draw::dofset :  0) : cnt),(X ? cnt : 0));
           crtx->line_to((X ? dwidth : cnt),(X ? cnt : dheight));
           cnt += up;
       }
	}
   crtx->stroke();
}
