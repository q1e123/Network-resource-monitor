#include "system-administration-page.h"
#include "gui-element-id.h"

#include "colors.h"
#include "fonts.h"

System_Administration_Page::System_Administration_Page(){}

System_Administration_Page::System_Administration_Page(wxNotebook *main_notebook, System *system){
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);

	generate_log_button = new wxButton(system_page, GUI_Element_ID::BUTTON_GENERATE_LOG, "Generate new log");
	generate_log_button->SetBackgroundColour(Colors::light_green);
	generate_log_button->SetForegroundColour(Colors::black);
	system_sizer->Add(generate_log_button, 0, wxALL, 5);
	
	environment_variable_text = new wxStaticText(system_page, wxID_ANY, "Environment Variables");
	environment_variable_text->SetFont(Fonts::h2);
    environment_variable_text->SetForegroundColour(Colors::white);
	environment_variable_panel = new Environment_Variable_Panel(system_page, system);

	system_sizer->Add(environment_variable_text, 0, wxALL, 5);
	system_sizer->Add(environment_variable_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	
	user_list_text = new wxStaticText(system_page, wxID_ANY, "Users");
	user_list_text->SetFont(Fonts::h2);
    user_list_text->SetForegroundColour(Colors::white);
	user_list_panel = new User_List_Panel(system_page, system);
	system_sizer->Add(user_list_text, 0, wxALL , 5);
	system_sizer->Add(user_list_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Administration_Page::get_all(){
	return system_page;
}

void System_Administration_Page::update(System *system){
	user_list_panel->update(system);
	environment_variable_panel->update(system);
}
