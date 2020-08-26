#include "add-system-form.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"

Add_System_From::Add_System_From(){}

Add_System_From::Add_System_From(wxWindow *parent){
    static_box = new wxStaticBox(parent, wxID_ANY,"");
	static_box->SetBackgroundColour(Colors::light_blue);

	sizer = new wxBoxSizer(wxVERTICAL);

	machine_id_sizer = new wxBoxSizer(wxHORIZONTAL);
    machine_id_text = new wxStaticText(parent,wxID_ANY,"Machine ID:");
	machine_id_text->SetForegroundColour(Colors::black);
	machine_id_input = new wxTextCtrl(parent,wxID_ANY,"");

	
	submit_button = new wxButton(parent, GUI_Element_ID::BUTTON_ADD_NEW_SYSTEM, "Submit");
	submit_button->SetBackgroundColour(Colors::light_green);
	submit_button->SetForegroundColour(Colors::black);


	machine_id_sizer->Add(machine_id_text, 0, wxALL, 5);
	machine_id_sizer->Add(machine_id_input, 1, wxALL | wxEXPAND, 5);

	sizer->Add(machine_id_sizer, 1, wxALL | wxEXPAND, 5);

	static_box_sizer = new wxStaticBoxSizer(static_box,wxVERTICAL);
	static_box_sizer->Add(sizer, 0, wxALL | wxEXPAND, 5);
	static_box_sizer->Add(submit_button, 0, wxALL | wxEXPAND, 5);
}

Add_System_From::~Add_System_From(){
}

wxStaticBoxSizer* Add_System_From::get_all(){
    return static_box_sizer;
}

DB_Systems Add_System_From::get_db_systems(){
	DB_Systems db_system;
	
	db_system.machine_id = machine_id_input->GetValue().ToStdString();
	
	return db_system;
}