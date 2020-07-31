#include "system-page.h"

System_Page::System_Page(){}

System_Page::System_Page(wxNotebook *main_notebook, System *system){
    system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	
	system_header = new System_Header(system_page);
	system_card = new System_Card(system_page, system);

	system_sizer->Add(system_header->get_all(), 0, wxALL | wxEXPAND, 5);
	system_sizer->Add(system_card->get_all(), 0, wxALL | wxEXPAND, 5);
	/*
	process_list_panel = new wxScrolledWindow(system_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "Process List");
	proc_sizer = new wxBoxSizer(wxHORIZONTAL);
	proc_cpu_sizer = new wxBoxSizer(wxVERTICAL);
	proc_name_sizer = new wxBoxSizer(wxVERTICAL);
	proc_pid_sizer = new wxBoxSizer(wxVERTICAL);
	proc_ram_sizer = new wxBoxSizer(wxVERTICAL);

	proc_name_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_name_text->SetFont(Fonts::normal);
	proc_name_text->SetForegroundColour(Colors::white);
	proc_pid_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_pid_text->SetFont(Fonts::normal);
	proc_pid_text->SetForegroundColour(Colors::white);
	proc_cpu_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_cpu_text->SetFont(Fonts::normal);
	proc_cpu_text->SetForegroundColour(Colors::white);
	proc_ram_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_ram_text->SetFont(Fonts::normal);
	proc_ram_text->SetForegroundColour(Colors::white);

	proc_name_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_NAME, "Process");
	proc_name_button->SetBackgroundColour(Colors::dark_gray);
	proc_name_button->SetForegroundColour(Colors::light_blue);
	proc_pid_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_PID, "PID");
	proc_pid_button->SetBackgroundColour(Colors::dark_gray);
	proc_pid_button->SetForegroundColour(Colors::light_blue);
	proc_cpu_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_CPU, "CPU");
	proc_cpu_button->SetBackgroundColour(Colors::dark_gray);
	proc_cpu_button->SetForegroundColour(Colors::light_blue);
	proc_ram_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_RAM, "RAM");
	proc_ram_button->SetBackgroundColour(Colors::dark_gray);
	proc_ram_button->SetForegroundColour(Colors::light_blue);

	proc_name_sizer->Add(proc_name_button, 0, wxALL, 5);
	proc_pid_sizer->Add(proc_pid_button, 0, wxALL, 5);
	proc_cpu_sizer->Add(proc_cpu_button, 0, wxALL, 5);
	proc_ram_sizer->Add(proc_ram_button, 0, wxALL, 5);
	proc_name_sizer->Add(proc_name_text, 1, wxALL, 5);
	proc_pid_sizer->Add(proc_pid_text, 1, wxALL, 5);
	proc_cpu_sizer->Add(proc_cpu_text, 1, wxALL, 5);
	proc_ram_sizer->Add(proc_ram_text, 1, wxALL, 5);
	
	proc_sizer->Add(proc_name_sizer, 1, wxALL, 0);
	proc_sizer->Add(proc_pid_sizer, 0, wxALL, 0);
	proc_sizer->Add(proc_cpu_sizer, 0, wxALL, 0);
	proc_sizer->Add(proc_ram_sizer, 0, wxALL, 0);

	process_list_panel->SetSizer(proc_sizer);
	process_list_panel->FitInside();
	process_list_panel->SetScrollRate(5,5);
	system_sizer->Add(process_list_panel, 1, wxALL | wxEXPAND, 5);*/

	system_page->SetSizerAndFit(system_sizer);
}

wxNotebookPage* System_Page::get_all(){
	return system_page;
}