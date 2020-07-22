#include "user-card.h"

User_Card::User_Card(wxWindow *parent, wxWindowID id, std::string user, System system){
	active_color = wxColour(145,255,160);
	inactive_color = wxColour(255,138,138);
	label_color = wxColour(0, 0 ,0);

	sizer = new wxBoxSizer(wxVERTICAL);
	card_static = new wxStaticBox(parent, id, "");
	user_text = new wxStaticText(parent, wxID_ANY, user); 
	user_text->SetForegroundColour(label_color);
	os_text = new wxStaticText(parent, wxID_ANY, system.get_os()); 
	os_text->SetForegroundColour(label_color);

	std::string ram = "";
	ram += "Total ram: " + std::to_string(system.get_total_ram()) + "\t";
	ram += "Avalabile ram: " + std::to_string(system.get_avalabile_ram()) + "\t";
	ram += "Used ram: " + std::to_string(system.get_used_ram());

	std::string cpu = "";
	for(auto item : system.get_cpu_usage()){
		cpu+= item.first + ": " + std::to_string(item.second) + "%\t";
	}

	ram_text = new wxStaticText(parent, wxID_ANY, ram);
	ram_text->SetForegroundColour(label_color);
	cpu_text = new wxStaticText(parent, wxID_ANY, cpu); 
	cpu_text->SetForegroundColour(label_color);
	card_sbox = new wxStaticBoxSizer(card_static, wxVERTICAL);

	card_sbox->Add(user_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(os_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(ram_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(cpu_text, 0, wxALL | wxEXPAND, 5);
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

User_Card::User_Card(){
}

wxStaticBoxSizer* User_Card::get_items(){
	return card_sbox;
}
