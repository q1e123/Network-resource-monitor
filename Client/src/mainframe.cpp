#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif 
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <thread>

#include "mainframe.h"
#include "system.h"
#include "utils.h"
#include "network-package.h"

#define PLOT_SIZE 60

using std::to_string;
using std::cout;
BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
	EVT_TIMER(TIMER, MainFrame::real_time)
	EVT_BUTTON(BUTTON_RESTART, MainFrame::restart)
	EVT_BUTTON(BUTTON_SHUTDOWN, MainFrame::shutdown)
	EVT_BUTTON(BUTTON_SORT_PROC_NAME, MainFrame::sort_by_name)
	EVT_BUTTON(BUTTON_SORT_PROC_PID, MainFrame::sort_by_pid)
	EVT_BUTTON(BUTTON_SORT_PROC_CPU, MainFrame::sort_by_cpu)
	EVT_BUTTON(BUTTON_SORT_PROC_RAM, MainFrame::sort_by_ram)
	EVT_BUTTON(BUTTON_CONNECT, MainFrame::connect)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *) NULL, -1, title, pos, size){
	SetAutoLayout(TRUE);
	system = new System();
	sort_type = SORT_NAME;
	init_colors();
	init_fonts();

	main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	main_panel->SetBackgroundColour(colors["gray"]);
	box = new wxBoxSizer(wxVERTICAL);
	timer = new wxTimer(this,TIMER);
	timer->Start(1000);
	
    main_notebook = new wxNotebook(main_panel, wxID_ANY);

	create_system_page();
	main_notebook->AddPage(system_page, "System", true);

	create_performance_page();
	main_notebook->AddPage(performance_page, "Performance");

	create_network_management_page();
	main_notebook->AddPage(network_management_page, "Network Management");

    box->Add(main_notebook, 1, wxEXPAND);
    main_panel->SetSizer(box);
}

void MainFrame::exit(wxCommandEvent &e){
	Close(TRUE);
}

void MainFrame::real_time(wxTimerEvent &e){
	time_plotting_points.push_back(t);
	check_points();
	std::thread ram(&MainFrame::update_ram, this);
	std::thread cpu(&MainFrame::update_cpu, this);
	std::thread network(&MainFrame::update_network, this);
	std::thread proc(&MainFrame::update_process_list, this);
	
	if(ram.joinable()){
		ram.join();
		avalabile_ram_text->SetLabel("Avalabile RAM: " + to_string(system->get_avalabile_ram()));
		used_ram_text->SetLabel("Used RAM: " + to_string(system->get_used_ram()));
		double ram_usage;
		ram_usage = double(system->get_used_ram()) / double(system->get_total_ram()) * 100;
		ram_plotting_points_Y.push_back(ram_usage);
		ram_plot->SetData(time_plotting_points, ram_plotting_points_Y);
		ram_plot_window->Fit(double(t - PLOT_SIZE / 2), double(t), -5, 105);
	}

	if (cpu.joinable()) {
		cpu.join();
		size_t i;
		i = 0;
		for (auto item : system->get_cpu_usage()) {
			cpu_usage_texts[i]->SetLabel(item.first + ": " + to_string(item.second).substr(0, to_string(item.second).size() - 4) + "%");
			cpu_plotting_points_Y[i].push_back(item.second);
			cpu_plot[i]->SetData(time_plotting_points, cpu_plotting_points_Y[i]);
			++i;
		}
		cpu_plot_window->Fit(double(t - PLOT_SIZE / 2), double(t), -5, 105);
	}
	
	if (network.joinable()) {
		network.join();
		for (auto item : system->get_network_usage()) {
			network_rx_plotting_points_Y[item.first].push_back(item.second.get_rx());
			network_tx_plotting_points_Y[item.first].push_back(item.second.get_tx());
		}
		double min_y, max_y;
		min_y = max_y = 0;
		if (string(interface_select_combo->GetStringSelection()) != "") {
			vector<double> rx, tx;
			rx = network_rx_plotting_points_Y[string(interface_select_combo->GetStringSelection())];
			tx = network_tx_plotting_points_Y[string(interface_select_combo->GetStringSelection())];

			network_rx_text->SetLabel("IN: " + to_string(rx.back()).substr(0, to_string(rx.back()).size() - 4));
			network_tx_text->SetLabel("OUT: " + to_string(tx.back()).substr(0, to_string(tx.back()).size() - 4));
			network_rx_plot->SetData(time_plotting_points, rx);
			network_tx_plot->SetData(time_plotting_points, tx);

			min_y = std::min(*std::min_element(rx.begin(), rx.end()), *std::min_element(tx.begin(), tx.end()));
			max_y = std::max(*std::max_element(rx.begin(), rx.end()), *std::max_element(tx.begin(), tx.end()));
		}
		network_plot_window->Fit(double(t - PLOT_SIZE / 2), double(t), min_y - 5, max_y + 5);
	}
	
	if (proc.joinable()) {
		proc.join();
		vector<Process> proc_list = system->get_process_list();
		switch(sort_type){
			case SORT_NAME:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_name);
				break;
			case SORT_PID:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_pid);
				break;
			case SORT_CPU:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_cpu);
				break;
			case SORT_RAM:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_ram);
				break;
			case SORT_NAME_REVERSE:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_name_reverse);
				break;
			case SORT_PID_REVERSE:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_pid_reverse);
				break;
			case SORT_CPU_REVERSE:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_cpu_reverse);
				break;
			case SORT_RAM_REVERSE:
				std::sort(proc_list.begin(), proc_list.end(), Proc_Utils::compare_ram_reverse);
				break;
		}

		string name, pid, cpu, ram;
		name = pid = cpu = ram = "";

		for (auto proc : proc_list) {
			string tmp_n, tmp_p, tmp_c, tmp_r;
			tmp_n = proc.get_name();
			tmp_p = to_string(proc.get_pid());
			tmp_c = to_string(utils::round_n(proc.get_cpu_usage(),2)).substr(0, to_string(proc.get_cpu_usage()).size() - 4);
			tmp_r = to_string(proc.get_ram());
			name += tmp_n + "\n";
			pid += tmp_p + "\n";
			cpu += tmp_c + "\n";
			ram += tmp_r + "\n";
		}
		proc_name_text->SetLabel(name);
		proc_pid_text->SetLabel(pid);
		proc_cpu_text->SetLabel(cpu);
		proc_ram_text->SetLabel(ram);
		process_list_panel->FitInside();

	}
	t +=0.5;
	if(connected){
		send_update();
	}
}
void MainFrame::shutdown(wxCommandEvent &e){
	system->shutdown();
}

