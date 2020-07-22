#include "user-card.h"

User_Card::User_Card(wxWindow *parent, wxWindowID id, std::string user, System system){
	card_static = new wxStaticBox(parent, id, "");
	user_text = new wxStaticText(parent, wxID_ANY, user); 
	os_text = new wxStaticText(parent, wxID_ANY, system.get_os()); 

	std::string ram = "";
	ram += "Total ram: " + std::to_string(system.get_total_ram()) + "\t";
	ram += "Avalabile ram: " + std::to_string(system.get_avalabile_ram()) + "\t";
	ram += "Used ram: " + std::to_string(system.get_used_ram());

	std::string cpu = "";
	for(auto item : system.get_cpu_usage()){
		cpu+= item.first + ": " + std::to_string(item.second) + "%\t";
	}

	ram_text = new wxStaticText(parent, wxID_ANY, ram);
	cpu_text = new wxStaticText(parent, wxID_ANY, cpu); 
	card_sbox = new wxStaticBoxSizer(card_static, wxVERTICAL);

	card_sbox->Add(user_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(os_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(ram_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(cpu_text, 0, wxALL | wxEXPAND, 5);

	active_color = wxColour(145,255,160);
	inactive_color = wxColour(255,138,138);
}

void User_Card::update(System system){
	std::string ram = "";
	ram += "Total ram: " + std::to_string(system.get_total_ram()) + "\t";
	ram += "Avalabile ram: " + std::to_string(system.get_avalabile_ram()) + "\t";
	ram += "Used ram: " + std::to_string(system.get_used_ram());

	std::string cpu = "";
	for(auto item : system.get_cpu_usage()){
		cpu+= item.first + ": " + std::to_string(item.second) + "%\t";
	}

	ram_text->SetLabel(ram);
	cpu_text->SetLabel(cpu);
}

void User_Card::set_active(){
	card_static->SetBackgroundColour(active_color);
}

void User_Card::set_inactive(){
	card_static->SetBackgroundColour(inactive_color);
}
