#ifndef USER_CARD_H
#define USER_CARD_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
 
#include <string>
#include "system.h"

class User_Card{
public:
	User_Card(wxWindow *parent, wxWindowID id, System system);

	void set_active();
	void set_inactive();

	void update(System system);
private:
	wxStaticBox *card_static;
	wxStaticBoxSizer *card_sbox;
	wxStaticText *user_text, *os_text, *ram_text, *cpu_text;
	wxColour active_color, inactive_color;
}

#endif
