#ifndef NETWORK_MANAGEMENT_PAGE_H
#define NETWORK_MANAGEMENT_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include <map>
#include <string>

#include "system.h"
#include "user-card.h"
#include "user-management-page.h"
#include "system-management-page.h"

class Network_Management_Page{
public:
    Network_Management_Page();
    Network_Management_Page(wxNotebook *main_notebook, System *system);

    wxNotebookPage* get_all();
    void change_to_connected_gui(std::string server_name);
    void update_user_cards(std::vector<System*> active_systems, std::vector<std::string> inactives);

    std::string get_ip();
    size_t get_port();

    void set_user_role(std::string role);
    void update_user_cards(std::vector<DB_Users> users);
    void update_system_cards(std::vector<DB_Systems> systems);

    std::vector<DB_Users> get_db_users();
    std::vector<DB_Systems> get_db_systems();
    
private:
    std::string user_role;
    System *system;
   	wxNotebook *notebook;
    wxNotebookPage *page;
    wxBoxSizer *box_sizer, *user_cards_box_sizer;
    wxStaticBox *static_box;
    wxStaticText *header_text, *network_text, *user_role_text;
    std::map<std::string, User_Card> user_cards;

    User_Management_Page *user_management_page;
    System_Management_Page *system_management_page;
    Add_User_From *add_user_form;

};

#endif