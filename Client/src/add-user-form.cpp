#include "add-user-form.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"

Add_User_From::Add_User_From(){}

Add_User_From::Add_User_From(wxWindow *parent){
    static_box = new wxStaticBox(parent, wxID_ANY,"");
	static_box->SetBackgroundColour(Colors::light_blue);

	sizer = new wxBoxSizer(wxVERTICAL);

	username_sizer = new wxBoxSizer(wxHORIZONTAL);
    username_text = new wxStaticText(parent,wxID_ANY,"Username:");
	username_text->SetForegroundColour(Colors::black);
	username_input = new wxTextCtrl(parent,wxID_ANY,"");

	system_id_sizer = new wxBoxSizer(wxHORIZONTAL);
    system_id_text = new wxStaticText(parent,wxID_ANY,"System ID:");
	system_id_text->SetForegroundColour(Colors::black);
    system_id_input = new wxTextCtrl(parent,wxID_ANY,"");

	submit_button = new wxButton(parent, GUI_Element_ID::BUTTON_ADD_NEW_USER, "Submit");
	submit_button->SetBackgroundColour(Colors::light_green);
	submit_button->SetForegroundColour(Colors::black);


	username_sizer->Add(username_text, 0, wxALL, 5);
	username_sizer->Add(username_input, 1, wxALL | wxEXPAND, 5);
	system_id_sizer->Add(system_id_text, 0, wxALL, 5);
	system_id_sizer->Add(system_id_input, 1, wxALL | wxEXPAND, 5);
	sizer->Add(username_sizer, 1, wxALL | wxEXPAND, 5);
	sizer->Add(system_id_sizer, 1, wxALL | wxEXPAND, 5);

	static_box_sizer = new wxStaticBoxSizer(static_box,wxVERTICAL);
	static_box_sizer->Add(sizer, 0, wxALL | wxEXPAND, 5);
	static_box_sizer->Add(submit_button, 0, wxALL | wxEXPAND, 5);
}

Add_User_From::~Add_User_From(){
}

wxStaticBoxSizer* Add_User_From::get_all(){
    return static_box_sizer;
}

DB_Users Add_User_From::get_db_user(){
	DB_Users db_user;
	
	db_user.username = username_input->->GetValue().ToStdString();
	db_user.system_id = std::stoi(system_id_input->GetValue().ToStdString());
	if(user_role_checkbox->GetValue()){
		db_user.user_role = 1;
	} else {
		db_user.user_role = 0;
	}

	return db_user;
}