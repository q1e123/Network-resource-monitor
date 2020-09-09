#include "real-time-management-card.h"
#include "colors.h"
#include "fonts.h"

Real_Time_Management_Card::Real_Time_Management_Card(wxWindow *parent, System *system){
	sizer = new wxBoxSizer(wxVERTICAL);
	card_static = new wxStaticBox(parent, wxID_ANY, "");
	notebook = new wxNotebook(parent, wxID_ANY);
	
	real_time_performance_page = new Real_Time_Performance_Page(notebook, system);
	notebook->AddPage(real_time_performance_page->get_all(), "Performance", true);

	card_sbox = new wxStaticBoxSizer(card_static, wxVERTICAL);
	card_sbox->Add(notebook, 1, wxALL | wxEXPAND, 5);
}

void Real_Time_Management_Card::update(System *system){
	real_time_performance_page->update(system);
}

void Real_Time_Management_Card::set_active(){
	card_static->SetBackgroundColour(Colors::light_green);
	active = true;
}

void Real_Time_Management_Card::set_inactive(){
	card_static->SetBackgroundColour(Colors::light_red);
	active = false;
}

Real_Time_Management_Card::Real_Time_Management_Card(){
}

wxStaticBoxSizer* Real_Time_Management_Card::get_items(){
	return card_sbox;
}

bool Real_Time_Management_Card::is_active(){
	return active;
}
