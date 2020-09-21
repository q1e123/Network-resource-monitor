#include "user-management-page.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"

User_Management_Page::User_Management_Page(){};

User_Management_Page::User_Management_Page(wxNotebook *main_notebook){
	user_role = "Not set";

    page = new wxPanel(main_notebook, wxID_ANY);
	add_user_form = new Add_User_From(page);
	page->SetBackgroundColour(Colors::dark_gray);

	box_sizer= new wxBoxSizer(wxVERTICAL);
    submit_changes_button = new wxButton(page, GUI_ELEMENT_ID::BUTTON_USER_MANAGEMENT_SUMBIT_CHANGES, "Sumbit changes");
	submit_changes_button->SetForegroundColour(Colors::black);
	submit_changes_button->SetBackgroundColour(Colors::light_green);
   	refresh_button = new wxButton(page, GUI_ELEMENT_ID::BUTTON_USER_MANAGEMENT_REFRESH, "Refresh");
	refresh_button->SetForegroundColour(Colors::black);
	refresh_button->SetBackgroundColour(Colors::light_green);

	buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	buttons_sizer->Add(submit_changes_button, 0, wxALL, 5);
	buttons_sizer->Add(refresh_button, 0, wxALL, 5);
	
	scrolled_panel = new wxScrolledWindow(page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "User List");
	scrolled_sizer = new wxBoxSizer(wxVERTICAL);
	scrolled_panel->SetSizer(scrolled_sizer);
	scrolled_panel->FitInside();
	scrolled_panel->SetScrollRate(20,20);

	box_sizer->Add(add_user_form->get_all(), 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(buttons_sizer, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(scrolled_panel, 1, wxALL | wxEXPAND, 5);
	page->SetSizerAndFit(box_sizer);
}

wxNotebookPage* User_Management_Page::get_all(){
    return page;
}

void User_Management_Page::update_user_cards(std::vector<DB_Users> users){
	for(auto user : users){
		std::string username = user.username;
		if(user_cards.find(username) == user_cards.end()){
            add(user);
		}else{
			user_cards[username]->update(user);
		}
	}
	scrolled_panel->FitInside();
}

void User_Management_Page::add(DB_Users db_users){
    User_Management_Card *umc = new User_Management_Card(scrolled_panel, db_users);
    scrolled_sizer->Add(umc->get_items(),  0, wxALL | wxEXPAND, 5);
    user_cards[db_users.username] = umc;
}

std::vector<DB_Users> User_Management_Page::get_db_users(){
	std::vector<DB_Users> user_list;
	for(auto item : user_cards){
		DB_Users db_user = item.second->get_db_user();
		user_list.push_back(db_user);
	}
	return user_list;
}

DB_Users User_Management_Page::get_new_user(){
	return this->add_user_form->get_db_user();
}