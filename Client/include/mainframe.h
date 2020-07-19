#ifndef MAINFRAME_H
#define MAINFRAME_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "system.h"
#include <vector>
#include <queue> 
#include "mathplot.h"
#include <wx/notebook.h>
#include "client.h"
#include <mutex>
using std::vector;
using std::queue;


enum Sort{
	SORT_NAME,
	SORT_PID,
	SORT_CPU,
	SORT_RAM,
	SORT_NAME_REVERSE,
	SORT_PID_REVERSE,
	SORT_CPU_REVERSE,
	SORT_RAM_REVERSE
};

class MainFrame : public wxFrame{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size); 
	
	void exit(wxCommandEvent &e);
	void real_time(wxTimerEvent &e);
	void shutdown(wxCommandEvent &e);
	void restart(wxCommandEvent &e);
	void sort_by_name(wxCommandEvent &e);
	void sort_by_pid(wxCommandEvent &e);
	void sort_by_cpu(wxCommandEvent &e);
	void sort_by_ram(wxCommandEvent &e);
	void connect(wxCommandEvent &e);

	DECLARE_EVENT_TABLE();

private:
	double t = 1;
	bool connected = false;
	map<string, wxColour> colors;
	map<string, wxFont> fonts;
	System *system;
	vector<wxColour> cpu_colors;
	Sort sort_type;

	wxStaticText *os_text, *total_ram_text, *avalabile_ram_text, *IP_text, *system_text, *performance_text, *used_ram_text, *cpu_title_text, *ram_title_text, *network_text, *network_rx_text, *network_tx_text, *proc_cpu_text, *proc_name_text, *proc_pid_text, *proc_ram_text, *network_management_network_text;
	wxTextCtrl *network_management_user_input, *network_management_port_input;
	vector<wxStaticText*> cpu_usage_texts;
	wxButton *exit_button, *restart_button, *shutdown_button, *proc_name_button, *proc_pid_button, *proc_cpu_button, *proc_ram_button, *network_management_connect_button;
	wxPanel *main_panel;
	wxBoxSizer *box,  *cpus_box, *header_buttons_box, *rx_tx_box, *proc_sizer, *proc_cpu_sizer, *proc_name_sizer, *proc_pid_sizer, *proc_ram_sizer, *network_management_connect_box;
	wxStaticBoxSizer *header_sbox, *system_sbox, *performance_sbox;
	wxTimer *timer;
	wxStaticBox *header_static, *system_static, *performance_static, *network_management_static;

	mpWindow *cpu_plot_window, *ram_plot_window, *network_plot_window;
	mpFXYVector *ram_plot, *network_rx_plot, *network_tx_plot;
	vector<mpFXYVector*> cpu_plot;
	mpScaleY *cpu_axis_Y, *ram_axis_Y, *network_axis_Y;

	vector<double>time_plotting_points, ram_plotting_points_Y;
	vector<vector<double>> cpu_plotting_points_Y; 
	map<string, vector<double>> network_rx_plotting_points_Y,network_tx_plotting_points_Y;

	wxScrolledWindow *process_list_panel;

	wxComboBox *interface_select_combo;

	wxNotebook *main_notebook;
	wxNotebookPage *performance_page, *system_page, *network_management_page;
	
	Client *client;
	std::thread worker;	
	std::mutex mtx;
	void send_update();
	void check_points();
	void update_ram();
	void update_cpu();
	void update_network();
	void update_process_list();
	void init_colors();
	void init_fonts();
	void create_system_page();
	void create_performance_page();
	void create_network_management_page();
	void start_client(std::string user, size_t port);
};


enum{
	BUTTON_EXIT = wxID_HIGHEST +1,
	BUTTON_RESTART,
	BUTTON_SHUTDOWN,
	TIMER,
	MP_WINDOW,
	COMBO_BOX_NETWORK,
	BUTTON_SORT_PROC_NAME,
	BUTTON_SORT_PROC_PID,
	BUTTON_SORT_PROC_CPU,
	BUTTON_SORT_PROC_RAM,
	BUTTON_CONNECT
};

#endif
