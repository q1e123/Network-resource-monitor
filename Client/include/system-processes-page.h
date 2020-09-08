#ifndef SYSTEM_PROCESSES_PAGE_H
#define SYSTEM_PROCESSES_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "process-list-panel.h"
#include "process-sort-type.h"
class System_Processes_Page{
public:
    System_Processes_Page();
    System_Processes_Page(wxNotebook *main_notebook, System *system);
    ~System_Processes_Page();
    wxNotebookPage* get_all();
    void update_process_list(Process_Sort_Type sort_type);

private:
    wxStaticBoxSizer *system_sbox;
    wxStaticBox  *system_static;
    wxNotebookPage *system_page;
    wxNotebook *notebook;

    Process_List_Panel *process_list_panel;
};

#endif