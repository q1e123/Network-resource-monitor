#include "network-management-page.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"
Network_Management_Page::Network_Management_Page(){};

Network_Management_Page::Network_Management_Page(wxNotebook *main_notebook, System *system, Client *client){
	user_role = "Not set";
	this->client = client;

    page = new wxPanel(main_notebook, wxID_ANY);
	box_sizer= new wxBoxSizer(wxVERTICAL);

	network_text = new wxStaticText(page,wxID_ANY,"");
	network_text->SetFont(Fonts::h1);
	network_text->SetForegroundColour(Colors::white);
	network_text->Hide();
	user_role_text = new wxStaticText(page,wxID_ANY,"");
	user_role_text->SetFont(Fonts::h1);
	user_role_text->SetForegroundColour(Colors::white);
	user_role_text->Hide();

	notebook = new wxNotebook(page, wxID_ANY);

	user_management_page = new User_Management_Page(notebook);
	notebook->AddPage(user_management_page->get_all(), "User Management", true);

	system_management_page = new System_Management_Page(notebook);
	notebook->AddPage(system_management_page->get_all(), "System Management");

	real_time_management_page = new Real_Time_Management_Page(notebook, client);
	notebook->AddPage(real_time_management_page->get_all(), "Real time");
	
	box_sizer->Add(network_text, 0, wxALL, 5);
	box_sizer->Add(user_role_text, 0, wxALL, 5);
    box_sizer->Add(notebook, 1, wxEXPAND);

	page->SetSizerAndFit(box_sizer);
}

wxNotebookPage* Network_Management_Page::get_all(){
    return page;
}

void Network_Management_Page::update_real_time_cards(std::vector<System*> active_systems, std::vector<std::string> inactives){
	real_time_management_page->update_real_time_cards(active_systems, inactives);
}

void Network_Management_Page::update_user_cards(std::vector<DB_Users> users){
	user_management_page->update_user_cards(users);
}
void Network_Management_Page::update_system_cards(std::vector<DB_Systems> systems){
	system_management_page->update_system_cards(systems);
}

void Network_Management_Page::change_to_connected_gui(std::string server_name){
    network_text->SetLabel("Connected to: " + server_name);
	network_text->Show();
	user_role_text->SetLabel("Role: " + this->user_role);
	user_role_text->Show();
	user_cards_box_sizer = new wxBoxSizer(wxVERTICAL);
	box_sizer->Add(user_cards_box_sizer, 0, wxALL | wxEXPAND, 5);
	page->SetSizerAndFit(box_sizer);
}

void Network_Management_Page::set_user_role(std::string role){
	this->user_role = role;
}

std::vector<DB_Users> Network_Management_Page::get_db_users(){
	return this->user_management_page->get_db_users();
}

std::vector<DB_Systems> Network_Management_Page::get_db_systems(){
	return this->system_management_page->get_db_systems();
}

DB_Users Network_Management_Page::get_new_user(){
	return this->user_management_page->get_new_user();
}

DB_Systems Network_Management_Page::get_new_system(){
	return this->system_management_page->get_new_system();
}