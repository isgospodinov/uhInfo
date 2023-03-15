/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef HDWARETC_
#define HDWARETC_

#include "../uhirc.h"
#include "../procun.h"

class CDrArCpuInTempr : public CDrArCpu
{
public:
	using CpuDaMode = enum class DaCpuInfoMode {NORMAL,EXTENDED};

	CDrArCpuInTempr(CProcUnits::PFQAVG pFA, const double *const mx,CpuDaMode cdm = CpuDaMode::NORMAL) : CDrArCpu(&pFA->FqAvg,&pFA->UsgAvg),cpuFqMax(mx),pFqAv(pFA),lcdm(cdm){}
	virtual ~CDrArCpuInTempr() = default;
private:
	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
	const double *const cpuFqMax = nullptr;

	CProcUnits::PFQAVG pFqAv = nullptr;
	CpuDaMode lcdm;

};

#endif // HDWARETC_
