#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif 
#include "mainframe.h"
#include "system.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>

#define PLOT_SIZE 60

using std::to_string;
using std::cout;
BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
	EVT_TIMER(TIMER, MainFrame::real_time)
	EVT_BUTTON(BUTTON_RESTART, MainFrame::restart)
	EVT_BUTTON(BUTTON_SHUTDOWN, MainFrame::shutdown)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *) NULL, -1, title, pos, size){
	SetAutoLayout(TRUE);
	system = new System();
	wxColour black(0, 0, 0);
	wxColour white(255,255,255);
	wxColour light_blue(128,200,255);
	wxColour dark_blue(2,44,99);
	wxColour light_red(255,138,138);
	wxColour dark_red(171,0,0);
	wxColour light_yellow(246,255,117);
	wxColour dark_yellow(238,255,0);
	wxColour light_green(145,255,160);
	wxColour dark_green(0,161,21);
	wxColour light_purple(198,133,255);
	wxColour dark_purple(107,0,201);
	wxColour light_gray(181,181,181);
	wxColour dark_gray(53,53,53);
	wxColour light_pink(235,135,239);
	wxColour dark_pink(255,0,221);
	wxColour gray(87,87,87);
	

	cpu_colors.push_back(light_blue);
	cpu_colors.push_back(light_red);
	cpu_colors.push_back(light_green);
	cpu_colors.push_back(light_purple);
	cpu_colors.push_back(light_pink);
	cpu_colors.push_back(dark_blue);
	cpu_colors.push_back(dark_red);
	cpu_colors.push_back(dark_green);
	cpu_colors.push_back(dark_purple);
	cpu_colors.push_back(dark_pink);

	wxFont h1(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont h2(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont normal_bold(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont normal(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);

	main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	main_panel->SetBackgroundColour(gray);
	box = new wxBoxSizer(wxVERTICAL);
	timer = new wxTimer(this,TIMER);
	timer->Start(1000);
	
	// Header
	header_static = new wxStaticBox(main_panel, STATIC_BOX,"");
	header_static->SetBackgroundColour(dark_gray);
	IP_text= new wxStaticText(main_panel, TEXT_READONLY, system->get_ip());
	header_buttons_box = new wxBoxSizer(wxHORIZONTAL);
	restart_button = new wxButton(main_panel, BUTTON_RESTART, "Restart");
	restart_button->SetBackgroundColour(light_gray);
	restart_button->SetForegroundColour(black);
	shutdown_button = new wxButton(main_panel, BUTTON_SHUTDOWN, "Shutdown");
	shutdown_button->SetBackgroundColour(light_gray);
	shutdown_button->SetForegroundColour(black);
	header_buttons_box->Add(shutdown_button, 0, wxALL | wxEXPAND, 5);
	header_buttons_box->Add(restart_button, 0, wxALL | wxEXPAND, 5);
	header_sbox = new wxStaticBoxSizer(header_static,wxVERTICAL);
	header_sbox->Add(IP_text, 0, wxALL | wxEXPAND, 10);
	header_sbox->Add(header_buttons_box, 0, wxALL | wxEXPAND, 5);
	
	// System
	system_static = new wxStaticBox(main_panel, STATIC_BOX,"");
	os_text = new wxStaticText(main_panel, TEXT_READONLY,"OS: " + system->get_os());
	os_text->SetForegroundColour(white);
	system_text = new wxStaticText(main_panel, TEXT_READONLY, "System");
	system_text->SetFont(h1);
	system_text->SetForegroundColour(white);
	system_sbox = new wxStaticBoxSizer(system_static, wxVERTICAL);
	system_sbox->Add(system_text, 0, wxALL | wxEXPAND, 5);
	system_sbox->Add(os_text, 0, wxALL | wxEXPAND, 5);

	// Performance
	performance_static = new wxStaticBox(main_panel, STATIC_BOX,"");
	performance_text= new wxStaticText(main_panel, TEXT_READONLY, "Performance");
	performance_text->SetFont(h1);
	performance_text->SetForegroundColour(white);

	ram_title_text = new wxStaticText(main_panel, TEXT_READONLY, "RAM");
	ram_title_text->SetFont(h2);
	ram_title_text->SetForegroundColour(white);
	total_ram_text = new wxStaticText(main_panel, TEXT_READONLY, "Total RAM: " + to_string(system->get_total_ram()));
	total_ram_text->SetForegroundColour(white);
	used_ram_text = new wxStaticText(main_panel, TEXT_READONLY,"Used RAM: " + to_string(system->get_used_ram()));
	used_ram_text->SetForegroundColour(white);
	avalabile_ram_text = new wxStaticText(main_panel, TEXT_READONLY,"Avalabile RAM: " + to_string(system->get_avalabile_ram()));
	avalabile_ram_text->SetForegroundColour(white);
	cpu_title_text = new wxStaticText(main_panel, TEXT_READONLY, "CPU usage");
	cpu_title_text->SetFont(h2);
	cpu_title_text->SetForegroundColour(white);
	cpus_box = new wxBoxSizer(wxHORIZONTAL);
	performance_sbox = new wxStaticBoxSizer(performance_static, wxVERTICAL);
	network_text = new wxStaticText(main_panel, TEXT_READONLY, "Networking");
	network_text->SetFont(h2);
	network_text->SetForegroundColour(white);
	rx_tx_box = new wxBoxSizer(wxHORIZONTAL);
	network_rx_text = new wxStaticText(main_panel, TEXT_READONLY, "In: 0");
	network_rx_text->SetFont(normal_bold);
	network_rx_text->SetForegroundColour(light_green);
	network_tx_text = new wxStaticText(main_panel, TEXT_READONLY, "Out: 0");
	network_tx_text->SetFont(normal_bold);
	network_tx_text->SetForegroundColour(light_red);


	size_t t=0;
	for(auto item:system->get_cpu_usage()){
		wxStaticText *cpu_text = new wxStaticText(main_panel, TEXT_READONLY,item.first + " " + to_string(item.second).substr(0, to_string(item.second).size()-4)+"%");
		cpu_text->SetFont(normal_bold);
		cpu_text->SetForegroundColour(cpu_colors[t++]);
		cpu_usage_texts.push_back(cpu_text);
		cpus_box->Add(cpu_text, 0, wxALL | wxEXPAND, 15);
		cpu_plotting_points_Y.push_back(vector<double>(2,0));
	}
	time_plotting_points.push_back(0);
	time_plotting_points.push_back(0.5);

	ram_plotting_points_Y = vector<double>(2,0);
	ram_plot_window = new mpWindow(main_panel, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	ram_plot_window->SetColourTheme(dark_gray,light_green,light_red);
	ram_axis_Y = new mpScaleY("",mpALIGN_LEFT);
	ram_axis_Y->SetFont(normal);
	ram_plot = new mpFXYVector();
	ram_plot->SetContinuity(true);
	ram_plot->SetDrawOutsideMargins(false);
	ram_plot->SetData(time_plotting_points, ram_plotting_points_Y);
	ram_plot->SetPen(wxPen(light_blue, 3, wxPENSTYLE_SOLID));
	ram_plot->SetDrawOutsideMargins(false);	
	ram_plot_window->AddLayer(ram_axis_Y);
	ram_plot_window->AddLayer(ram_plot);

	cpu_plot_window = new mpWindow(main_panel, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	cpu_plot_window->SetColourTheme(dark_gray,light_green,light_red);
	cpu_axis_Y = new mpScaleY("",mpALIGN_LEFT);
	cpu_axis_Y->SetFont(normal);
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
	interface_select_combo = new wxComboBox(main_panel, COMBO_BOX_NETWORK, "Select interface");
	interface_select_combo->Set(choices);
	
	network_plot_window = new mpWindow(main_panel, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	network_plot_window->SetColourTheme(dark_gray,light_green,light_red);
	network_axis_Y = new mpScaleY("",mpALIGN_LEFT);
	network_axis_Y->SetFont(normal);
	network_rx_plot = new mpFXYVector();
	network_rx_plot->SetContinuity(true);
	network_rx_plot->SetDrawOutsideMargins(false);
	network_rx_plot->SetData(time_plotting_points, vector<double>(2,0));
	network_rx_plot->SetPen(wxPen(light_green, 3, wxPENSTYLE_SOLID));
	network_rx_plot->SetDrawOutsideMargins(false);	
	network_tx_plot = new mpFXYVector();
	network_tx_plot->SetContinuity(true);
	network_tx_plot->SetDrawOutsideMargins(false);
	network_tx_plot->SetData(time_plotting_points, vector<double>(2,0));
	network_tx_plot->SetPen(wxPen(light_red, 3, wxPENSTYLE_SOLID));
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

	box->Add(header_sbox, 0, wxALL | wxEXPAND, 10);
	box->Add(system_sbox, 0, wxALL | wxEXPAND, 10);
	box->Add(performance_sbox,1, wxALL | wxEXPAND, 10 );

	main_panel->SetSizer(box);
}

void MainFrame::exit(wxCommandEvent &e){
	Close(TRUE);
}

void MainFrame::real_time(wxTimerEvent &e){
	system->update_avalabile_ram();
	avalabile_ram_text->SetLabel("Avalabile RAM: " + to_string(system->get_avalabile_ram()));
	used_ram_text->SetLabel("Used RAM: " + to_string(system->get_used_ram()));
	system->update_cpu_usage();
	static double t = 1.0;
	size_t i;
	i = 0;
	time_plotting_points.push_back(t);
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
	for(auto item:system->get_cpu_usage()){
		cpu_usage_texts[i]->SetLabel(item.first + ": " + to_string(item.second).substr(0, to_string(item.second).size()-4)+"%");
		cpu_plotting_points_Y[i].push_back(item.second);
		cpu_plot[i]->SetData(time_plotting_points, cpu_plotting_points_Y[i]);
		++i;
	}
	cpu_plot_window->Fit(double(t-PLOT_SIZE/2), double(t),-5,105);
	double ram_usage;
	ram_usage = double(system->get_used_ram()) / double(system->get_total_ram()) * 100;
	ram_plotting_points_Y.push_back(ram_usage);
	ram_plot->SetData(time_plotting_points, ram_plotting_points_Y); 
	ram_plot_window->Fit(double(t-PLOT_SIZE/2), double(t),-5,105);

	system->update_network_usage();
	for(auto item:system->get_network_usage()){
		network_rx_plotting_points_Y[item.first].push_back(item.second.get_rx());
		network_tx_plotting_points_Y[item.first].push_back(item.second.get_tx());
	}
	double min_y, max_y;
	min_y = max_y = 0;
	if(string(interface_select_combo->GetStringSelection()) != ""){
		vector<double> rx,tx;
		rx = network_rx_plotting_points_Y[string(interface_select_combo->GetStringSelection())];
		tx = network_tx_plotting_points_Y[string(interface_select_combo->GetStringSelection())];

		network_rx_text->SetLabel("IN: " + to_string(rx.back()).substr(0, to_string(rx.back()).size() - 4));
		network_tx_text->SetLabel("OUT: " + to_string(tx.back()).substr(0, to_string(tx.back()).size() - 4));
		network_rx_plot->SetData(time_plotting_points, rx);
		network_tx_plot->SetData(time_plotting_points, tx);

		min_y = std::min(*std::min_element(rx.begin(), rx.end()),*std::min_element(tx.begin(), tx.end()));
		max_y = std::max(*std::max_element(rx.begin(), rx.end()),*std::max_element(tx.begin(), tx.end()));
	}


	network_plot_window->Fit(double(t-PLOT_SIZE/2), double(t), min_y - 5, max_y + 5);

	t +=0.5;
}
void MainFrame::shutdown(wxCommandEvent &e){
	system->shutdown();
}

void MainFrame::restart(wxCommandEvent &e){
	system->restart();
}
