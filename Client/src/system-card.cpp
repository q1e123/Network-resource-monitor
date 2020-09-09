#include "system-card.h"


#include "fonts.h"
#include "colors.h"

System_Card::System_Card(){}

System_Card::System_Card(wxNotebookPage *system_page, System *system){
    this->system = system;

    static_box = new wxStaticBox(system_page, wxID_ANY,"");
	os_text = new wxStaticText(system_page, wxID_ANY,"OS: " + system->get_os());
	os_text->SetForegroundColour(Colors::white);
	machine_id_text = new wxStaticText(system_page, wxID_ANY,"Machine ID: " + system->get_machine_id());
	machine_id_text->SetForegroundColour(Colors::white);
    header_text = new wxStaticText(system_page, wxID_ANY, "System");
    header_text->SetFont(Fonts::h1);
    header_text->SetForegroundColour(Colors::white);
    current_user_text = new wxStaticText(system_page, wxID_ANY, "User: " + system->get_current_user());
    current_user_text->SetForegroundColour(Colors::white);
    std::string avalabile_storage_str = std::to_string(system->get_avalabile_space());
    avalabile_storage_text = new wxStaticText(system_page, wxID_ANY, "Avalabile storage: " + avalabile_storage_str.substr(0, avalabile_storage_str.size()-4) + " GB");
    avalabile_storage_text->SetForegroundColour(Colors::white);

    ip_text = new wxStaticText(system_page, wxID_ANY, "IP");
    ip_text->SetFont(Fonts::h2);
    ip_text->SetForegroundColour(Colors::white);

    std::vector<wxString> interface_choices;
    for(auto network_interface : system->get_network_interfaces()){
		interface_choices.push_back(network_interface);
	}
	interface_combo_box = new wxComboBox(system_page, wxID_ANY);
	interface_combo_box->Set(interface_choices);
    ipv4_text = new wxStaticText(system_page, wxID_ANY, "IPv4: ");
    ipv4_text->SetForegroundColour(Colors::white);
	ip_box_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    ip_box_sizer->Add(interface_combo_box, 0, wxALL, 5);
	ip_box_sizer->Add(ipv4_text, 0, wxALL | wxEXPAND, 5);
    
    static_box_sizer = new wxStaticBoxSizer(static_box, wxVERTICAL);
    static_box_sizer->Add(header_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(os_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(current_user_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(machine_id_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(ip_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(avalabile_storage_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(ip_box_sizer, 0, wxALL | wxEXPAND, 5);
}

System_Card::~System_Card(){
}

wxStaticBoxSizer* System_Card::get_all(){
    return static_box_sizer;
}

void System_Card::update(){
    std::string avalabile_storage_str = std::to_string(system->get_avalabile_space());
    avalabile_storage_text->SetLabel("Avalabile storage: " + avalabile_storage_str.substr(0, avalabile_storage_str.size()-4) + " GB");
    std::string interface_choice = std::string(interface_combo_box->GetStringSelection());
    if(interface_choice != ""){
        ipv4_text->SetLabel("IPv4: " + system->get_ipv4()[interface_choice]);
    }
}