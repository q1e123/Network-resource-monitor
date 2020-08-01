#include "cpu-performance.h"

#include <string>

Cpu_Performance::Cpu_Performance(){}

Cpu_Performance::Cpu_Performance(wxNotebookPage *page, System *system){
    this->system = system;

    header_text = new wxStaticText(page, wxID_ANY, "CPU usage");
	header_text->SetFont(Fonts::h2);
	header_text->SetForegroundColour(Colors::white);
	usage_box_sizer = new wxBoxSizer(wxHORIZONTAL);
	box_sizer = new wxBoxSizer(wxVERTICAL);
    init_colors();

    plot = new Cpu_Plot(page, system->get_cpu_usage().size(), colors);

	size_t i = 0;
	for(auto item:system->get_cpu_usage()){
		wxStaticText *cpu_text = new wxStaticText(page, wxID_ANY,item.first + " " + std::to_string(item.second).substr(0, std::to_string(item.second).size()-4)+"%");
		cpu_text->SetFont(Fonts::normal_bold);
		cpu_text->SetForegroundColour(colors[i++]);
		usage_texts.push_back(cpu_text);
		usage_box_sizer->Add(cpu_text, 0, wxALL | wxEXPAND, 15);
	}
	
	box_sizer->Add(header_text, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(usage_box_sizer, 0, wxALL | wxEXPAND, 15);
	box_sizer->Add(plot->get_all(), 0, wxALL | wxEXPAND, 15);

}

void Cpu_Performance::init_colors(){
	colors.push_back(Colors::light_blue);
	colors.push_back(Colors::light_red);
	colors.push_back(Colors::light_green);
	colors.push_back(Colors::light_purple);
	colors.push_back(Colors::light_pink);
	colors.push_back(Colors::dark_blue);
	colors.push_back(Colors::dark_red);
	colors.push_back(Colors::dark_green);
	colors.push_back(Colors::dark_purple);
	colors.push_back(Colors::dark_pink);
}

void Cpu_Performance::update_data(double x){
    std::vector<double> y;
    for(auto item:system->get_cpu_usage()){
		y.push_back(item.second);
	}
    plot->add_point(x,y);
    plot->shrink();
}

void Cpu_Performance::update_gui(){
    plot->update_gui();
	size_t i = 0;
    for (auto item : system->get_cpu_usage()) {
        usage_texts[i++]->SetLabel(item.first + ": " + std::to_string(item.second).substr(0, std::to_string(item.second).size() - 4) + "%");
	}
}

wxBoxSizer* Cpu_Performance::get_all(){
	return box_sizer;
}