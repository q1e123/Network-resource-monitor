#include "system-management-page.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"

System_Management_Page::System_Management_Page(){};

System_Management_Page::System_Management_Page(wxNotebook *main_notebook){
    page = new wxPanel(main_notebook, wxID_ANY);
	page->SetBackgroundColour(Colors::dark_gray);

	box_sizer= new wxBoxSizer(wxVERTICAL);
    submit_changes_button = new wxButton(page, GUI_ELEMENT_ID::BUTTON_SYSTEM_MANAGEMENT_SUMBIT_CHANGES, "Sumbit changes");
	submit_changes_button->SetForegroundColour(Colors::black);
	submit_changes_button->SetBackgroundColour(Colors::light_green);
   	refresh_button = new wxButton(page, GUI_ELEMENT_ID::BUTTON_SYSTEM_MANAGEMENT_REFRESH, "Refresh");
	refresh_button->SetForegroundColour(Colors::black);
	refresh_button->SetBackgroundColour(Colors::light_green);

	buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	buttons_sizer->Add(submit_changes_button, 0, wxALL, 5);
	buttons_sizer->Add(refresh_button, 0, wxALL, 5);
	
	scrolled_panel = new wxScrolledWindow(page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "System List");
	scrolled_sizer = new wxBoxSizer(wxVERTICAL);
	scrolled_panel->SetSizer(scrolled_sizer);
	scrolled_panel->FitInside();
	scrolled_panel->SetScrollRate(20,20);

	this->add_system_form = new Add_System_From(page);

	box_sizer->Add(add_system_form->get_all(), 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(buttons_sizer, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(scrolled_panel, 1, wxALL | wxEXPAND, 5);
	page->SetSizerAndFit(box_sizer);
}

wxNotebookPage* System_Management_Page::get_all(){
    return page;
}

void System_Management_Page::update_system_cards(std::vector<DB_Systems> systems){
	for(auto system : systems){
		int id = system.id;
		if(system_cards.find(id) == system_cards.end()){
            add(system);
		}else{
			system_cards[id]->update(system);
		}
	}
	scrolled_panel->FitInside();
}

void System_Management_Page::add(DB_Systems db_systems){
    System_Management_Card *smc = new System_Management_Card(scrolled_panel, db_systems);
    scrolled_sizer->Add(smc->get_items(),  0, wxALL | wxEXPAND, 5);
    system_cards[db_systems.id] = smc;
}

std::vector<DB_Systems> System_Management_Page::get_db_systems(){
	std::vector<DB_Systems> system_list;
	for(auto item : system_cards){
		DB_Systems db_system = item.second->get_db_system();
		system_list.push_back(db_system);
	}
	return system_list;
}

DB_Systems System_Management_Page::get_new_system(){
	return this->add_system_form->get_db_systems();
}