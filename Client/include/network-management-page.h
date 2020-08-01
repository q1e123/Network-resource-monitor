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
#include "recv-package.h"

class Network_Management_Page{
public:
    Network_Management_Page();
    Network_Management_Page(wxNotebook *main_notebook, System *system);

    wxNotebookPage* get_all();
    void change_to_connected_gui(std::string server_name);
    void update_user_cards(Recv_Package recv_package);

    std::string get_user();
    std::string get_ip();
    size_t get_port();
private:
    System *system;
    wxNotebookPage *page;
    wxBoxSizer *box_sizer, *connect_box_sizer, *user_cards_box_sizer;
    wxStaticBox *static_box;
    wxStaticText *header_text, *network_text;
    wxTextCtrl *user_input, *ip_input, *port_input;
    wxButton *connect_button;
    std::map<std::string, User_Card> user_cards;

};

#endif