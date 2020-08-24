#ifndef SYSTEM_MANAGEMENT_CARD
#define SYSTEM_MANAGEMENT_CARD

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
 
#include <string>
#include "database-structs.h"

class System_Management_Card{
public:
	System_Management_Card(wxWindow *parent, DB_Systems db_systems);
	System_Management_Card();

	wxStaticBoxSizer *get_items();
	
    DB_Systems get_db_system();

	void update(DB_Systems db_systems);
	void turn_on();
	void turn_off();

private:
    DB_Systems db_system;
	wxBoxSizer *sizer, *machine_id_sizer;
	wxStaticBox *card_static;
	wxStaticBoxSizer *card_sbox;
    wxTextCtrl *machine_id_input;
	wxStaticText *id_text, *system_status, *machine_id_text;
};

#endif
