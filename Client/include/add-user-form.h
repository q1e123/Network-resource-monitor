#ifndef ADD_USER_FORM
#define ADD_USER_FORM


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "gui-element-id.h"
#include "database-structs.h"

class Add_User_From{
public:
    Add_User_From();
    Add_User_From(wxWindow *parent);
    ~Add_User_From();

    wxStaticBoxSizer* get_all();

    DB_Users get_db_user();
    
private:
    wxBoxSizer *sizer, *system_id_sizer, *username_sizer;
    wxStaticBoxSizer *static_box_sizer;
    wxStaticBox *static_box;
    wxButton *submit_button;
    wxTextCtrl *system_id_input, *username_input;
    wxCheckBox *user_role_checkbox;
	wxStaticText *id_text, *username_text, *system_id_text;
};

#endif