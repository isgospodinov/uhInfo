/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _DWERAC_H_
#define _DWERAC_H_

#include "hdwareuhi.h"

class CDrArCpu : public CDrArUhi
{
public:
	using StatPaint = enum class PaintMode {FREQP,COMPAREFREQP,USAGE};
	using TUDRAWVECTOR = std::array<double, uhiutil::calc::draw_cpu_statistic>;

	CDrArCpu(const TUDRAWVECTOR *dw_frec = nullptr,const TUDRAWVECTOR *dw_frec_cp = nullptr,const TUDRAWVECTOR *dw_usg = nullptr);
	virtual ~CDrArCpu() = default;

	static const Gtk::Switch *l_CPUModeSwitch,*l_CPUCompareSwitch;
private:
	const TUDRAWVECTOR *valfreq = nullptr, *valfreqcmpr = nullptr, *valusg = nullptr;
	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
	virtual void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X = false) const override;
	void DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth,StatPaint pm) const;
};

#endif // _DWERAC_H_
