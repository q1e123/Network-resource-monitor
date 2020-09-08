#ifndef SYSTEM_ADMINISTRATION_PAGE_H
#define SYSTEM_ADMINISTRATION_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "System.h"

class System_Administration_Page{
public:
    System_Administration_Page();
    System_Administration_Page(wxNotebook *main_notebook, System *system);
    ~System_Administration_Page();
    wxNotebookPage* get_all();
    void update();

private:
    System system
    wxStaticBoxSizer *system_sbox;
    wxStaticBox  *system_static;
    wxNotebookPage *system_page;
    wxNotebook *notebook;

    Process_List_Panel *process_list_panel;
};

#endif