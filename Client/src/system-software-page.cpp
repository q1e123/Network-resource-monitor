#include "system-software-page.h"
#include "gui-element-id.h"
#include "colors.h"

System_Software_Page::System_Software_Page(){}

System_Software_Page::System_Software_Page(wxNotebook *main_notebook, System *system){
    this->system = system;

	system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	
	check_software_button = new wxButton(system_page, GUI_ELEMENT_ID::BUTTON_CHECK_SOFTWARE, "Check");
	check_software_button->SetBackgroundColour(Colors::light_gray);
	check_software_button->SetForegroundColour(Colors::black);
	software_input = new wxTextCtrl(system_page,wxID_ANY,"");

	software_list_panel = new Software_List_Panel(system_page, system);
	system_sizer->Add(software_input, 0, wxALL | wxEXPAND, 5);
	system_sizer->Add(check_software_button, 0, wxALL, 5);
	system_sizer->Add(software_list_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Software_Page::get_all(){
	return system_page;
}

void System_Software_Page::update(System *system){
	software_list_panel->update(system);
}


void System_Software_Page::check_software(){
	std::string software = software_input->GetValue().ToStdString();
	if(system->check_if_installed(software)){
		wxMessageBox(software + " is installed on this machine");
		return;
	}
	wxMessageBox(software + " is not installed on this machine");
}