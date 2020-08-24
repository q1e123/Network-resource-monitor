#include "user-management-page.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"

User_Management_Page::User_Management_Page(){};

User_Management_Page::User_Management_Page(wxNotebook *main_notebook){
	user_role = "Not set";

    page = new wxPanel(main_notebook, wxID_ANY);
	scrolled_panel = new wxScrolledWindow(page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "User List");
	scrolled_sizer = new wxBoxSizer(wxVERTICAL);
	box_sizer= new wxBoxSizer(wxVERTICAL);
	network_text = new wxStaticText(page,wxID_ANY,"");
	network_text->SetFont(Fonts::h1);
	network_text->SetForegroundColour(Colors::white);
	user_role_text = new wxStaticText(page,wxID_ANY,"");
	user_role_text->SetFont(Fonts::h1);
	user_role_text->SetForegroundColour(Colors::white);
    submit_changes = new wxButton(page, GUI_ELEMENT_ID::BUTTON_USER_MANAGEMENT_SUMBIT_CHANGES, "Sumbit changes");
	submit_changes->SetForegroundColour(Colors::black);
	submit_changes->SetBackgroundColour(Colors::light_green);
	
	box_sizer->Add(submit_changes, 0, wxALL, 5);
	box_sizer->Add(scrolled_panel, 0, wxALL | wxEXPAND, 5);

	page->SetSizerAndFit(box_sizer);
	scrolled_panel->SetSizer(scrolled_sizer);
	scrolled_panel->FitInside();
	scrolled_panel->SetScrollRate(20,02);
	
}

wxNotebookPage* User_Management_Page::get_all(){
    return page;
}

void User_Management_Page::update_user_cards(std::vector<DB_Users> users){
	for(auto user : users){
		std::string username = user.username;
		if(user_cards.find(username) == user_cards.end()){
            add(user);
		}
	}
	page->SetSizerAndFit(box_sizer);
}

void User_Management_Page::add(DB_Users db_users){
    User_Management_Card *umc = new User_Management_Card(page, db_users);
    scrolled_sizer->Add(umc->get_items(),  0, wxEXPAND);
    user_cards[db_users.username] = umc;
}