#include "real-time-management-card.h"
#include "colors.h"
#include "fonts.h"

Real_Time_Management_Card::Real_Time_Management_Card(wxWindow *parent, System *system){
	sizer = new wxBoxSizer(wxVERTICAL);
	card_static = new wxStaticBox(parent, wxID_ANY, "");
	user_text = new wxStaticText(parent, wxID_ANY, "User: " + system->get_current_user()); 
	user_text->SetForegroundColour(Colors::black);
	user_text->SetFont(Fonts::h1);
	os_text = new wxStaticText(parent, wxID_ANY, system->get_os()); 
	os_text->SetForegroundColour(Colors::black);
	os_text->SetFont(Fonts::normal);
	machine_id_text = new wxStaticText(parent, wxID_ANY, "Machine ID: " + system->get_machine_id()); 
	machine_id_text->SetForegroundColour(Colors::black);
	machine_id_text->SetFont(Fonts::normal);
	rx_tx_box_sizer = new wxBoxSizer(wxHORIZONTAL); 
	environment_variable_box_sizer = new wxBoxSizer(wxHORIZONTAL);
	user_box_sizer = new wxBoxSizer(wxHORIZONTAL);
	rx_text = new wxStaticText(parent, wxID_ANY, "In: 0");
	rx_text->SetFont(Fonts::normal_bold);
	rx_text->SetForegroundColour(Colors::dark_green);
	tx_text = new wxStaticText(parent, wxID_ANY, "Out: 0");
	tx_text->SetFont(Fonts::normal_bold);
	tx_text->SetForegroundColour(Colors::light_red);
	
	for(auto network_interface : system->get_network_interfaces()){
		network_choices.push_back(network_interface);
	}
	interface_combo_box = new wxComboBox(parent, wxID_ANY, "Select interface");
	interface_combo_box->Set(network_choices);

	for(auto user : system->get_user_list()){
		user_choices.push_back(user.username);
	}
	user_combo_box = new wxComboBox(parent, wxID_ANY, "Select user");
	user_combo_box->Set(user_choices);

	for(auto env_var : system->get_environment_variables()){
		environment_variable_choices.push_back(env_var.first);
	}
	environment_variable_combo_box = new wxComboBox(parent, wxID_ANY, "Select environment variable");
	environment_variable_combo_box->Set(environment_variable_choices);

	std::string ram = "";
	ram += "Total ram: " + std::to_string(system->get_total_ram()) + "\t";
	ram += "Avalabile ram: " + std::to_string(system->get_avalabile_ram()) + "\t";
	ram += "Used ram: " + std::to_string(system->get_used_ram());

	std::string cpu = "";
	for(auto item : system->get_cpu_usage()){
		cpu+= item.first + ": " + std::to_string(item.second) + "%\t";
	}

	ram_text = new wxStaticText(parent, wxID_ANY, ram);
	ram_text->SetForegroundColour(Colors::black);
	ram_text->SetFont(Fonts::normal);
	cpu_text = new wxStaticText(parent, wxID_ANY, cpu); 
	cpu_text->SetForegroundColour(Colors::black);
	cpu_text->SetFont(Fonts::normal);

	user_log_text = new wxStaticText(parent, wxID_ANY, "Last log in: ");
	user_log_text->SetFont(Fonts::normal);
	user_log_text->SetForegroundColour(Colors::black);

	environment_variable_text = new wxStaticText(parent, wxID_ANY, "Environment variable value");
	environment_variable_text->SetFont(Fonts::normal);
	environment_variable_text->SetForegroundColour(Colors::black);

	card_sbox = new wxStaticBoxSizer(card_static, wxVERTICAL);

	card_sbox->Add(user_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(os_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(machine_id_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(ram_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(cpu_text, 0, wxALL | wxEXPAND, 5);

	rx_tx_box_sizer->Add(rx_text, 0, wxALL | wxEXPAND, 5);
	rx_tx_box_sizer->Add(tx_text, 0, wxALL | wxEXPAND, 5);

	user_box_sizer->Add(user_combo_box, 0, wxALL | wxEXPAND, 5);
	user_box_sizer->Add(user_log_text, 0, wxALL | wxEXPAND, 5);

	environment_variable_box_sizer->Add(environment_variable_combo_box, 0, wxALL | wxEXPAND, 5);
	environment_variable_box_sizer->Add(environment_variable_text, 0, wxALL | wxEXPAND, 5);

	card_sbox->Add(interface_combo_box, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(rx_tx_box_sizer, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(user_box_sizer, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(environment_variable_box_sizer, 0, wxALL | wxEXPAND, 5);
}

void Real_Time_Management_Card::update(System *system){
	std::string ram = "";
	ram += "Total ram: " + std::to_string(system->get_total_ram()) + "\t";
	ram += "Avalabile ram: " + std::to_string(system->get_avalabile_ram()) + "\t";
	ram += "Used ram: " + std::to_string(system->get_used_ram());

	std::string cpu = "";
	for(auto item : system->get_cpu_usage()){
		cpu+= item.first + ": " + std::to_string(item.second) + "%\t";
	}

	ram_text->SetLabel(ram);
	cpu_text->SetLabel(cpu);

	std::string interface_choice = std::string(interface_combo_box->GetStringSelection());
    if(interface_choice != ""){
		double rx, tx;
		rx = system->get_network_usage()[interface_choice].get_rx();
		tx = system->get_network_usage()[interface_choice].get_tx();
        rx_text->SetLabel("IN: " + std::to_string(rx).substr(0, std::to_string(rx).size() - 4));
        tx_text->SetLabel("OUT: " + std::to_string(tx).substr(0, std::to_string(tx).size() - 4));
    }


	std::string user_choice = std::string(user_combo_box->GetStringSelection());
    if(user_choice != ""){
		for(auto user : system->get_user_list()){
			if(user_choice == user.username) {
				struct tm* t = gmtime(&user.last_login);
				char buffer [80];
				std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", t);
				user_log_text->SetLabel("Last log in: " + std::string(buffer));
				break;
			}			
		}
    }

	std::string environment_variable_choice = std::string(environment_variable_combo_box->GetStringSelection());
    if(environment_variable_choice != ""){
		std::string value = system->get_environment_variables()[environment_variable_choice];
		environment_variable_text->SetLabel(value);
    }
}

void Real_Time_Management_Card::set_active(){
	card_static->SetBackgroundColour(Colors::light_green);
	active = true;
}

void Real_Time_Management_Card::set_inactive(){
	card_static->SetBackgroundColour(Colors::light_red);
	active = false;
}

Real_Time_Management_Card::Real_Time_Management_Card(){
}

wxStaticBoxSizer* Real_Time_Management_Card::get_items(){
	return card_sbox;
}

bool Real_Time_Management_Card::is_active(){
	return active;
}
