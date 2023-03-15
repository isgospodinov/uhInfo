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
	using StatPaint = enum class PaintMode {TEMPRCPUSTAT,TEMPRCPUSGSTAT,FREQP,COMPAREFREQP,USAGE};
	using TUDRAWVECTOR = std::array<double, uhiutil::calc::draw_cpu_statistic>;

	CDrArCpu(const TUDRAWVECTOR *dw_frec, const TUDRAWVECTOR *dw_frec_cp, const TUDRAWVECTOR *dw_usg);
	CDrArCpu(const TUDRAWVECTOR *cpuFq,const TUDRAWVECTOR *cpuUsg) : cpuFqSum(cpuFq),cpuUsgSum(cpuUsg) {}
	virtual ~CDrArCpu() = default;

	static const Gtk::Switch *l_CPUModeSwitch,*l_CPUCompareSwitch;
private:
	const TUDRAWVECTOR *valfreq = nullptr, *valfreqcmpr = nullptr, *valusg = nullptr;
	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
protected:
	const TUDRAWVECTOR *const cpuFqSum = nullptr,*const cpuUsgSum = nullptr;
	void DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth,StatPaint pm) const;
	virtual void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X = false) const override;
	double on_draw_prep(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

#endif // _DWERAC_H_