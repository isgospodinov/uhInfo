/*
 *    uhInfo
 *    Copyright (C) 2025
 */

#include "expnr.h"
#include "../mwnd.h"

UIHWindow *uhiExpand::p_mWnd = nullptr;

void uhiExpand::on_expr_state_changed(bool state, Expanders& ind)
{

    switch(ind)
    {
      case Expanders::MOBO ... Expanders::NETW  : {
        	                 if(!state) {
        	                      if(p_mWnd) p_mWnd->set_default_size(p_mWnd->get_width(),p_mWnd->get_height()/2); // height autoadjust
                             }
                             break;
         }
         case Expanders::OS  : {
        		             if(p_mWnd) p_mWnd->m_Frame_User.set_visible(state);
        	                 break;

         }
         case Expanders::MAIN : {
        	                 set_label((state ? _(" Detailed - summary - view") : _(" Simplified - summary - view")) );
        	                 INIT_EXPANDERS(p_mWnd,state);
        	                 break;
         }
         default: break;
    }

    if(ind != Expanders::MAIN && p_mWnd) {
    	  Action laction = on_auto_adjust(p_mWnd->mb_expr.get_expanded(),p_mWnd->os_expr.get_expanded(),p_mWnd->mm_expr.get_expanded(),p_mWnd->mn_expr.get_expanded(),
    			                                            p_mWnd->au_expr.get_expanded(),p_mWnd->nt_expr.get_expanded(),p_mWnd->v_expr.get_expanded());

    if(laction == Action::EXPAND) p_mWnd->v_expr.set_expanded();
    else
    	if(laction == Action::SHRINK) p_mWnd->v_expr.set_expanded(false);
      }
}

uhiExpand::Action uhiExpand::on_auto_adjust(bool mbe, bool ose, bool mme, bool mne, bool aue, bool nte, bool ve) const
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
