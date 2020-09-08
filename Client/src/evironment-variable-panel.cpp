#include "environment-variable-panel.h"

#include <vector>
#include <algorithm>
#include <string>

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"
#include "my-process.h"
#include "utils.h"

Environment_Variable_Panel::Environment_Variable_Panel(){}

Environment_Variable_Panel::Environment_Variable_Panel(wxNotebookPage *system_page, System *system){
    this->system = system;

    panel = new wxScrolledWindow(system_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "Process List");
	box_sizer = new wxBoxSizer(wxHORIZONTAL);
	value_box_sizer = new wxBoxSizer(wxVERTICAL);
	environment_variable_box_sizer = new wxBoxSizer(wxVERTICAL);

	environment_variable_text = new wxStaticText(panel, wxID_ANY, "---");
	environment_variable_text->SetFont(Fonts::normal);
	environment_variable_text->SetForegroundColour(Colors::white);
	value_text = new wxStaticText(panel, wxID_ANY, "---");
	value_text->SetFont(Fonts::normal);
	value_text->SetForegroundColour(Colors::white);
    
	environment_variable_box_sizer->Add(environment_variable_text, 1, wxALL, 5);
    value_box_sizer->Add(value_text, 1, wxALL, 5);
		
	box_sizer->Add(environment_variable_box_sizer, 1, wxALL, 5);
	box_sizer->Add(value_box_sizer, 1, wxALL, 5);
	
	panel->SetSizer(box_sizer);
	panel->FitInside();
	panel->SetScrollRate(5,5);
}

Environment_Variable_Panel::~Environment_Variable_Panel(){}

wxScrolledWindow* Environment_Variable_Panel::get_all(){
    return panel;
}

void Environment_Variable_Panel::update(){
    std::vector<System_User> user_list = system->get_user_list();
	std::map<std::string, std::string> environment_variables = system->get_environment_variables();
    std::string key, value;
    key = value = "";
    for (auto item : environment_variables) {
        key += item.first + "\n";
		value += item.second + "\n";
    }
    environment_variable_text->SetLabel(key);
    value_text->SetLabel(value);

    panel->FitInside();
}