void MainFrame::restart(wxCommandEvent &e){
	system->restart();
}

void MainFrame::check_points(){
	if(time_plotting_points.size()>PLOT_SIZE-1){
		time_plotting_points.erase(time_plotting_points.begin());
		for(size_t j = 0; j<system->get_cpu_usage().size();++j){
			cpu_plotting_points_Y[j].erase(cpu_plotting_points_Y[j].begin());
		}
		ram_plotting_points_Y.erase(ram_plotting_points_Y.begin());

		for(auto item : network_rx_plotting_points_Y){
			network_rx_plotting_points_Y[item.first].erase(network_rx_plotting_points_Y[item.first].begin());
			network_tx_plotting_points_Y[item.first].erase(network_tx_plotting_points_Y[item.first].begin());
		}
	}
}

void MainFrame::update_ram(){
	system->update_avalabile_ram();

}

void MainFrame::update_cpu(){
	system->update_cpu_usage();
	
}

void MainFrame::update_network(){
	system->update_network_usage();
}

void MainFrame::update_process_list(){
	system->update_process_list();
}

void MainFrame::sort_by_name(wxCommandEvent &e){
	if(sort_type != SORT_NAME){
		sort_type = SORT_NAME;
	}else{
		sort_type = SORT_NAME_REVERSE;	
	}
}

void MainFrame::sort_by_pid(wxCommandEvent &e){
	if(sort_type != SORT_PID){
		sort_type = SORT_PID;
	}else{
		sort_type = SORT_PID_REVERSE;	
	}
}

void MainFrame::sort_by_cpu(wxCommandEvent &e){
	if(sort_type != SORT_CPU){
		sort_type = SORT_CPU;
	}else{
		sort_type = SORT_CPU_REVERSE;	
	}
}

void MainFrame::sort_by_ram(wxCommandEvent &e){
	if(sort_type != SORT_RAM){
		sort_type = SORT_RAM;
	}else{
		sort_type = SORT_RAM_REVERSE;	
	}
}

