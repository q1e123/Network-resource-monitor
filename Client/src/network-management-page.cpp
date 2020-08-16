#include "network-management-page.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"
Network_Management_Page::Network_Management_Page(){};

Network_Management_Page::Network_Management_Page(wxNotebook *main_notebook, System *system){
    page = new wxPanel(main_notebook, wxID_ANY);
	box_sizer= new wxBoxSizer(wxVERTICAL);
	connect_box_sizer = new wxBoxSizer(wxHORIZONTAL);

	user_input = new wxTextCtrl(page,wxID_ANY,"USER");
	user_input->SetForegroundColour(Colors::white);
	ip_input = new wxTextCtrl(page,wxID_ANY,"127.0.0.1");
	ip_input->SetForegroundColour(Colors::white);
	port_input = new wxTextCtrl(page,wxID_ANY,"50005");
	port_input->SetForegroundColour(Colors::white);
	connect_button = new wxButton(page, GUI_ELEMENT_ID::BUTTON_CONNECT, "Connect");
	connect_button->SetForegroundColour(Colors::black);
	connect_button->SetBackgroundColour(Colors::light_green);
	network_text = new wxStaticText(page,wxID_ANY,"");
	network_text->SetFont(Fonts::h1);
	network_text->SetForegroundColour(Colors::white);
	network_text->Hide();
	user_role_text->SetFont(Fonts::h1);
	user_role_text->SetForegroundColour(Colors::white);
	user_role_text->Hide();

	connect_box_sizer->Add(user_input, 0, wxALL | wxEXPAND, 5);
	connect_box_sizer->Add(ip_input, 0, wxALL | wxEXPAND, 5);
	connect_box_sizer->Add(port_input, 0, wxALL | wxEXPAND, 5);
	connect_box_sizer->Add(connect_button, 0, wxALL | wxEXPAND, 5);
	
	box_sizer->Add(network_text, 0, wxALL, 5);
	box_sizer->Add(connect_box_sizer, 0, wxALL, 5);
	page->SetSizerAndFit(box_sizer);
}

wxNotebookPage* Network_Management_Page::get_all(){
    return page;
}

void Network_Management_Page::update_user_cards(Recv_Package recv_package){
	for(auto item : recv_package.get_user_data()){
		if(user_cards.find(item.first) != user_cards.end()){
			if(!user_cards[item.first].is_active()){
				user_cards[item.first].set_active();
			}
			user_cards[item.first].update(System(item.second));
		}else{
			User_Card uc = User_Card(page, wxID_ANY, item.first, System(item.second));
			uc.set_active();
			user_cards_box_sizer->Add(uc.get_items(), 1, wxALL | wxEXPAND, 5);
			user_cards[item.first] = uc;
		}
	}
	for(auto inactive : recv_package.get_inactive_users()){
		user_cards[inactive].set_inactive();
	}
	page->SetSizerAndFit(box_sizer);
}

void Network_Management_Page::change_to_connected_gui(std::string server_name){
    network_text->SetLabel("Connected to: " + server_name);
	network_text->Show();
	user_role_text->SetLabel("Role: " + this->user_role);
	user_role_text->Show();
	port_input->Hide();
	user_input->Hide();
	ip_input->Hide();
	connect_button->Hide();
	user_cards_box_sizer = new wxBoxSizer(wxVERTICAL);
	box_sizer->Add(user_cards_box_sizer, 0, wxALL, 5);
	page->SetSizerAndFit(box_sizer);
}

std::string Network_Management_Page::get_user(){
    std::string user = user_input->GetValue().ToStdString();
    return user;
}

std::string Network_Management_Page::get_ip(){
    std::string ip = ip_input->GetValue().ToStdString();
    return ip;
}

size_t Network_Management_Page::get_port(){
    size_t port = std::stol(port_input->GetValue().ToStdString());
    return port;
}

void Network_Management_Page::set_user_role(std::string role){
	this->user_role = user_role;
}
