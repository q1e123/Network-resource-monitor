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

    static_box_sizer = new wxStaticBoxSizer(static_box, wxVERTICAL);
    static_box_sizer->Add(header_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(os_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(current_user_text, 0, wxALL | wxEXPAND, 5);
    static_box_sizer->Add(machine_id_text, 0, wxALL | wxEXPAND, 5);
}

System_Card::~System_Card(){
}

wxStaticBoxSizer* System_Card::get_all(){
    return static_box_sizer;
}