#include "real-time-performance-page.h"
#include "colors.h"
#include "fonts.h"

Real_Time_Performance_Page::Real_Time_Performance_Page(wxNotebook *parent, System *system){
	sizer = new wxBoxSizer(wxVERTICAL);
	page = new wxPanel(parent, wxID_ANY);
	page->SetBackgroundColour(Colors::dark_gray);

	std::string cpu = "";
	for(auto item : system->get_cpu_usage()){
		cpu+= item.first + ": " + std::to_string(item.second) + "%\t";
	}

	ram_total_text = new wxStaticText(page, wxID_ANY, "Total ram: " + std::to_string(system->get_total_ram()));
	ram_total_text->SetForegroundColour(Colors::white);
	ram_total_text->SetFont(Fonts::normal);
	ram_avalabile_text = new wxStaticText(page, wxID_ANY, "Avalabile ram: " + std::to_string(system->get_avalabile_ram()));
	ram_avalabile_text->SetForegroundColour(Colors::white);
	ram_avalabile_text->SetFont(Fonts::normal);
	ram_used_text = new wxStaticText(page, wxID_ANY, "Used ram: " + std::to_string(system->get_used_ram()));
	ram_used_text->SetForegroundColour(Colors::white);
	ram_used_text->SetFont(Fonts::normal);
	cpu_text = new wxStaticText(page, wxID_ANY, cpu); 
	cpu_text->SetForegroundColour(Colors::white);
	cpu_text->SetFont(Fonts::normal);

	sizer->Add(ram_total_text, 1, wxALL | wxEXPAND, 5);
	sizer->Add(ram_avalabile_text, 1, wxALL | wxEXPAND, 5);
	sizer->Add(ram_used_text, 1, wxALL | wxEXPAND, 5);
	sizer->Add(cpu_text, 1, wxALL | wxEXPAND, 5);

	page->SetSizerAndFit(sizer);
}

void Real_Time_Performance_Page::update(System *system){
	std::string ram = "";
	ram += "Total ram: " + std::to_string(system->get_total_ram()) + "\t";
	ram += "Avalabile ram: " + std::to_string(system->get_avalabile_ram()) + "\t";
	ram += "Used ram: " + std::to_string(system->get_used_ram());

	std::string cpu = "";
	for(auto item : system->get_cpu_usage()){
		cpu+= item.first + ": " + std::to_string(item.second) + "%\t";
	}
	ram_total_text->SetLabel("Total ram: " + std::to_string(system->get_total_ram()));
	ram_avalabile_text->SetLabel("Avalabile ram: " + std::to_string(system->get_avalabile_ram()));
	ram_used_text->SetLabel("Used ram: " + std::to_string(system->get_used_ram()));

	cpu_text->SetLabel(cpu);
}


Real_Time_Performance_Page::Real_Time_Performance_Page(){
}

wxNotebookPage* Real_Time_Performance_Page::get_all(){
	return page;
}