#ifndef USER_MANAGEMENT_CARD
#define USER_MANAGEMENT_CARD

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
 
#include <string>
#include "database-structs.h"

class User_Management_Card{
public:
	User_Management_Card(wxWindow *parent, DB_Users db_user);
	User_Management_Card();

	wxStaticBoxSizer *get_items();
	
    DB_Users get_db_user();

	void update(DB_Users db_users);

private:
    DB_Users db_user;
	bool active;
	wxBoxSizer *sizer, *machine_id_sizer, *system_id_sizer;
	wxStaticBox *card_static;
	wxStaticBoxSizer *card_sbox;
    wxTextCtrl *machine_id_input, *system_id_input;
    wxCheckBox *user_role_checkbox;
	wxStaticText *id_text, *username_text, *machine_id_text, *system_id_text;
};

#endif
