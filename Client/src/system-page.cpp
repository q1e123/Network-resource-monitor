#include "system-page.h"

System_Page::System_Page(){}

System_Page::System_Page(wxNotebook *main_notebook, System *system){
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	
	system_header = new System_Header(system_page);
	
	notebook = new wxNotebook(system_page, wxID_ANY);

	system_general_page = new System_General_Page(notebook, system);
	notebook->AddPage(system_general_page->get_all(), "General", true);

	system_administration_page = new System_Administration_Page(notebook, system);
	notebook->AddPage(system_administration_page->get_all(), "Administration", false);

	system_software_page = new System_Software_Page(notebook, system);
	notebook->AddPage(system_software_page->get_all(), "Software", false);

	system_sizer->Add(system_header->get_all(), 0, wxALL | wxEXPAND, 5);
	
	system_sizer->Add(notebook, 1, wxEXPAND);

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Page::get_all(){
	return system_page;
}

void System_Page::update(Process_Sort_Type sort_type){
	system_administration_page->update_process_list(sort_type);
	system_general_page->update();
	system_software_page->update();	
}

void System_Page::check_software(){
	system_software_page->check_software();
}