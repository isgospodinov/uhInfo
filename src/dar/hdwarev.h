/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _HDWAREV_H_
#define _HDWAREV_H_

#include "hdwaret.h"

class CDrArVcore : public CDrArTempr
{
public:
	CDrArVcore(const VCORESBUNCH *const pVC) : pVcores(pVC){}
	virtual ~CDrArVcore() = default;

	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
	void ClearOrActivateVCStatistic(bool active = false);
	const bool VCoresActivities() const;
private:
	const VCORESBUNCH *const pVcores;
};



#endif /* _HDWAREV_H_ */
