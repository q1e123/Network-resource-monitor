#include "tray-icon.h"

wxBEGIN_EVENT_TABLE(Tray_Icon, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DCLICK  (Tray_Icon::on_left_button_click)
wxEND_EVENT_TABLE()


#include "tray-icon.xpm"

Tray_Icon::Tray_Icon(wxFrame *mainframe){
    this->mainframe = mainframe;
    wxIcon icon(icon_xpm);
    this->SetIcon(icon, "wxTaskBarIcon Sample - a different icon");
}

void Tray_Icon::on_left_button_click(wxTaskBarIconEvent&){
    mainframe->Show(!(mainframe->IsShown()));
}