/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef HDWARETC_
#define HDWARETC_

#include "uhirc.h"

class CDrArCpuInTempr : public CDrArCpu
{
public:
	CDrArCpuInTempr(const CDrArCpu::TUDRAWVECTOR *const cpu_fda,const CDrArCpu::TUDRAWVECTOR *const cpu_usg,const double *const mx) : CDrArCpu(&cpu_fda,cpu_usg),cpuFqMax(mx){}
	virtual ~CDrArCpuInTempr() = default;
private:
	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
	const double *const cpuFqMax = 0;
};

#endif // HDWARETC_
