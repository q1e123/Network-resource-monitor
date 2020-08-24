#include "user-management-card.h"

#include "colors.h"
#include "fonts.h"

User_Management_Card::User_Management_Card(wxWindow *parent, DB_Users db_user){
    this->db_user = db_user;

	sizer = new wxBoxSizer(wxVERTICAL);
	machine_id_sizer = new wxBoxSizer(wxHORIZONTAL);
	system_id_sizer = new wxBoxSizer(wxHORIZONTAL);

	card_static = new wxStaticBox(parent, wxID_ANY, "");
	card_static->SetBackgroundColour(Colors::light_blue);
    id_text = new wxStaticText(parent,wxID_ANY,"ID: " + std::to_string(db_user.id));
	id_text->SetFont(Fonts::normal_bold);
	id_text->SetForegroundColour(Colors::black);

    username_text = new wxStaticText(parent,wxID_ANY,"Username: " + db_user.username);
	username_text->SetFont(Fonts::normal_bold);
	username_text->SetForegroundColour(Colors::black);

    machine_id_text = new wxStaticText(parent,wxID_ANY,"Machine ID:");
	machine_id_text->SetFont(Fonts::normal_bold);
	machine_id_text->SetForegroundColour(Colors::black);
    machine_id_input = new wxTextCtrl(parent,wxID_ANY,db_user.machine_id);
	machine_id_input->SetForegroundColour(Colors::white);

    system_id_text = new wxStaticText(parent,wxID_ANY,"System ID: ");
	system_id_text->SetFont(Fonts::normal_bold);
	system_id_text->SetForegroundColour(Colors::black);
    system_id_input = new wxTextCtrl(parent,wxID_ANY,std::to_string(db_user.system_id));
	system_id_input->SetForegroundColour(Colors::white);

	user_role_checkbox = new wxCheckBox(parent, wxID_ANY, "Administrator");
	user_role_checkbox->SetForegroundColour(Colors::black);
	if(db_user.user_rank == 1){
		user_role_checkbox->SetValue(true);
	} else if (db_user.user_rank == 0) {
		user_role_checkbox->SetValue(false);
	}

	card_sbox = new wxStaticBoxSizer(card_static, wxVERTICAL);
	card_sbox->Add(id_text, 1, wxALL | wxEXPAND, 5);
	card_sbox->Add(username_text, 1, wxALL | wxEXPAND, 5);

	machine_id_sizer->Add(machine_id_text, 0, wxALL, 5);
	machine_id_sizer->Add(machine_id_input, 1, wxALL | wxEXPAND, 5);
	system_id_sizer->Add(system_id_text, 0, wxALL, 5);
	system_id_sizer->Add(system_id_input, 1, wxALL | wxEXPAND, 5);

	card_sbox->Add(machine_id_sizer, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(system_id_sizer, 0, wxALL | wxEXPAND, 5);
	card_sbox->Add(user_role_checkbox, 1, wxALL | wxEXPAND, 5);

}

User_Management_Card::User_Management_Card(){
}

wxStaticBoxSizer* User_Management_Card::get_items(){
	return card_sbox;
}

DB_Users User_Management_Card::get_db_user(){
    this->db_user.machine_id = machine_id_input->GetValue().ToStdString();
    this->db_user.system_id = std::stoi(system_id_input->GetValue().ToStdString());
	if(user_role_checkbox->GetValue()){
		this->db_user.user_rank = 1;
	} else {
		this->db_user.user_rank = 0;
	}
    return db_user;
}
