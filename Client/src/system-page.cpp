#include "system-page.h"

System_Page::System_Page(){}

System_Page::System_Page(wxNotebook *main_notebook, System *system){
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	
	system_header = new System_Header(system_page);
	system_card = new System_Card(system_page, system);

	system_sizer->Add(system_header->get_all(), 0, wxALL | wxEXPAND, 5);
	system_sizer->Add(system_card->get_all(), 0, wxALL | wxEXPAND, 5);
	
	process_list_panel = new Process_List_Panel(system_page, system);
	system_sizer->Add(process_list_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Page::get_all(){
	return system_page;
}

void System_Page::update_process_list(Process_Sort_Type sort_type){
	process_list_panel->update(sort_type);
}