/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "hdwareuhi.h"
#include "uhirc.h"

CDrArUhi::CDrArUhi()
{
	set_size_request(draw::dwaw,draw::dwah);
    set_draw_func(sigc::mem_fun(*this, &CDrArUhi::on_draw_area));
    set_expand();
}
