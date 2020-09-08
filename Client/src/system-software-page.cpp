#include "system-administration-page.h"

System_Administration_Page::System_Administration_Page(){}

System_Administration_Page::System_Administration_Page(wxNotebook *main_notebook, System *system){
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	
	process_list_panel = new Process_List_Panel(system_page, system);
	system_sizer->Add(process_list_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Administration_Page::get_all(){
	return system_page;
}

void System_Administration_Page::update_process_list(Process_Sort_Type sort_type){
	process_list_panel->update(sort_type);
}