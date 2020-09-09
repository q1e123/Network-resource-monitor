#include "system-page.h"

System_General_Page::System_General_Page(){}

System_General_Page::System_General_Page(wxNotebook *main_notebook, System *system){
	this->system = system;
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	
	system_card = new System_Card(system_page, system);

	system_sizer->Add(system_card->get_all(), 0, wxALL | wxEXPAND, 5);
	
	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_General_Page::get_all(){
	return system_page;
}

void System_General_Page::update(){
	system_card->update();
}
