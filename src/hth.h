/*
 *    uhInfo
 *    Copyright (C) 2018
 */

#ifndef _HTH_H_
#define _HTH_H_

#include <future>

class CHWindow;

class CInitThread 
{
	using fp_DispNtf = void (CHWindow::*)();
protected :
    virtual void Init(CHWindow *plMw) const;
    virtual std::string MOBO_Info() const;
    virtual std::string OS_Info() const;
    virtual std::string Mem_Info() const;
    virtual std::string GetDevices(DeviceType dType) const;
    virtual ~CInitThread() = default;
    
    std::unique_ptr<Glib::Dispatcher> m_Dispatch{nullptr};
    std::unique_ptr<sigc::connection> m_Connection{nullptr};
    std::future<int> th_fstate;

    void CreateThreadDispatcher() {if(!m_Dispatch) m_Dispatch = std::make_unique<Glib::Dispatcher>();}
    void FinishThreadAndClear();
    void VeryFastQuitGuard() const; 
    virtual void PrepAndMakeThread(CHWindow *m_wnd,fp_DispNtf func);
    virtual void CreateThread(CHWindow *m_wnd);
};

#endif // _HTH_H_
