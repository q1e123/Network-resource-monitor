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
	card_sbox = new wxStaticBoxSizer(card_static, wxVERTICAL);

	card_sbox->Add(user_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(os_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(machine_id_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(ram_text, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(cpu_text, 0, wxALL | wxEXPAND, 5);
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
