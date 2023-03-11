/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "hdwarev.h"
#include "../sysens.h"

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

	   for(Sensor_node* snc : *pVcores)  {
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
	for(Sensor_node* snc : *pVcores) {

		if(active && snc->visible) {
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
	for(Sensor_node* snc : *pVcores) {
		 if(snc->visible)
					return true;
	}

	return false;
}
