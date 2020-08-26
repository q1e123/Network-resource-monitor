#ifndef REAL_TIME_MANAGEMENT_PAGE_H
#define REAL_TIME_MANAGEMENT_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include <map>
#include <string>

#include "system.h"
#include "real-time-management-card.h"
#include "database-structs.h"
#include "add-user-form.h"

class Real_Time_Management_Page{
public:
    Real_Time_Management_Page();
    Real_Time_Management_Page(wxNotebook *main_notebook);

    wxNotebookPage* get_all();

    void update_real_time_cards(std::vector<System*> active_systems, std::vector<std::string> inactives);

private:
    wxNotebookPage *page;
    wxBoxSizer *box_sizer, *scrolled_sizer;
    wxStaticBox *static_box;
    wxScrolledWindow *scrolled_panel;

    std::map<std::string, Real_Time_Management_Card> real_time_cards;
    void add(DB_Users db_user);

};

#endif