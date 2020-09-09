#ifndef SOFTWARE_LIST_PANEL_H
#define SOFTWARE_LIST_PANEL_H


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"

class Software_List_Panel{
public:
    Software_List_Panel();
    Software_List_Panel(wxNotebookPage *system_page, System *system);
    ~Software_List_Panel();

    void update(System *system = nullptr);
    wxScrolledWindow* get_all();
private:
    System *system;
    wxScrolledWindow *panel;
    wxStaticText *total_installed_software_text, *software_list_text;
    wxBoxSizer *box_sizer;
};

#endif