#ifndef USER_MANAGEMENT_PAGE_H
#define USER_MANAGEMENT_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include <map>
#include <string>

#include "system.h"
#include "user-management-card.h"
#include "database-structs.h"

class User_Management_Page{
public:
    User_Management_Page();
    User_Management_Page(wxNotebook *main_notebook);

    wxNotebookPage* get_all();

    void update_user_cards(std::vector<DB_Users> users);

private:
    std::string user_role;
    wxNotebookPage *page;
    wxBoxSizer *box_sizer, *user_cards_box_sizer;
    wxStaticBox *static_box;
    wxStaticText *header_text, *network_text, *user_role_text;
    wxButton *submit_changes;
    std::map<std::string, User_Management_Card*> user_cards;
    void add(DB_Users db_user);

};

#endif