#ifndef MAINFRAME_H
#define MAINFRAME_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "system.h"
#include <vector>
#include "mathplot.h"
#include <wx/notebook.h>
#include "client.h"
#include <mutex>


#include "system-page.h"
#include "process-sort-type.h"
#include "performance-page.h"
#include "network-management-page.h"
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
	System *system;
	vector<wxColour> cpu_colors;
	Process_Sort_Type process_sort_type;
	wxTimer *timer;
	wxPanel *main_panel;
	wxBoxSizer *box_sizer;
	wxNotebook *main_notebook;
	
	Client *client;
	std::thread worker;	
	std::mutex mtx;

	System_Page *system_page;
	Performance_Page *performance_page;
	Network_Management_Page *network_management_page;
	
	void send_update();
	void check_points();
	void update_ram();
	void update_cpu();
	void update_network();
	void update_process_list();
};

#endif
