#ifndef SYSTEM_HEADER_H
#define SYSTEM_HEADER_H


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "gui-element-id.h"
#include "system.h"

class System_Header{
public:
    System_Header();
    System_Header(wxNotebookPage *system_page);
    ~System_Header();

    wxStaticBoxSizer* get_all();
private:
    System *system;
    wxBoxSizer *buttons_box;
    wxStaticBoxSizer *static_box_sizer;
    wxStaticBox *static_box;
    wxButton *restart_button, *shutdown_button;
};

#endif