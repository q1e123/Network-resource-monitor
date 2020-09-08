#ifndef SYSTEM_SOFTWARE_PAGE_H
#define SYSTEM_SOFTWARE_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"
#include "software-list-panel.h"

class System_Software_Page{
public:
    System_Software_Page();
    System_Software_Page(wxNotebook *main_notebook, System *system);
    ~System_Software_Page();
    wxNotebookPage* get_all();
    void update();
    void check_software();

private:
    System *system;
    wxStaticBoxSizer *system_sbox;
    wxStaticBox  *system_static;
    wxNotebookPage *system_page;
    wxNotebook *notebook;
    wxTextCtrl *software_input;
    wxButton *check_software_button;

    Software_List_Panel *software_list_panel;
};

#endif