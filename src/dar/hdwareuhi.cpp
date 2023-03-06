/*
 *    uhInfo
 *    Copyright (C) 2023
 */

#include "hdwareuhi.h"
#include "../uhirc.h"

CDrArUhi::CDrArUhi()
{
	set_size_request(draw::dwaw,draw::dwah);
	//Just setting up a "connection",not a virtual call to a pure virtual function from here
    set_draw_func(sigc::mem_fun(*this, &CDrArUhi::on_draw_area));
    set_expand();
}
