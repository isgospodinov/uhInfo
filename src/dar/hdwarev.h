/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#ifndef _HDWAREV_H_
#define _HDWAREV_H_

#include "hdwareuhi.h"

using Sensor_node = struct _Sensor_node;

class CDrArVcore : public CDrArUhi
{
public:
    CDrArVcore() = default;
	virtual ~CDrArVcore() = default;

    using TmpWndState = enum class DAWndState{NORMAL,FULL};
	using VCORESBUNCH = std::list<Sensor_node*>;
	const bool VCoresActivities();
	VCORESBUNCH *const GetDAVcoreAccess() {return &draw_Vcores;}
	void ClearOrActivateVCStatistic(bool active = false);

    TmpWndState m_TmpWndCurrState = DAWndState::NORMAL;
protected:
    using DRAWVECTOR = const std::vector<double>*;

	virtual void on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) override;
	const bool HasVCores() const {return !draw_Vcores.empty();}
private:
	bool draw_sensor_name = false;
protected:
	static VCORESBUNCH draw_Vcores;
	DRAWVECTOR tmpmon = nullptr;

	virtual void DrawAxis_XY(const Cairo::RefPtr<Cairo::Context>& crtx,int dwidth,int dheight,bool X = false) const override;
	void DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth = 0) const;
	void DA_Text(Glib::RefPtr<Pango::Layout>& ly,int& dw,int& dh, std::string dt) const {ly->set_text(dt);ly->get_pixel_size(dw,dh);}
};



#endif /* _HDWAREV_H_ */
