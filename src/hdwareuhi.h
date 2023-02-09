/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _DWERAUHI_H_
#define _DWERAUHI_H_

#include "util.h"
#include <cairomm/context.h>

class CDrArUhi : public Gtk::DrawingArea
{
public:
	CDrArUhi();
	virtual ~CDrArUhi() = default;

	using TmpWndState = enum class DAWndState {NORMAL,FULL};
	using DMode = enum class DrawMode {CPUDRAW,TEMPERATUREDRAW};

	TmpWndState m_TmpWndCurrState;

	void Redraw() {queue_draw();}
protected:
	void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,DMode dm,bool X = false) const;
	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width,int height) = 0;
};

#endif // _DWERAUHI_H_
