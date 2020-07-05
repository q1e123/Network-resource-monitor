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

using std::vector;
using std::queue;

class MainFrame : public wxFrame{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size); 
	
	void exit(wxCommandEvent &e);
	void real_time(wxTimerEvent &e);
	void shutdown(wxCommandEvent &e);
	void restart(wxCommandEvent &e);

	DECLARE_EVENT_TABLE();

private:
	double t = 1;
	System *system;

	wxStaticText *os_text, *total_ram_text, *avalabile_ram_text, *IP_text, *system_text, *performance_text, *used_ram_text, *cpu_title_text, *ram_title_text, *network_text, *network_rx_text, *network_tx_text;
	vector<wxStaticText*> cpu_usage_texts;
	wxButton *exit_button, *restart_button, *shutdown_button;
	wxPanel *main_panel;
	wxBoxSizer *box,  *cpus_box, *header_buttons_box, *rx_tx_box;
	wxStaticBoxSizer *header_sbox, *system_sbox, *performance_sbox;
	wxTimer *timer;
	wxStaticBox *header_static, *system_static, *performance_static;

	mpWindow *cpu_plot_window, *ram_plot_window, *network_plot_window;
	mpFXYVector *ram_plot, *network_rx_plot, *network_tx_plot;
	vector<mpFXYVector*> cpu_plot;
	mpScaleY *cpu_axis_Y, *ram_axis_Y, *network_axis_Y;

	vector<double>time_plotting_points, ram_plotting_points_Y;
	vector<vector<double>> cpu_plotting_points_Y; 
	map<string, vector<double>> network_rx_plotting_points_Y,network_tx_plotting_points_Y; 

	void check_points();
	void update_ram();
	void update_cpu();
	void update_network();
	
	vector<wxColour> cpu_colors;

	wxComboBox *interface_select_combo;

	wxNotebook *main_notebook;
};

enum{
	BUTTON_EXIT = wxID_HIGHEST +1,
	BUTTON_RESTART,
	BUTTON_SHUTDOWN,
	TEXT_READONLY,
	TIMER,
	MP_WINDOW,
	STATIC_BOX,
	COMBO_BOX_NETWORK
};

#endif
