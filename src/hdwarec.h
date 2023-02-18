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

	CDrArCpu(const TUDRAWVECTOR *dw_frec = nullptr,const TUDRAWVECTOR *dw_frec_cp = nullptr,const TUDRAWVECTOR *dw_usg = nullptr);
	CDrArCpu(const TUDRAWVECTOR *const*const cpuFq,const TUDRAWVECTOR *const cpuUsg) : cpuFqSum(*cpuFq),cpuUsgSum(cpuUsg) {}
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

class CDrArCpuInTempr : public CDrArCpu
{
public:
	CDrArCpuInTempr(const CDrArCpu::TUDRAWVECTOR *const cpu_fda,const CDrArCpu::TUDRAWVECTOR *const cpu_usg,const double *const mx) : CDrArCpu(&cpu_fda,cpu_usg),cpuFqMax(mx){}
	virtual ~CDrArCpuInTempr() = default;
private:
	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
	const double *const cpuFqMax = 0;
};

#endif // _DWERAC_H_
