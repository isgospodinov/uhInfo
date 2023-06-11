/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "hdwarev.h"
#include "../sysens.h"

CDrArVcore::SENSBUNCH CDrArVcore::dwVCF;

void CDrArVcore::DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X) const
{
	if(!X) {
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
       int cnt = (double) dheight / (double) draw::uhi_draw_yscale,up = cnt;
       while(cnt <= (X ? dheight : dwidth)) {
           crtx->move_to((X ? (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset - draw::dofset :  0) : cnt),(X ? cnt : 0));
           crtx->line_to((X ? dwidth : cnt),(X ? cnt : dheight));
           cnt += up;
       }
	}

    crtx->stroke();
}

void CDrArVcore::DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth) const
{
  if((tmpmon ? (*tmpmon).size() : 0) < 2) return;

  crtx->move_to(FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0, ((tmpmon ? (*tmpmon)[0] : 0)) * dheight);
  for(long unsigned int br = 1; br < (tmpmon ? (((*tmpmon).size() < uhiutil::calc::t_statistic_len) ? (*tmpmon).size() : uhiutil::calc::t_statistic_len) : 0);br++) {
    	   crtx->line_to(atvy * br + (FULLAPPWNDMODE(dwidth,dheight) ? draw::xoffset : 0),
    	               		               ((tmpmon ? (*tmpmon)[br] : 0)) * ((*tmpmon)[br] * dheight >= uhiutil::cpu::max_cpu_t ? dheight - 1 : dheight));
  }
  crtx->stroke();
}

void CDrArVcore::on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
	   cr->save();
	   cr->set_source_rgb(1.0, 1.0, 1.0);

	   Cairo::Matrix matrix(-1.0 , 0.0, 0.0, -1.0, width, height); // axes flipping
	   cr->transform(matrix);// (0,0) -> (width,height):(width,0) -> (0,height):(0,height) -> (width,0):...

	   cr->set_line_width(1.0);
	   cr->set_dash(std::vector<double>{1.0}, 1);

	   DrawAxis_XY(cr,width,height);      // X axis
	   DrawAxis_XY(cr,width,height,true); // Y axis

	   cr->unset_dash();

	   int dw = 0,dh = 0,br = 0;
	   const double scale = uhiutil::calc::draw_Vcore_scaler / (double) uhiutil::draw::uhi_draw_yscale;
	   Glib::RefPtr<Pango::Layout> layout = create_pango_layout("");
	   layout->set_font_description(DA_DrawFont(false));

	   for(Sensor_node* snc : dwVCF) {
		          tmpmon = &snc->t_statistic;
		          Gdk::Cairo::set_source_rgba(cr,Gdk::RGBA(snc->statistic_color));
	              DrawActivity(cr,(double)width / ((double)uhiutil::calc::draw_cpu_statistic - 1.0),height,width);

	              if(draw_sensor_name && snc->t_statistic.size() > 1) {
	                  cr->save();
	                  matrix.invert();
	                  cr->transform(matrix);
	                  cr->set_source_rgb(1.0, 1.0, 1.0);
	                  DA_Text(layout, dw , dh,snc->label);
	                  DADRAWTEXT(cr, layout,width - (dw + (uhiutil::draw::dofset / 2)), (height - ((snc->t_statistic[0] * height) + (dh + (uhiutil::draw::dofset / 2)))));
	                  cr->restore();
	              }
	   }

	   cr->restore();

	   cr->save();
	   cr->set_source_rgb(1.0, 1.0, 1.0);

	   DA_Text(layout, dw , dh,std::to_string(scale * (uhiutil::draw::uhi_draw_yscale - br)).substr(0,3) + " V");
	   DADRAWTEXT(cr,layout,uhiutil::draw::dofset / 2,height - (height - (uhiutil::draw::dofset / 2)));
	   br++;

	   int cnt = (double) height / (double) uhiutil::draw::uhi_draw_yscale,up = cnt;
	   while(cnt <= height) {
	        DA_Text(layout, dw , dh,std::to_string(scale * (uhiutil::draw::uhi_draw_yscale - br)).substr(0,br == 5 ? 1 : 3) + ((uhiutil::draw::uhi_draw_yscale - br) ? " V" : ""));
	        DADRAWTEXT(cr,layout,uhiutil::draw::dofset / 2,cnt - dh);
	        cnt += up;
	        br++;
	   }

	   cr->restore();
}

void CDrArVcore::ClearOrActivateVCStatistic(bool active)
{
	int cv = 0;
	for(Sensor_node* snc : dwVCF) {

		if(active && snc->visible && snc->is_Vcore) {
			snc->SetStatisticFeatures(true,"Red");
			cv++;
		}
		else
			snc->EraseStatistic();
	}

	draw_sensor_name = (active && (cv > 1));
}

const bool CDrArVcore::VCoresActivities() const
{
	for(Sensor_node* snc : dwVCF) {
		 if(snc->is_Vcore && snc->visible)
					return true;
	}

	return false;
}
