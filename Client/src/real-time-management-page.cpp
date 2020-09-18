#include "real-time-management-page.h"

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"

Real_Time_Management_Page::Real_Time_Management_Page(){};

Real_Time_Management_Page::Real_Time_Management_Page(wxNotebook *main_notebook){
    page = new wxPanel(main_notebook, wxID_ANY);

	box_sizer= new wxBoxSizer(wxVERTICAL);
	
	scrolled_panel = new wxScrolledWindow(page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "User List");
	scrolled_sizer = new wxBoxSizer(wxVERTICAL);
	scrolled_panel->SetSizer(scrolled_sizer);
	scrolled_panel->FitInside();
	scrolled_panel->SetScrollRate(20,20);

	box_sizer->Add(scrolled_panel, 1, wxALL | wxEXPAND, 5);
	page->SetSizerAndFit(box_sizer);
}

wxNotebookPage* Real_Time_Management_Page::get_all(){
    return page;
}

void Real_Time_Management_Page::update_real_time_cards(std::vector<System*> active_systems, std::vector<std::string> inactives){
	for(auto system : active_systems){
		std::string id = system->get_current_user();
		std::cout << id << std::endl;
		if(real_time_cards.find(id) != real_time_cards.end()){
			real_time_cards[id]->set_active();
			real_time_cards[id]->update(system);
		}else{
			Real_Time_Management_Card *rt = new Real_Time_Management_Card(scrolled_panel, system);
			rt->set_active();
			scrolled_sizer->Add(rt->get_items(), 0, wxALL | wxEXPAND, 5);
			real_time_cards[id] = rt;
		}
	}
	for(auto id : inactives){
		if(real_time_cards.find(id) != real_time_cards.end()){
			real_time_cards[id]->set_inactive();
		}
	}

	scrolled_panel->FitInside();
}