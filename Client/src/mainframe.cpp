#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif 
#include <string>
#include <ctime>
#include <algorithm>
#include <thread>

#include "system.h"
#include "utils.h"
#include "recv-package.h"

#include "mainframe.h"

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
	box_sizer = new wxBoxSizer(wxVERTICAL);
	timer = new wxTimer(this,TIMER);
	timer->Start(1000);
	
    main_notebook = new wxNotebook(main_panel, wxID_ANY);

	system_page = new System_Page(main_notebook, system);
	main_notebook->AddPage(system_page->get_all(), "System", true);

	performance_page = new Performance_Page(main_notebook, system);
	main_notebook->AddPage(performance_page->get_all(), "Performance");

	network_management_page = new Network_Management_Page(main_notebook, system);
	main_notebook->AddPage(network_management_page->get_all(), "Network Management");

    box_sizer->Add(main_notebook, 1, wxEXPAND);
    main_panel->SetSizer(box_sizer);
}

void MainFrame::exit(wxCommandEvent &e){
	Close(TRUE);
}

void MainFrame::real_time(wxTimerEvent &e){
	std::thread ram_worker(&MainFrame::update_ram, this);
	std::thread cpu_worker(&MainFrame::update_cpu, this);
	std::thread network_worker(&MainFrame::update_network, this);
	std::thread process_list_worker(&MainFrame::update_process_list, this);
	
	if (process_list_worker.joinable()) {
		process_list_worker.join();
		if(main_notebook->GetSelection() == 0){
			system_page->update_process_list(process_sort_type);
		}
	}

	if(ram_worker.joinable() && cpu_worker.joinable() && network_worker.joinable()){
		ram_worker.join();
		cpu_worker.join();
		network_worker.join();
		performance_page->update_data();
		if(main_notebook->GetSelection() == 1){
			performance_page->update_gui();	
		}
	}
	
	if(connected){
		if(main_notebook->GetSelection() == 2){
			client->send_system_state(system);
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

}
void MainFrame::connect(wxCommandEvent &e){
	std::string user = system->get_current_user();
	size_t port = network_management_page->get_port();
	std::string ip = network_management_page->get_ip();
	std::string machine_id = system->get_machine_id();
	client = new Client(user, machine_id, ip, port);
	if(client == nullptr){
		std::cout << "no client\n";
	}
	try{
		client->connect_to_server();
		network_management_page->set_user_role(client->get_role());

	}catch(const Server_Down_Exception& e){
		wxMessageBox("Server down\nVerify if you've put the correct ip and port. If you still get "
						"this error despite those being correct, contact admin.");
		return;
	}catch(const Login_Exception& e){
		wxMessageBox("Login failed\nIf you think you should be able to connect to this network " 
						"conntact admin.");
		return;
	}
	connected = true;
	client->start_reciver();
	
	std::string server_name = client->get_server_name();
	network_management_page->change_to_connected_gui(server_name);
}
