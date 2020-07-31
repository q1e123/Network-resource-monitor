#ifndef SYSTEM_PAGE_H
#define SYSTEM_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"
#include "system-header.h"
#include "system-card.h"
#include "process-list-panel.h"
#include "process-sort-type.h"
class System_Page{
public:
    System_Page();
    System_Page(wxNotebook *main_notebook, System *system);
    ~System_Page();
    wxNotebookPage* get_all();
    void update_process_list(Process_Sort_Type sort_type);

private:
    wxStaticText *system_text;
    wxStaticBoxSizer *system_sbox;
    wxStaticBox  *system_static;
    wxNotebookPage *system_page;

    System_Header *system_header;
    System_Card *system_card;
    Process_List_Panel *process_list_panel;
};

#endif