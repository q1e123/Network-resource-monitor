#include "process-list-panel.h"

#include <vector>
#include <algorithm>
#include <string>

#include "colors.h"
#include "fonts.h"
#include "gui-element-id.h"
#include "my-process.h"
#include "utils.h"

Process_List_Panel::Process_List_Panel(){}

Process_List_Panel::Process_List_Panel(wxNotebookPage *system_page, System *system){
    this->system = system;

    panel = new wxScrolledWindow(system_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "Process List");
	box_sizer = new wxBoxSizer(wxHORIZONTAL);
	cpu_box_sizer = new wxBoxSizer(wxVERTICAL);
	name_box_sizer = new wxBoxSizer(wxVERTICAL);
	pid_box_sizer = new wxBoxSizer(wxVERTICAL);
	ram_box_sizer = new wxBoxSizer(wxVERTICAL);

	name_text = new wxStaticText(panel, wxID_ANY, "---");
	name_text->SetFont(Fonts::normal);
	name_text->SetForegroundColour(Colors::white);
	pid_text = new wxStaticText(panel, wxID_ANY, "---");
	pid_text->SetFont(Fonts::normal);
	pid_text->SetForegroundColour(Colors::white);
	cpu_text = new wxStaticText(panel, wxID_ANY, "---");
	cpu_text->SetFont(Fonts::normal);
	cpu_text->SetForegroundColour(Colors::white);
	ram_text = new wxStaticText(panel, wxID_ANY, "---");
	ram_text->SetFont(Fonts::normal);
	ram_text->SetForegroundColour(Colors::white);

	sort_by_name_button = new wxButton(panel, GUI_ELEMENT_ID::BUTTON_SORT_PROC_NAME, "Process");
	sort_by_name_button->SetBackgroundColour(Colors::dark_gray);
	sort_by_name_button->SetForegroundColour(Colors::light_blue);
	sort_by_pid_button = new wxButton(panel, GUI_ELEMENT_ID::BUTTON_SORT_PROC_PID, "PID");
	sort_by_pid_button->SetBackgroundColour(Colors::dark_gray);
	sort_by_pid_button->SetForegroundColour(Colors::light_blue);
	sort_by_cpu_button = new wxButton(panel, GUI_ELEMENT_ID::BUTTON_SORT_PROC_CPU, "CPU");
	sort_by_cpu_button->SetBackgroundColour(Colors::dark_gray);
	sort_by_cpu_button->SetForegroundColour(Colors::light_blue);
	sort_by_ram_button = new wxButton(panel, GUI_ELEMENT_ID::BUTTON_SORT_PROC_RAM, "RAM");
	sort_by_ram_button->SetBackgroundColour(Colors::dark_gray);
	sort_by_ram_button->SetForegroundColour(Colors::light_blue);

	name_box_sizer->Add(sort_by_name_button, 0, wxALL, 5);
	pid_box_sizer->Add(sort_by_pid_button, 0, wxALL, 5);
	cpu_box_sizer->Add(sort_by_cpu_button, 0, wxALL, 5);
	ram_box_sizer->Add(sort_by_ram_button, 0, wxALL, 5);
	name_box_sizer->Add(name_text, 1, wxALL, 5);
	pid_box_sizer->Add(pid_text, 1, wxALL, 5);
	cpu_box_sizer->Add(cpu_text, 1, wxALL, 5);
	ram_box_sizer->Add(ram_text, 1, wxALL, 5);
	
	box_sizer->Add(name_box_sizer, 1, wxALL, 0);
	box_sizer->Add(pid_box_sizer, 0, wxALL, 0);
	box_sizer->Add(cpu_box_sizer, 0, wxALL, 0);
	box_sizer->Add(ram_box_sizer, 0, wxALL, 0);

	panel->SetSizer(box_sizer);
	panel->FitInside();
	panel->SetScrollRate(5,5);
}

Process_List_Panel::~Process_List_Panel(){}

wxScrolledWindow* Process_List_Panel::get_all(){
    return panel;
}

void Process_List_Panel::update(Process_Sort_Type sort_type){
    std::vector<Process> process_list = system->get_process_list();
    switch(sort_type){
        case Process_Sort_Type::NAME:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_name);
            break;
        case Process_Sort_Type::PID:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_pid);
            break;
        case Process_Sort_Type::CPU:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_cpu);
            break;
        case Process_Sort_Type::RAM:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_ram);
            break;
        case Process_Sort_Type::NAME_REVERSE:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_name_reverse);
            break;
        case Process_Sort_Type::PID_REVERSE:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_pid_reverse);
            break;
        case Process_Sort_Type::CPU_REVERSE:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_cpu_reverse);
            break;
        case RAM_REVERSE:
            std::sort(process_list.begin(), process_list.end(), Proc_Utils::compare_ram_reverse);
            break;
    }
    std::string name, pid, cpu, ram;
    name = pid = cpu = ram = "";

    for (auto proc : process_list) {
        std::string tmp_n, tmp_p, tmp_c, tmp_r;
        tmp_n = proc.get_name();
        tmp_p = std::to_string(proc.get_pid());
        tmp_c = std::to_string(utils::round_n(proc.get_cpu_usage(),2)).substr(0, std::to_string(proc.get_cpu_usage()).size() - 4);
        tmp_r = std::to_string(proc.get_ram());
        name += tmp_n + "\n";
        pid += tmp_p + "\n";
        cpu += tmp_c + "\n";
        ram += tmp_r + "\n";
    }
    name_text->SetLabel(name);
    pid_text->SetLabel(pid);
    cpu_text->SetLabel(cpu);
    ram_text->SetLabel(ram);
    panel->FitInside();
}