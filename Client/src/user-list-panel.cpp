#include "user-list-panel.h"

#include <vector>
#include <algorithm>
#include <string>

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"
#include "my-process.h"
#include "utils.h"

User_List_Panel::User_List_Panel(){}

User_List_Panel::User_List_Panel(wxNotebookPage *system_page, System *system){
    this->system = system;

    panel = new wxScrolledWindow(system_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "Process List");
	panel->SetBackgroundColour(Colors::dark_gray);

	box_sizer = new wxBoxSizer(wxHORIZONTAL);
	user_list_box_sizer = new wxBoxSizer(wxVERTICAL);
	last_log_box_sizer = new wxBoxSizer(wxVERTICAL);

	user_list_text = new wxStaticText(panel, wxID_ANY, "---");
	user_list_text->SetFont(Fonts::normal);
	user_list_text->SetForegroundColour(Colors::white);
	last_log_text = new wxStaticText(panel, wxID_ANY, "---");
	last_log_text->SetFont(Fonts::normal);
	last_log_text->SetForegroundColour(Colors::white);
    
	user_list_box_sizer->Add(user_list_text, 1, wxALL, 5);
    last_log_box_sizer->Add(last_log_text, 1, wxALL, 5);
		
	box_sizer->Add(user_list_box_sizer, 1, wxALL, 5);
	box_sizer->Add(last_log_box_sizer, 1, wxALL, 5);
	
	panel->SetSizer(box_sizer);
	panel->FitInside();
	panel->SetScrollRate(5,5);
}

User_List_Panel::~User_List_Panel(){}

wxScrolledWindow* User_List_Panel::get_all(){
    return panel;
}

void User_List_Panel::update(){
    std::vector<System_User> user_list = system->get_user_list();

    std::string users, last_log;
    users = last_log = "";
    for (auto user : user_list) {
        users += user.username + "\n";
		struct tm* t = gmtime(&user.last_login);
		char buffer [80];
    	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", t);
        last_log += std::string(buffer) + "\n";
    }
    user_list_text->SetLabel(users);
    last_log_text->SetLabel(last_log);

    panel->FitInside();
}