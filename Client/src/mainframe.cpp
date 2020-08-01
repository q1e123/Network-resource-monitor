#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif 
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <thread>

#include "system.h"
#include "utils.h"
#include "network-package.h"
#include "recv-package.h"
#include "colors.h"
#include "fonts.h"

#include "mainframe.h"

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
	process_sort_type = Process_Sort_Type::NAME;

	init_colors();

	main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	main_panel->SetBackgroundColour(Colors::gray);
	box = new wxBoxSizer(wxVERTICAL);
	timer = new wxTimer(this,TIMER);
	timer->Start(1000);
	
    main_notebook = new wxNotebook(main_panel, wxID_ANY);


	system_page = new System_Page(main_notebook, system);
	main_notebook->AddPage(system_page->get_all(), "System", true);

	performance_page = new Performance_Page(main_notebook, system);
	main_notebook->AddPage(performance_page->get_all(), "Performance");

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
	//check_points();
	std::thread ram(&MainFrame::update_ram, this);
	std::thread cpu(&MainFrame::update_cpu, this);
	std::thread network(&MainFrame::update_network, this);
	std::thread proc(&MainFrame::update_process_list, this);
	
	if(ram.joinable() && cpu.joinable() && network.joinable()){
		ram.join();
		cpu.join();
		network.join();
		performance_page->update_data();
		performance_page->update_gui();
	}
/*
	if (cpu.joinable()) {
		cpu.join();
		size_t i;
		i = 0;
		for (auto item : system->get_cpu_usage()) {
			cpu_plotting_points_Y[i].push_back(item.second);
			if(main_notebook->GetSelection() == 1){
				cpu_usage_texts[i]->SetLabel(item.first + ": " + to_string(item.second).substr(0, to_string(item.second).size() - 4) + "%");
				cpu_plot[i]->SetData(time_plotting_points, cpu_plotting_points_Y[i]);
			}
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
		if(main_notebook->GetSelection() == 1){
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
	}
	t +=0.5;*/

	if (proc.joinable()) {
		proc.join();
		if(main_notebook->GetSelection() == 0){
			system_page->update_process_list(process_sort_type);
		}
	}
	if(main_notebook->GetSelection() == 2){
		if(connected){
			send_update();
			update_user_cards();
		}
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
	if(process_sort_type != Process_Sort_Type::NAME){
		process_sort_type = Process_Sort_Type::NAME;
	}else{
		process_sort_type = Process_Sort_Type::NAME_REVERSE;	
	}
}

void MainFrame::sort_by_pid(wxCommandEvent &e){
	if(process_sort_type != Process_Sort_Type::PID){
		process_sort_type = Process_Sort_Type::PID;
	}else{
		process_sort_type = Process_Sort_Type::PID_REVERSE;	
	}
}

void MainFrame::sort_by_cpu(wxCommandEvent &e){
	if(process_sort_type != Process_Sort_Type::CPU){
		process_sort_type = Process_Sort_Type::CPU;
	}else{
		process_sort_type = Process_Sort_Type::CPU_REVERSE;	
	}
}

void MainFrame::sort_by_ram(wxCommandEvent &e){
	if(process_sort_type != Process_Sort_Type::RAM){
		process_sort_type = Process_Sort_Type::RAM;
	}else{
		process_sort_type = Process_Sort_Type::RAM_REVERSE;	
	}
}

void MainFrame::create_performance_page(){
	/*

	performance_sbox = new wxStaticBoxSizer(performance_static, wxVERTICAL);
	network_text = new wxStaticText(performance_page, wxID_ANY, "Networking");
	network_text->SetFont(Fonts::h2);
	network_text->SetForegroundColour(Colors::white); 
	rx_tx_box = new wxBoxSizer(wxHORIZONTAL); 
	network_rx_text = new wxStaticText(performance_page, wxID_ANY, "In: 0");
	network_rx_text->SetFont(Fonts::normal_bold);
	network_rx_text->SetForegroundColour(Colors::light_green);
	network_tx_text = new wxStaticText(performance_page, wxID_ANY, "Out: 0");
	network_tx_text->SetFont(Fonts::normal_bold);
	network_tx_text->SetForegroundColour(Colors::light_red);

	vector<wxString> choices;
	for(auto inter : system->get_network_interfaces()){
		choices.push_back(inter);
		network_rx_plotting_points_Y[inter] = vector<double>(2,0);
		network_tx_plotting_points_Y[inter] = vector<double>(2,0);
	}
	interface_select_combo = new wxComboBox(performance_page, COMBO_BOX_NETWORK, "Select interface");
	interface_select_combo->Set(choices);
	
	network_plot_window = new mpWindow(performance_page, MP_WINDOW,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	network_plot_window->SetColourTheme(Colors::dark_gray, Colors::light_green, Colors::light_red);
	network_axis_Y = new mpScaleY("",mpALIGN_LEFT);
	network_axis_Y->SetFont(Fonts::normal);
	network_rx_plot = new mpFXYVector();
	network_rx_plot->SetContinuity(true);
	network_rx_plot->SetDrawOutsideMargins(false);
	network_rx_plot->SetData(time_plotting_points, vector<double>(2,0));
	network_rx_plot->SetPen(wxPen(Colors::light_green, 3, wxPENSTYLE_SOLID));
	network_rx_plot->SetDrawOutsideMargins(false);	
	network_tx_plot = new mpFXYVector();
	network_tx_plot->SetContinuity(true);
	network_tx_plot->SetDrawOutsideMargins(false);
	network_tx_plot->SetData(time_plotting_points, vector<double>(2,0));
	network_tx_plot->SetPen(wxPen(Colors::light_red, 3, wxPENSTYLE_SOLID));
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
	performance_page->SetSizerAndFit(perfomance_sizer);*/
}

