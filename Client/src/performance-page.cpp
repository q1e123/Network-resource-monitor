#include "performance-page.h"

#include "fonts.h"
#include "colors.h"

Performance_Page::Performance_Page(){}
Performance_Page::Performance_Page(wxNotebook *main_notebook, System *system){
    t = 0;

    this->system = system;
    page = new wxPanel(main_notebook, wxID_ANY);
	box_sizer = new wxBoxSizer(wxVERTICAL);
	static_box = new wxStaticBox(page, wxID_ANY,"");
	header_text= new wxStaticText(page, wxID_ANY, "Performance");
	header_text->SetFont(Fonts::h1);
	header_text->SetForegroundColour(Colors::white);
    
    ram_performance = new Ram_Performance(page, system);
    cpu_performance = new Cpu_Performance(page, system);
    network_performance = new Network_Performance(page, system);

    box_sizer->Add(header_text, 0, wxALL | wxEXPAND, 5);
    box_sizer->Add(ram_performance->get_all(), 0, wxALL | wxEXPAND, 5);
    box_sizer->Add(cpu_performance->get_all(), 0, wxALL | wxEXPAND, 5);
    box_sizer->Add(network_performance->get_all(), 0, wxALL | wxEXPAND, 5);

	page->SetSizerAndFit(box_sizer);
}

void Performance_Page::update_data(){
    ram_performance->update_data(t);
    cpu_performance->update_data(t);
    network_performance->update_data(t);
    t += 0.5;
}

void Performance_Page::update_gui(){
    ram_performance->update_gui();
    cpu_performance->update_gui();
    network_performance->update_gui();
}

wxNotebookPage* Performance_Page::get_all(){
    return page;
}