void MainFrame::init_colors(){
	colors["black"] = wxColour(0, 0, 0);
	colors["white"] = wxColour(255,255,255);
	colors["light_blue"] = wxColour(128,200,255);
	colors["dark_blue"] = wxColour(2,44,99);
	colors["light_red"] = wxColour(255,138,138);
	colors["dark_red"] = wxColour(171,0,0);
	colors["light_yellow"] = wxColour(246,255,117);
	colors["dark_yellow"] = wxColour(238,255,0);
	colors["light_green"] = wxColour(145,255,160);
	colors["dark_green"] = wxColour(0,161,21);
	colors["light_purple"] = wxColour(198,133,255);
	colors["dark_purple"] = wxColour(107,0,201);
	colors["light_gray"] = wxColour(181,181,181);
	colors["dark_gray"] = wxColour(53,53,53);
	colors["light_pink"] = wxColour(235,135,239);
	colors["dark_pink"] = wxColour(255,0,221);
	colors["gray"] = wxColour(87,87,87);

	cpu_colors.push_back(colors["light_blue"]);
	cpu_colors.push_back(colors["light_red"]);
	cpu_colors.push_back(colors["light_green"]);
	cpu_colors.push_back(colors["light_purple"]);
	cpu_colors.push_back(colors["light_pink"]);
	cpu_colors.push_back(colors["dark_blue"]);
	cpu_colors.push_back(colors["dark_red"]);
	cpu_colors.push_back(colors["dark_green"]);
	cpu_colors.push_back(colors["dark_purple"]);
	cpu_colors.push_back(colors["dark_pink"]);
}

