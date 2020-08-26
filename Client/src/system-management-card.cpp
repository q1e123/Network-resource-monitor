#include "system-management-card.h"

#include "colors.h"
#include "fonts.h"

System_Management_Card::System_Management_Card(wxWindow *parent, DB_Systems db_system){
    this->db_system = db_system;

	sizer = new wxBoxSizer(wxVERTICAL);
	machine_id_sizer = new wxBoxSizer(wxHORIZONTAL);

	card_static = new wxStaticBox(parent, wxID_ANY, "");
	card_static->SetBackgroundColour(Colors::light_blue);
    id_text = new wxStaticText(parent,wxID_ANY,"ID: " + std::to_string(db_system.id));
	id_text->SetFont(Fonts::normal_bold);
	id_text->SetForegroundColour(Colors::black);

    system_status = new wxStaticText(parent,wxID_ANY,"System status: " + std::to_string(db_system.status));
	system_status->SetFont(Fonts::normal_bold);
	system_status->SetForegroundColour(Colors::black);
	if(db_system.status == 1){
		turn_on();
	} else if(db_system.status == 0) {
		turn_off();
	}

    machine_id_text = new wxStaticText(parent,wxID_ANY,"Machine ID:");
	machine_id_text->SetFont(Fonts::normal_bold);
	machine_id_text->SetForegroundColour(Colors::black);
    machine_id_input = new wxTextCtrl(parent,wxID_ANY,db_system.machine_id);
	machine_id_input->SetForegroundColour(Colors::white);

	card_sbox = new wxStaticBoxSizer(card_static, wxVERTICAL);
	card_sbox->Add(id_text, 1, wxALL | wxEXPAND, 5);
	card_sbox->Add(system_status, 1, wxALL | wxEXPAND, 5);

	machine_id_sizer->Add(machine_id_text, 0, wxALL, 5);
	machine_id_sizer->Add(machine_id_input, 1, wxALL | wxEXPAND, 5);

	card_sbox->Add(machine_id_sizer, 0, wxALL | wxEXPAND, 5);
}

System_Management_Card::System_Management_Card(){
}

wxStaticBoxSizer* System_Management_Card::get_items(){
	return card_sbox;
}

DB_Systems System_Management_Card::get_db_system(){
    this->db_system.machine_id = machine_id_input->GetValue().ToStdString();
    return db_system;
}

void System_Management_Card::update(DB_Systems db_system){
	this->db_system = db_system;
	machine_id_input->SetValue(db_system.machine_id);
	if(db_system.status == 1){
		turn_on();
	} else if(db_system.status == 0) {
		turn_off();
	}
}
void System_Management_Card::turn_on(){
	card_static->SetBackgroundColour(Colors::light_green);
}

void System_Management_Card::turn_off(){
	card_static->SetBackgroundColour(Colors::light_red);
}