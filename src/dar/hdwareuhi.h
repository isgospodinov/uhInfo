/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _DWERAUHI_H_
#define _DWERAUHI_H_

#include "../util.h"
#include <cairomm/context.h>

class CDrArUhi : public Gtk::DrawingArea
{
public:
	CDrArUhi();
	virtual ~CDrArUhi() = default;

	void Redraw() {queue_draw();}
protected:
	virtual void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X = false) const = 0;
	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width,int height) = 0;
	const Pango::FontDescription DA_DrawFont(bool fd = true)  const { Pango::FontDescription font; font.set_family(uhiutil::draw::text_font_family);
			font.set_weight(fd ? Pango::Weight::BOLD : Pango::Weight::THIN);
			font.set_style(fd ? Pango::Style::ITALIC : Pango::Style::NORMAL);
			font.set_size((fd ? uhiutil::draw::dtxtmax : uhiutil::draw::dtxthin) * PANGO_SCALE);
			return font;
	  }
};

#endif // _DWERAUHI_H_
