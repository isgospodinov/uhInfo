/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "hdwaretc.h"
#include <glibmm/i18n.h>

void CDrArCpuInTempr::on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
  cr->save();
  double xc = on_draw_prep(cr,width,height);
  const int scale = *cpuFqMax / uhiutil::draw::uhi_draw_yscale;
  int dw = 0,dh = 0,br = 0;

  for(int pm = (int)StatPaint::TEMPRCPUSTAT;pm < (int)StatPaint::FREQP ; pm++) {
	  cr->set_source_rgb(pm == (int)StatPaint::TEMPRCPUSTAT ? 1.0 : 0.40,pm == (int)StatPaint::TEMPRCPUSTAT ? 0.15 : 0.80,pm == (int)StatPaint::TEMPRCPUSTAT ? 0.25 : 0.67);
	  cr->set_line_width(pm == (int)StatPaint::TEMPRCPUSTAT ? 1.7 : 1.4);
	  DrawActivity(cr,xc,height,0,(StatPaint) pm);
  }
  cr->restore();

  cr->save();
  cr->set_source_rgb(1.0, 1.0, 1.0);
  Glib::RefPtr<Pango::Layout> layout = create_pango_layout("");
  layout->set_font_description(DA_DrawFont(false));

  layout->set_text(std::to_string(scale * (uhiutil::draw::uhi_draw_yscale - br)) + " Mhz");
  layout->get_pixel_size(dw,dh);
  DADRAWTEXT(cr,layout,uhiutil::draw::dofset / 2,height - (height - (uhiutil::draw::dofset / 2)));
  br++;

  int cnt = (double) height / (double) uhiutil::draw::uhi_draw_yscale,up = cnt;
  while(cnt <= height) {
      layout->set_text(std::to_string(scale * (uhiutil::draw::uhi_draw_yscale - br)) + ((uhiutil::draw::uhi_draw_yscale - br) ? " Mhz" : ""));;
      DADRAWTEXT(cr,layout,uhiutil::draw::dofset / 2,cnt - dh);
      cnt += up;
      br++;
  }

  if(lcdm == CpuDaMode::EXTENDED) {
	  layout->set_text(_("Overall CPU activity"));
	  layout->get_pixel_size(dw,dh);
	  DADRAWTEXT(cr,layout,width / 2 - (dw / 2),dh  + uhiutil::draw::dofset);

	  layout->unset_font_description();
	  layout->set_font_description(DA_DrawFont());

	  layout->set_text(*pFqAv->cpun);
	  layout->get_pixel_size(dw,dh);
	  DADRAWTEXT(cr,layout,width / 2 - (dw / 2),dh  + (uhiutil::draw::dofset * 2));

	  layout->set_text(std::to_string(*pFqAv->cu) + _(" active CPU units"));
	  layout->get_pixel_size(dw,dh);
	  DADRAWTEXT(cr,layout,width / 2 - (dw / 2),dh * 2 + uhiutil::draw::dofset);

	  layout->set_text(_("Last report : ") + std::to_string((int)(pFqAv->FqAvg[0] * (*cpuFqMax))) + " MHz  "
			  + std::to_string((int)(pFqAv->UsgAvg[0] * 100)) + "% " + _("usage"));
	  layout->get_pixel_size(dw,dh);
	  DADRAWTEXT(cr,layout,width / 2 - (dw / 2),dh * 4 + uhiutil::draw::dofset);
  }

  layout->unset_font_description();

  cr->restore();
}