void MainFrame::init_fonts(){
	wxFont h1(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont h2(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont normal_bold(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont normal(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);

	fonts["h1"] = h1;
	fonts["h2"] = h2;
	fonts["normal_bold"] = normal_bold;
	fonts["normal"] = normal;
}

void MainFrame::create_system_page(){
	system_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *system_sizer = new wxBoxSizer(wxVERTICAL);
	header_static = new wxStaticBox(system_page, wxID_ANY,"");
	header_static->SetBackgroundColour(colors["dark_gray"]);
	IP_text= new wxStaticText(system_page, wxID_ANY, system->get_ip());
	header_buttons_box = new wxBoxSizer(wxHORIZONTAL);
	restart_button = new wxButton(system_page, BUTTON_RESTART, "Restart");
	restart_button->SetBackgroundColour(colors["light_gray"]);
	restart_button->SetForegroundColour(colors["black"]);
	shutdown_button = new wxButton(system_page, BUTTON_SHUTDOWN, "Shutdown");
	shutdown_button->SetBackgroundColour(colors["light_gray"]);
	shutdown_button->SetForegroundColour(colors["black"]);
	header_buttons_box->Add(shutdown_button, 0, wxALL | wxEXPAND, 5);
	header_buttons_box->Add(restart_button, 0, wxALL | wxEXPAND, 5);
	header_sbox = new wxStaticBoxSizer(header_static,wxVERTICAL);
	header_sbox->Add(IP_text, 0, wxALL | wxEXPAND, 10);
	header_sbox->Add(header_buttons_box, 0, wxALL | wxEXPAND, 5);
	
	system_static = new wxStaticBox(system_page, wxID_ANY,"");
	os_text = new wxStaticText(system_page, wxID_ANY,"OS: " + system->get_os());
	os_text->SetForegroundColour(colors["white"]);
	system_text = new wxStaticText(system_page, wxID_ANY, "System");
	system_text->SetFont(fonts["h1"]);
	system_text->SetForegroundColour(colors["white"]);
	system_sbox = new wxStaticBoxSizer(system_static, wxVERTICAL);
	system_sbox->Add(system_text, 0, wxALL | wxEXPAND, 5);
	system_sbox->Add(os_text, 0, wxALL | wxEXPAND, 5);

	system_sizer->Add(header_sbox, 0, wxALL | wxEXPAND, 5);
	system_sizer->Add(system_sbox, 0, wxALL | wxEXPAND, 5);
	
	process_list_panel = new wxScrolledWindow(system_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL, "Process List");
	proc_sizer = new wxBoxSizer(wxHORIZONTAL);
	proc_cpu_sizer = new wxBoxSizer(wxVERTICAL);
	proc_name_sizer = new wxBoxSizer(wxVERTICAL);
	proc_pid_sizer = new wxBoxSizer(wxVERTICAL);
	proc_ram_sizer = new wxBoxSizer(wxVERTICAL);

	proc_name_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_name_text->SetFont(fonts["normal"]);
	proc_name_text->SetForegroundColour(colors["white"]);
	proc_pid_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_pid_text->SetFont(fonts["normal"]);
	proc_pid_text->SetForegroundColour(colors["white"]);
	proc_cpu_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_cpu_text->SetFont(fonts["normal"]);
	proc_cpu_text->SetForegroundColour(colors["white"]);
	proc_ram_text = new wxStaticText(process_list_panel, wxID_ANY, "---");
	proc_ram_text->SetFont(fonts["normal"]);
	proc_ram_text->SetForegroundColour(colors["white"]);

	proc_name_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_NAME, "Process");
	proc_name_button->SetBackgroundColour(colors["dark_gray"]);
	proc_name_button->SetForegroundColour(colors["light_blue"]);
	proc_pid_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_PID, "PID");
	proc_pid_button->SetBackgroundColour(colors["dark_gray"]);
	proc_pid_button->SetForegroundColour(colors["light_blue"]);
	proc_cpu_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_CPU, "CPU");
	proc_cpu_button->SetBackgroundColour(colors["dark_gray"]);
	proc_cpu_button->SetForegroundColour(colors["light_blue"]);
	proc_ram_button = new wxButton(process_list_panel, BUTTON_SORT_PROC_RAM, "RAM");
	proc_ram_button->SetBackgroundColour(colors["dark_gray"]);
	proc_ram_button->SetForegroundColour(colors["light_blue"]);

	proc_name_sizer->Add(proc_name_button, 0, wxALL, 5);
	proc_pid_sizer->Add(proc_pid_button, 0, wxALL, 5);
	proc_cpu_sizer->Add(proc_cpu_button, 0, wxALL, 5);
	proc_ram_sizer->Add(proc_ram_button, 0, wxALL, 5);
	proc_name_sizer->Add(proc_name_text, 1, wxALL, 5);
	proc_pid_sizer->Add(proc_pid_text, 1, wxALL, 5);
	proc_cpu_sizer->Add(proc_cpu_text, 1, wxALL, 5);
	proc_ram_sizer->Add(proc_ram_text, 1, wxALL, 5);
	
	proc_sizer->Add(proc_name_sizer, 1, wxALL, 0);
	proc_sizer->Add(proc_pid_sizer, 0, wxALL, 0);
	proc_sizer->Add(proc_cpu_sizer, 0, wxALL, 0);
	proc_sizer->Add(proc_ram_sizer, 0, wxALL, 0);

	process_list_panel->SetSizer(proc_sizer);
	process_list_panel->FitInside();
	process_list_panel->SetScrollRate(5,5);

	system_sizer->Add(process_list_panel, 1, wxALL | wxEXPAND, 5);
	system_page->SetSizerAndFit(system_sizer);
}

