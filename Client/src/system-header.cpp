#include "system-header.h"

#include "colors.h"
#include "gui-element-id.h"

System_Header::System_Header(){}

System_Header::System_Header(wxNotebookPage *system_page){
	this->system = system;
    static_box = new wxStaticBox(system_page, wxID_ANY,"");
	static_box->SetBackgroundColour(Colors::dark_gray);
	buttons_box = new wxBoxSizer(wxHORIZONTAL);
	restart_button = new wxButton(system_page, GUI_Element_ID::BUTTON_RESTART, "Restart");
	restart_button->SetBackgroundColour(Colors::light_gray);
	restart_button->SetForegroundColour(Colors::black);
	shutdown_button = new wxButton(system_page, GUI_Element_ID::BUTTON_SHUTDOWN, "Shutdown");
	shutdown_button->SetBackgroundColour(Colors::light_gray);
	shutdown_button->SetForegroundColour(Colors::black);
	buttons_box->Add(shutdown_button, 0, wxALL | wxEXPAND, 5);
	buttons_box->Add(restart_button, 0, wxALL | wxEXPAND, 5);
	static_box_sizer = new wxStaticBoxSizer(static_box,wxVERTICAL);
	static_box_sizer->Add(buttons_box, 0, wxALL | wxEXPAND, 5);
}

System_Header::~System_Header(){
}

wxStaticBoxSizer* System_Header::get_all(){
    return static_box_sizer;
}