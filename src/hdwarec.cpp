/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "mwnd.h"
#include "proc.h"
#include <cairomm/context.h>
#include "hdwarec.h"

const Gtk::Switch *CDrArCpu::l_CPUModeSwitch = nullptr,*CDrArCpu::l_CPUCompareSwitch = nullptr;

CDrArCpu::CDrArCpu(const TUDRAWVECTOR *dw_frec,const TUDRAWVECTOR *dw_frec_cp,const TUDRAWVECTOR *dw_usg)
{
  valfreq = dw_frec;
  valusg = dw_usg;
  valfreqcmpr = dw_frec_cp;
}

void CDrArCpu::DrawActivity(const Cairo::RefPtr<Cairo::Context>& crtx,double atvy,int dheight,int dwidth,StatPaint pm) const
{
  crtx->move_to(0, (pm == StatPaint::USAGE ? (*valusg)[0] :  ( pm == StatPaint::FREQP ?  (*valfreq)[0] : (*valfreqcmpr)[0])) * dheight);
  crtx->arc(0, (pm == StatPaint::USAGE ? (*valusg)[0] : (pm == StatPaint::FREQP ? (*valfreq)[0] : (*valfreqcmpr)[0])) * dheight, 1.1, 0, 2 * M_PI);
  for(long unsigned int br = 1; br < uhiutil::calc::draw_cpu_statistic ;br++) {
       crtx->arc(atvy * br, (pm == StatPaint::USAGE ? (*valusg)[br] : (pm == StatPaint::FREQP ? (*valfreq)[br] : (*valfreqcmpr)[br])) * dheight, 1.1, 0, 2 * M_PI);
  }
  crtx->stroke();
}

void CDrArCpu::on_draw_area(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
  cr->save();
  cr->set_source_rgb(1.0, 1.0, 1.0);

  Cairo::Matrix matrix(-1.0 , 0.0, 0.0, -1.0, width, height); // axes flipping
  cr->transform(matrix);// (0,0) -> (width,height):(width,0) -> (0,height):(0,height) -> (width,0):...

  double xc = (double)width / ((double)uhiutil::calc::draw_cpu_statistic - 1.0);

  cr->set_line_width(1.0);
  cr->set_dash(std::vector<double>{1.0}, 1);

  DrawAxis_XY(cr,width,height,DMode::CPUDRAW);                     // X axis
  DrawAxis_XY(cr,width,height,DMode::CPUDRAW,true); // Y axis

  cr->unset_dash();

      bool l_cmsactive = l_CPUModeSwitch->get_active(),l_csactive = l_CPUCompareSwitch->get_active();
      cr->set_source_rgb(0.0, 0.75, 1.0);
      cr->set_line_width(1.7);
      DrawActivity(cr,xc,height,0,((l_cmsactive && CProc::m_CpuAltCalc) ? StatPaint::COMPAREFREQP : StatPaint::FREQP));

      cr->set_source_rgb(0.40, 0.80, 0.67);
      cr->set_line_width(1.4);
      DrawActivity(cr,xc,height,0,StatPaint::USAGE);

      if(CProc::m_CpuAltCalc && l_csactive) {
          cr->set_source_rgb(0.90, 0.80, 0.40);
          cr->set_line_width(1.2);
          DrawActivity(cr,xc,height,0,(l_cmsactive ? StatPaint::FREQP : StatPaint::COMPAREFREQP));
      }

  cr->restore();
}
