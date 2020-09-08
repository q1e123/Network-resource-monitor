#ifndef SYSTEM_PAGE_H
#define SYSTEM_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"
#include "system-header.h"
#include "system-general-page.h"
#include "system-administration-page.h"
#include "system-software-page.h"
#include "system-processes-page.h"
#include "process-sort-type.h"
class System_Page{
public:
    System_Page();
    System_Page(wxNotebook *main_notebook, System *system);
    ~System_Page();
    wxNotebookPage* get_all();
    void update(Process_Sort_Type sort_type);
    void check_software();
private:
    wxStaticText *system_text;
    wxStaticBoxSizer *system_sbox;
    wxStaticBox  *system_static;
    wxNotebookPage *system_page;
    wxNotebook *notebook;

    System_Header *system_header;
    System_General_Page *system_general_page;
    System_Administration_Page *system_administration_page;
    System_Software_Page *system_software_page;
    System_Processes_Page *system_processes_page;
};

#endif