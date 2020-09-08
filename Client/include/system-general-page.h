#ifndef SYSTEM_GENERAL_PAGE_H
#define SYSTEM_GENERAL_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"
#include "system-header.h"
#include "system-card.h"

class System_General_Page{
public:
    System_General_Page();
    System_General_Page(wxNotebook *main_notebook, System *system);
    ~System_General_Page();
    wxNotebookPage* get_all();

    void update();
    
private:
    System *system;
    wxStaticBoxSizer *system_sbox;
    wxStaticBox  *system_static;
    wxNotebookPage *system_page;

    System_Card *system_card;
};

#endif