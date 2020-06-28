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
	shutdown_button = new wxButton(main_panel, BUTTON_SHUTDOWN, "Shutdown");
	header_buttons_box->Add(shutdown_button, 0, wxALL | wxEXPAND, 5);
	header_buttons_box->Add(restart_button, 0, wxALL | wxEXPAND, 5);
	header_sbox = new wxStaticBoxSizer(header_static,wxVERTICAL);
	header_sbox->Add(IP_text, 0, wxALL | wxEXPAND, 10);
	header_sbox->Add(header_buttons_box, 0, wxALL | wxEXPAND, 5);
	
	// System
	system_static = new wxStaticBox(main_panel, STATIC_BOX,"");
	os_text = new wxStaticText(main_panel, TEXT_READONLY,"OS: " + system->get_os());
	system_text = new wxStaticText(main_panel, TEXT_READONLY, "System");
	system_text->SetFont(h1);
	system_sbox = new wxStaticBoxSizer(system_static, wxVERTICAL);
	system_sbox->Add(system_text, 0, wxALL | wxEXPAND, 5);
	system_sbox->Add(os_text, 0, wxALL | wxEXPAND, 5);

	// Performance
	performance_static = new wxStaticBox(main_panel, STATIC_BOX,"");
	performance_text= new wxStaticText(main_panel, TEXT_READONLY, "Performance");
	performance_text->SetFont(h1);
	total_ram_text = new wxStaticText(main_panel, TEXT_READONLY, "Total RAM: " + to_string(system->get_total_ram()));
	used_ram_text = new wxStaticText(main_panel, TEXT_READONLY,"Used RAM: " + to_string(system->get_used_ram()));
	avalabile_ram_text = new wxStaticText(main_panel, TEXT_READONLY,"Avalabile RAM: " + to_string(system->get_avalabile_ram()));
	cpus_box = new wxBoxSizer(wxHORIZONTAL);
	performance_sbox = new wxStaticBoxSizer(performance_static, wxVERTICAL);
	
	size_t t=0;
	for(auto item:system->get_cpu_usage()){
		wxStaticText *cpu_text = new wxStaticText(main_panel, TEXT_READONLY,item.first + " " + to_string(item.second).substr(0, to_string(item.second).size()-4)+"%");
		cpu_text->SetFont(normal_bold);
		cpu_text->SetForegroundColour(cpu_colors[t++]);
		cpu_usage_texts.push_back(cpu_text);
		cpus_box->Add(cpu_text, 0, wxALL | wxEXPAND, 15);
		cpu_plotting_points_Y.push_back(vector<double>(0));
	}
	plot_window = new mpWindow(main_panel, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	plot_window->SetColourTheme(dark_gray,dark_gray,dark_gray);
	cpu_plotting_points_X.push_back(0);
	for(size_t i = 0; i < cpu_usage_texts.size();++i){
		cpu_plotting_points_Y[i].push_back(0);
		mpFXYVector *tmp = new mpFXYVector();
		tmp->SetContinuity(true);
		tmp->SetDrawOutsideMargins(false);
		tmp->SetData(cpu_plotting_points_X, cpu_plotting_points_Y[i]);
		tmp->SetPen(wxPen(cpu_colors[i], 3, wxPENSTYLE_SOLID));
		tmp->SetDrawOutsideMargins(false);	
		cpu_plot.push_back(tmp);
		plot_window->AddLayer(cpu_plot[i]);
	}

	performance_sbox->Add(performance_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(total_ram_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(used_ram_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(avalabile_ram_text, 0, wxALL | wxEXPAND, 5);
	performance_sbox->Add(cpus_box, 0, wxALL, 5);
	performance_sbox->Add(plot_window,1, wxBOTTOM | wxEXPAND,50);

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
	time_t t;
	t = time(NULL);
	size_t i;
	i = 0;
	cpu_plotting_points_X.push_back((double)t);
	for(auto item:system->get_cpu_usage()){
		cpu_usage_texts[i]->SetLabel(item.first + ": " + to_string(item.second).substr(0, to_string(item.second).size()-4)+"%");
		cpu_plotting_points_Y[i].push_back(item.second);
		if(cpu_plotting_points_X.size()>11){
			for(size_t i = 0; i<system->get_cpu_usage().size();++i){
				cpu_plotting_points_Y[i].erase(cpu_plotting_points_Y[i].begin());
			}
			cpu_plotting_points_X.erase(cpu_plotting_points_X.begin());
		}
		cpu_plot[i]->SetData(cpu_plotting_points_X, cpu_plotting_points_Y[i]);
		++i;
	}
	plot_window->Fit((double)t-10,double(t+5),0,100);
}
void MainFrame::shutdown(wxCommandEvent &e){
	system->shutdown();
}

void MainFrame::restart(wxCommandEvent &e){
	system->restart();
}
