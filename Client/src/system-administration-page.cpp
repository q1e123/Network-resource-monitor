#include "system-administration-page.h"

System_Administration_Page::System_Administration_Page(){}

System_Administration_Page::System_Administration_Page(wxNotebook *main_notebook, System *system){
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	
	environment_variable_panel = new Environment_Variable_Panel(system_page, system);
	system_sizer->Add(environment_variable_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	user_list_panel = new User_List_Panel(system_page, system);
	system_sizer->Add(user_list_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Administration_Page::get_all(){
	return system_page;
}

void System_Administration_Page::update(){
	user_list_panel->update();
	environment_variable_panel->update();
}