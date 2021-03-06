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
#include "add-user-form.h"

class User_Management_Page{
public:
    User_Management_Page();
    User_Management_Page(wxNotebook *main_notebook);

    wxNotebookPage* get_all();

    void update_user_cards(std::vector<DB_Users> users);

    std::vector<DB_Users> get_db_users();
    DB_Users get_new_user();

private:
    std::string user_role;

	Add_User_From *add_user_form;

    wxNotebookPage *page;
    wxBoxSizer *box_sizer, *scrolled_sizer, *buttons_sizer;
    wxStaticBox *static_box;
    wxButton *submit_changes_button, *refresh_button;
    wxScrolledWindow *scrolled_panel;

    std::map<std::string, User_Management_Card*> user_cards;
    void add(DB_Users db_user);

};

#endif