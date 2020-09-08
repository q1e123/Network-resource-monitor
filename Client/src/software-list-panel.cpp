#include "software-list-panel.h"

#include <vector>
#include <algorithm>
#include <string>

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"
#include "my-process.h"
#include "utils.h"

Software_List_Panel::Software_List_Panel(){}

Software_List_Panel::Software_List_Panel(wxNotebookPage *system_page, System *system){
    this->system = system;

    panel = new wxScrolledWindow(system_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "Process List");
	box_sizer = new wxBoxSizer(wxHORIZONTAL);

	total_installed_software_text = new wxStaticText(panel, wxID_ANY, "Total installed software: ");
	total_installed_software_text->SetFont(Fonts::h2);

	software_list_text = new wxStaticText(panel, wxID_ANY, "");

	box_sizer->Add(total_installed_software_text, 0, wxALL, 0);
	box_sizer->Add(software_list_text, 1, wxALL, 0);

	panel->SetSizer(box_sizer);
	panel->FitInside();
	panel->SetScrollRate(5,5);
}

Software_List_Panel::~Software_List_Panel(){}

wxScrolledWindow* Software_List_Panel::get_all(){
    return panel;
}

void Software_List_Panel::update(){
    std::vector<std::string> software_list = system->get_installed_programs();
	total_installed_software_text->SetLabel("Total installed software: " + std::to_string(software_list.size()));    
    std::sort(software_list.begin(), software_list.end());
    
    std::string softwares = "";
    for(auto software : software_list){
        softwares += software + "\n";
    }

    software_list_text->SetLabel(softwares);
    panel->FitInside();
}