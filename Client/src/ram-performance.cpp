#include "ram-performance.h"

#include <string>

#include "colors.h"
#include "fonts.h"

Ram_Performance::Ram_Performance(){}

Ram_Performance::Ram_Performance(wxNotebookPage *page, System *system){
    this->system = system;
    header_text = new wxStaticText(page, wxID_ANY, "RAM");
	header_text->SetFont(Fonts::h2);
	header_text->SetForegroundColour(Colors::white);
	total_text = new wxStaticText(page, wxID_ANY, "Total RAM: " + std::to_string(system->get_total_ram()));
	total_text->SetForegroundColour(Colors::white);
	used_text = new wxStaticText(page, wxID_ANY,"Used RAM: " + std::to_string(system->get_used_ram()));
	used_text->SetForegroundColour(Colors::white);
	avalabile_text = new wxStaticText(page, wxID_ANY,"Avalabile RAM: " + std::to_string(system->get_avalabile_ram()));
	avalabile_text->SetForegroundColour(Colors::white);
    box_sizer = new wxBoxSizer(wxVERTICAL);

    plot = new Ram_Plot(page);

	box_sizer->Add(header_text, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(total_text, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(used_text, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(avalabile_text, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(plot->get_all(), 0, wxALL | wxEXPAND, 5);
}

wxBoxSizer* Ram_Performance::get_all(){
    return box_sizer;
}

void Ram_Performance::update_data(double x){
    double ram_usage;
    ram_usage = double(system->get_used_ram()) / double(system->get_total_ram()) * 100;
    plot->add_point(x,ram_usage);
	plot->shrink();
}

void Ram_Performance::update_gui(){
    avalabile_text->SetLabel("Avalabile RAM: " + std::to_string(system->get_avalabile_ram()));
    used_text->SetLabel("Used RAM: " + std::to_string(system->get_used_ram()));
    plot->update_gui();
}