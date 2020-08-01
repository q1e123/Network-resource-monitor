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

	network_management_page = new Network_Management_Page(main_notebook, system);
	main_notebook->AddPage(network_management_page->get_all(), "Network Management");

    box->Add(main_notebook, 1, wxEXPAND);
    main_panel->SetSizer(box);
}

void MainFrame::exit(wxCommandEvent &e){
	Close(TRUE);
}

void MainFrame::real_time(wxTimerEvent &e){
	time_plotting_points.push_back(t);
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

	if (proc.joinable()) {
		proc.join();
		if(main_notebook->GetSelection() == 0){
			system_page->update_process_list(process_sort_type);
		}
	}
	if(main_notebook->GetSelection() == 2){
		if(connected){
			send_update();
			std::string package = client->get_message_recived();
			Recv_Package recv_package(package);
			network_management_page->update_user_cards(recv_package);
		}
	}
}
void MainFrame::shutdown(wxCommandEvent &e){
	system->shutdown();
}

void MainFrame::restart(wxCommandEvent &e){
	system->restart();
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

void MainFrame::send_update(){
	std::string pkg;
	pkg = Network_Package::send_package(system->serilize(), client->get_user());
	client->send_message(pkg);
}
void MainFrame::connect(wxCommandEvent &e){
	connected = true;
	size_t port = network_management_page->get_port();
	string user = network_management_page->get_user();
	string ip = network_management_page->get_ip();
	client = new Client(user, ip, port);
	client->connect_to_server();
	client->start_reciver();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
	std::string server_name = client->get_server_name();
	network_management_page->change_to_connected_gui(server_name);
}
