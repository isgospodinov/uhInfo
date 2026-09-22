/*
 *    uhInfo
 *    Copyright (C) 2025
 */

#include "expnr.h"
#include "../mwnd.h"

UIHWindow *uhiExpand::p_mWnd = nullptr;
constexpr int TM_DELAY = 250;

void uhiExpand::on_expr_state_changed(const bool state, const Expanders ind)
{
    if(!p_mWnd) return;

    switch(ind)
    {
          case Expanders::MOBO ... Expanders::NETW  :
          case Expanders::MAIN : {
							 int curr_disks_h = p_mWnd->m_Frame_Disks.get_allocated_height();
							 p_mWnd->m_Frame_Disks.set_size_request(-1, curr_disks_h);

							 if(ind == Expanders::MAIN) {
								  set_label((state ? _(" Detailed - summary - view") : _(" Simplified - summary - view")) );
								  INIT_EXPANDERS(p_mWnd,state);
							 }
							 else
								 if(!state) p_mWnd->set_default_size(p_mWnd->get_width(), p_mWnd->get_height()/2);

							 p_mWnd->m_Frame_Disks.queue_resize();

							 /* Glib::signal_idle().connect_once([w = p_mWnd]() {
							                                      if (w) w->m_Frame_Disks.set_size_request(-1, -1);
							                                  }); */

							 Glib::signal_timeout().connect_once([w = p_mWnd]() {
										if (w) w->m_Frame_Disks.set_size_request(-1, -1); }, TM_DELAY);

                             break;
          }
          case Expanders::OS  : {
                             if(p_mWnd) p_mWnd->m_Frame_User.set_visible(state);
                             break;
          }
          default: break;
    }

    if(ind != Expanders::MAIN && p_mWnd) {

    	  bool vstate = p_mWnd->v_expr.get_expanded();

    	  Action laction = on_auto_adjust(p_mWnd->mb_expr.get_expanded(),p_mWnd->os_expr.get_expanded(),p_mWnd->mm_expr.get_expanded(),p_mWnd->mn_expr.get_expanded(),
    			                                            p_mWnd->au_expr.get_expanded(),p_mWnd->nt_expr.get_expanded(), vstate);

          if(laction == Action::EXPAND && !vstate) p_mWnd->v_expr.set_expanded();
          else
    	      if(laction == Action::SHRINK && vstate) p_mWnd->v_expr.set_expanded(false);
      }
}

const uhiExpand::Action uhiExpand::on_auto_adjust(const bool mbe, const bool ose, const bool mme, const bool mne, const bool aue, const bool nte, const bool ve) const
{
	 if(!ve && mbe && ose && mme && mne && aue && nte) {
		 return Action::EXPAND;
     }
	 else
         if(ve && !mbe && !ose && !mme && !mne && !aue && !nte) {
        	 return Action::SHRINK;
         }

	 return Action::NOACTION;
}
