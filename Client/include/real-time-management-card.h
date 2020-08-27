#ifndef REAL_TIME_MANAGEMENT_CARD_H
#define REAL_TIME_MANAGEMENT_CARD_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
 
#include <string>
#include "system.h"

class Real_Time_Management_Card{
public:
	Real_Time_Management_Card(wxWindow *parent, System *system);
	Real_Time_Management_Card();
	void set_active();
	void set_inactive();
	wxStaticBoxSizer *get_items();
	void update(System *system);
	bool is_active();
private:
	bool active;
	wxBoxSizer *sizer;
	wxStaticBox *card_static;
	wxStaticBoxSizer *card_sbox;
	wxStaticText *user_text, *os_text, *ram_text, *cpu_text, *machine_id_text;
};

#endif