void MainFrame::create_performance_page(){
	// Performance
	performance_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *perfomance_sizer = new wxBoxSizer(wxVERTICAL);
	performance_static = new wxStaticBox(performance_page, wxID_ANY,"");
	performance_text= new wxStaticText(performance_page, wxID_ANY, "Performance");
	performance_text->SetFont(fonts["h1"]);
	performance_text->SetForegroundColour(colors["white"]);

	ram_title_text = new wxStaticText(performance_page, wxID_ANY, "RAM");
	ram_title_text->SetFont(fonts["h2"]);
	ram_title_text->SetForegroundColour(colors["white"]);
	total_ram_text = new wxStaticText(performance_page, wxID_ANY, "Total RAM: " + to_string(system->get_total_ram()));
	total_ram_text->SetForegroundColour(colors["white"]);
	used_ram_text = new wxStaticText(performance_page, wxID_ANY,"Used RAM: " + to_string(system->get_used_ram()));
	used_ram_text->SetForegroundColour(colors["white"]);
	avalabile_ram_text = new wxStaticText(performance_page, wxID_ANY,"Avalabile RAM: " + to_string(system->get_avalabile_ram()));
	avalabile_ram_text->SetForegroundColour(colors["white"]);
	cpu_title_text = new wxStaticText(performance_page, wxID_ANY, "CPU usage");
	cpu_title_text->SetFont(fonts["h2"]);
	cpu_title_text->SetForegroundColour(colors["white"]);
	cpus_box = new wxBoxSizer(wxHORIZONTAL);
	performance_sbox = new wxStaticBoxSizer(performance_static, wxVERTICAL);
	network_text = new wxStaticText(performance_page, wxID_ANY, "Networking");
	network_text->SetFont(fonts["h2"]);
	network_text->SetForegroundColour(colors["white"]); 
	rx_tx_box = new wxBoxSizer(wxHORIZONTAL); 
	network_rx_text = new wxStaticText(performance_page, wxID_ANY, "In: 0");
	network_rx_text->SetFont(fonts["normal_bold"]);
	network_rx_text->SetForegroundColour(colors["light_green"]);
	network_tx_text = new wxStaticText(performance_page, wxID_ANY, "Out: 0");
	network_tx_text->SetFont(fonts["normal_bold"]);
	network_tx_text->SetForegroundColour(colors["light_red"]);


	size_t t=0;
	for(auto item:system->get_cpu_usage()){
		wxStaticText *cpu_text = new wxStaticText(performance_page, wxID_ANY,item.first + " " + to_string(item.second).substr(0, to_string(item.second).size()-4)+"%");
		cpu_text->SetFont(fonts["normal_bold"]);
		cpu_text->SetForegroundColour(cpu_colors[t++]);
		cpu_usage_texts.push_back(cpu_text);
		cpus_box->Add(cpu_text, 0, wxALL | wxEXPAND, 15);
		cpu_plotting_points_Y.push_back(vector<double>(2,0));
	}
	time_plotting_points.push_back(0);
	time_plotting_points.push_back(0.5);

	ram_plotting_points_Y = vector<double>(2,0);
	ram_plot_window = new mpWindow(performance_page, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	ram_plot_window->SetColourTheme(colors["dark_gray"], colors["light_green"], colors["light_red"]);
	ram_axis_Y = new mpScaleY("",mpALIGN_LEFT);
	ram_axis_Y->SetFont(fonts["normal"]);
	ram_plot = new mpFXYVector();
	ram_plot->SetContinuity(true);
	ram_plot->SetDrawOutsideMargins(false);
	ram_plot->SetData(time_plotting_points, ram_plotting_points_Y);
	ram_plot->SetPen(wxPen(colors["light_blue"], 3, wxPENSTYLE_SOLID));
	ram_plot->SetDrawOutsideMargins(false);	
	ram_plot_window->AddLayer(ram_axis_Y);
	ram_plot_window->AddLayer(ram_plot);

	cpu_plot_window = new mpWindow(performance_page, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	cpu_plot_window->SetColourTheme(colors["dark_gray"], colors["light_green"], colors["light_red"]);
	cpu_axis_Y = new mpScaleY("",mpALIGN_LEFT);
	cpu_axis_Y->SetFont(fonts["normal"]);
	cpu_plot_window->AddLayer(cpu_axis_Y);
	for(size_t i = 0; i < cpu_usage_texts.size();++i){
		mpFXYVector *tmp = new mpFXYVector();
		tmp->SetContinuity(true);
		tmp->SetDrawOutsideMargins(false);
		tmp->SetData(time_plotting_points, cpu_plotting_points_Y[i]);
		tmp->SetPen(wxPen(cpu_colors[i], 3, wxPENSTYLE_SOLID));
		tmp->SetDrawOutsideMargins(false);	
		cpu_plot.push_back(tmp);
		cpu_plot_window->AddLayer(cpu_plot[i]);
	}

	vector<wxString> choices;
	for(auto inter : system->get_network_interfaces()){
		choices.push_back(inter);
		network_rx_plotting_points_Y[inter] = vector<double>(2,0);
		network_tx_plotting_points_Y[inter] = vector<double>(2,0);
	}
	interface_select_combo = new wxComboBox(performance_page, COMBO_BOX_NETWORK, "Select interface");
	interface_select_combo->Set(choices);
	
	network_plot_window = new mpWindow(performance_page, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	network_plot_window->SetColourTheme(colors["dark_gray"], colors["light_green"], colors["light_red"]);
	network_axis_Y = new mpScaleY("",mpALIGN_LEFT);
	network_axis_Y->SetFont(fonts["normal"]);
	network_rx_plot = new mpFXYVector();
	network_rx_plot->SetContinuity(true);
	network_rx_plot->SetDrawOutsideMargins(false);
	network_rx_plot->SetData(time_plotting_points, vector<double>(2,0));
	network_rx_plot->SetPen(wxPen(colors["light_green"], 3, wxPENSTYLE_SOLID));
	network_rx_plot->SetDrawOutsideMargins(false);	
	network_tx_plot = new mpFXYVector();
	network_tx_plot->SetContinuity(true);
	network_tx_plot->SetDrawOutsideMargins(false);
	network_tx_plot->SetData(time_plotting_points, vector<double>(2,0));
	network_tx_plot->SetPen(wxPen(colors["light_red"], 3, wxPENSTYLE_SOLID));
	network_tx_plot->SetDrawOutsideMargins(false);	
	network_plot_window->AddLayer(network_axis_Y);
	network_plot_window->AddLayer(network_rx_plot);
	network_plot_window->AddLayer(network_tx_plot);

	rx_tx_box->Add(network_rx_text,0, wxALL | wxEXPAND,15);
	rx_tx_box->Add(network_tx_text,0, wxALL | wxEXPAND,15);
	
	performance_sbox->Add(performance_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(ram_title_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(total_ram_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(used_ram_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(avalabile_ram_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(ram_plot_window,1, wxBOTTOM | wxEXPAND,50);
	performance_sbox->Add(cpu_title_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(cpus_box, 0, wxALL, 5);
	performance_sbox->Add(cpu_plot_window,1, wxBOTTOM | wxEXPAND,50);
	performance_sbox->Add(network_text,1, wxBOTTOM | wxEXPAND,5);
	performance_sbox->Add(rx_tx_box, 0, wxALL, 5);
	performance_sbox->Add(interface_select_combo, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(network_plot_window,1, wxBOTTOM | wxEXPAND,50);

	perfomance_sizer->Add(performance_sbox, 0, wxALL | wxEXPAND, 5);
	performance_page->SetSizerAndFit(perfomance_sizer);
}

void MainFrame::create_network_management_page(){
	network_management_page = new wxPanel(main_notebook, wxID_ANY);
	wxSizer *network_management_sizer= new wxBoxSizer(wxVERTICAL);
	network_management_connect_box = new wxBoxSizer(wxHORIZONTAL);

	network_management_user_input = new wxTextCtrl(network_management_page,wxID_ANY,"USER");
	network_management_user_input->SetForegroundColour(colors["white"]);
	network_management_port_input = new wxTextCtrl(network_management_page,wxID_ANY,"50005");
	network_management_port_input->SetForegroundColour(colors["white"]);
	network_management_connect_button = new wxButton(network_management_page, BUTTON_CONNECT, "Connect");
	network_management_connect_button->SetForegroundColour(colors["black"]);
	network_management_connect_button->SetBackgroundColour(colors["light_green"]);
	network_management_network_text = new wxStaticText(network_management_page,wxID_ANY,"");
	network_management_network_text->SetFont(fonts["h1"]);
	network_management_network_text->SetForegroundColour(colors["white"]);
	network_management_network_text->Hide();

	network_management_connect_box->Add(network_management_user_input, 0, wxALL | wxEXPAND, 5);
	network_management_connect_box->Add(network_management_port_input, 0, wxALL | wxEXPAND, 5);
	network_management_connect_box->Add(network_management_connect_button, 0, wxALL | wxEXPAND, 5);

	network_management_sizer->Add(network_management_connect_box, 0, wxALL, 5);

	network_management_page->SetSizerAndFit(network_management_sizer);
}

void MainFrame::start_client(std::string user, size_t port){
	mtx.lock();
	client = new Client(user, port);
	mtx.unlock();
}

void MainFrame::send_update(){
	std::string pkg = Network_Package::serilize_system(*(this->system));
	client->msg_send = pkg;
	client->send_msg(pkg);
}
void MainFrame::connect(wxCommandEvent &e){
	connected = true;
	size_t port = std::stol(network_management_port_input->GetValue().ToStdString());
	string user = network_management_user_input->GetValue().ToStdString();
	client = new Client(user, port);
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
	network_management_network_text->SetLabel("Connected to: " + client->msg_rec);
	network_management_network_text->Show();
	network_management_port_input->Hide();
	network_management_user_input->Hide();
	network_management_connect_button->Hide();
}