void MainFrame::create_network_management_page(){
	network_management_page = new wxPanel(main_notebook, wxID_ANY);
	network_management_sizer= new wxBoxSizer(wxVERTICAL);
	network_management_connect_box = new wxBoxSizer(wxHORIZONTAL);

	network_management_user_input = new wxTextCtrl(network_management_page,wxID_ANY,"USER");
	network_management_user_input->SetForegroundColour(Colors::white);
	network_management_ip_input = new wxTextCtrl(network_management_page,wxID_ANY,"127.0.0.1");
	network_management_ip_input->SetForegroundColour(Colors::white);
	network_management_port_input = new wxTextCtrl(network_management_page,wxID_ANY,"50005");
	network_management_port_input->SetForegroundColour(Colors::white);
	network_management_connect_button = new wxButton(network_management_page, BUTTON_CONNECT, "Connect");
	network_management_connect_button->SetForegroundColour(Colors::black);
	network_management_connect_button->SetBackgroundColour(Colors::light_green);
	network_management_network_text = new wxStaticText(network_management_page,wxID_ANY,"");
	network_management_network_text->SetFont(Fonts::h1);
	network_management_network_text->SetForegroundColour(Colors::white);
	network_management_network_text->Hide();

	network_management_connect_box->Add(network_management_user_input, 0, wxALL | wxEXPAND, 5);
	network_management_connect_box->Add(network_management_ip_input, 0, wxALL | wxEXPAND, 5);
	network_management_connect_box->Add(network_management_port_input, 0, wxALL | wxEXPAND, 5);
	network_management_connect_box->Add(network_management_connect_button, 0, wxALL | wxEXPAND, 5);
	
	network_management_sizer->Add(network_management_network_text, 0, wxALL, 5);
	network_management_sizer->Add(network_management_connect_box, 0, wxALL, 5);
	network_management_page->SetSizerAndFit(network_management_sizer);
}

void MainFrame::send_update(){
	std::string pkg;
	pkg = Network_Package::send_package(system->serilize(), client->get_user());
	client->send_message(pkg);
}
void MainFrame::connect(wxCommandEvent &e){
	connected = true;
	size_t port = std::stol(network_management_port_input->GetValue().ToStdString());
	string user = network_management_user_input->GetValue().ToStdString();
	string ip = network_management_ip_input->GetValue().ToStdString();
	client = new Client(user, ip, port);
	client->connect_to_server();
	client->start_reciver();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
	network_management_network_text->SetLabel("Connected to: " + client->get_server_name());
	network_management_network_text->Show();
	network_management_port_input->Hide();
	network_management_user_input->Hide();
	network_management_ip_input->Hide();
	network_management_connect_button->Hide();
	network_management_user_cards_box = new wxBoxSizer(wxVERTICAL);

	network_management_sizer->Add(network_management_user_cards_box, 0, wxALL, 5);

	network_management_page->SetSizerAndFit(network_management_sizer);
}

void MainFrame::update_user_cards(){
	std::string pkg = client->get_message_recived();
	//std::cout<<pkg<<"\n";
	Recv_Package r_pkg(pkg);
	for(auto item : r_pkg.get_user_data()){
		if(user_cards.find(item.first) != user_cards.end()){
			if(!user_cards[item.first].is_active()){
				user_cards[item.first].set_active();
			}
			user_cards[item.first].update(System(item.second));
		}else{
			User_Card uc = User_Card(network_management_page, wxID_ANY, item.first, System(item.second));
			uc.set_active();
			network_management_user_cards_box->Add(uc.get_items(), 1, wxALL | wxEXPAND, 5);
			user_cards[item.first] = uc;
		}
	}
	for(auto inactive : r_pkg.get_inactive_users()){
		user_cards[inactive].set_inactive();
	}
	network_management_page->SetSizerAndFit(network_management_sizer);
}

void MainFrame::init_colors(){
	cpu_colors.push_back(Colors::light_blue);
	cpu_colors.push_back(Colors::light_red);
	cpu_colors.push_back(Colors::light_green);
	cpu_colors.push_back(Colors::light_purple);
	cpu_colors.push_back(Colors::light_pink);
	cpu_colors.push_back(Colors::dark_blue);
	cpu_colors.push_back(Colors::dark_red);
	cpu_colors.push_back(Colors::dark_green);
	cpu_colors.push_back(Colors::dark_purple);
	cpu_colors.push_back(Colors::dark_pink);
}