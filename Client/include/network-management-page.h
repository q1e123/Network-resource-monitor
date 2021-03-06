#ifndef NETWORK_MANAGEMENT_PAGE_H
#define NETWORK_MANAGEMENT_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include <map>
#include <string>

#include "system.h"
#include "user-management-page.h"
#include "system-management-page.h"
#include "real-time-management-page.h"
#include "client.h"

class Network_Management_Page{
public:
    Network_Management_Page();
    Network_Management_Page(wxNotebook *main_notebook, System *system, Client *client);

    wxNotebookPage* get_all();
    void change_to_connected_gui(std::string server_name);

    std::string get_ip();
    size_t get_port();

    void set_user_role(std::string role);
    void update_user_cards(std::vector<DB_Users> users);
    void update_system_cards(std::vector<DB_Systems> systems);
    void update_real_time_cards(std::vector<System*> active_systems, std::vector<std::string> inactives);

    std::vector<DB_Users> get_db_users();
    std::vector<DB_Systems> get_db_systems();

    DB_Users get_new_user();
    DB_Systems get_new_system();

private:
    Client *client;
    std::string user_role;
    System *system;
   	wxNotebook *notebook;
    wxNotebookPage *page;
    wxBoxSizer *box_sizer, *user_cards_box_sizer;
    wxStaticBox *static_box;
    wxStaticText *header_text, *network_text, *user_role_text;

    User_Management_Page *user_management_page;
    System_Management_Page *system_management_page;
    Real_Time_Management_Page *real_time_management_page;
    Add_User_From *add_user_form;

};

#endif