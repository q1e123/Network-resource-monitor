#include "system-processes-page.h"
#include "colors.h"
System_Processes_Page::System_Processes_Page(){}

System_Processes_Page::System_Processes_Page(wxNotebook *main_notebook, System *system){
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	system_page->SetBackgroundColour(Colors::dark_gray);

	process_list_panel = new Process_List_Panel(system_page, system);
	system_sizer->Add(process_list_panel->get_all(), 1, wxALL | wxEXPAND, 5);

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Processes_Page::get_all(){
	return system_page;
}

void System_Processes_Page::update_process_list(Process_Sort_Type sort_type){
	process_list_panel->update(sort_type);
}