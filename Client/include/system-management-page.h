#ifndef SYSTEM_MANAGEMENT_PAGE_H
#define SYSTEM_MANAGEMENT_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include <map>
#include <string>

#include "system.h"
#include "system-management-card.h"
#include "database-structs.h"

class System_Management_Page{
public:
    System_Management_Page();
    System_Management_Page(wxNotebook *main_notebook);

    wxNotebookPage* get_all();

    void update_system_cards(std::vector<DB_Systems> systems);

    std::vector<DB_Systems> get_db_systems();

private:
    wxNotebookPage *page;
    wxBoxSizer *box_sizer, *scrolled_sizer, *buttons_sizer;
    wxStaticBox *static_box;
    wxButton *submit_changes_button, *refresh_button;
    wxScrolledWindow *scrolled_panel;

    std::map<int, System_Management_Card*> system_cards;
    void add(DB_Systems db_system);

};

#endif