#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/taskbar.h>

class Tray_Icon : public wxTaskBarIcon{
public:
    Tray_Icon(wxFrame *mainframe);
    void on_left_button_click(wxTaskBarIconEvent&);
    wxDECLARE_EVENT_TABLE();

private:
    wxFrame *mainframe;
    bool gui;
};


#endif