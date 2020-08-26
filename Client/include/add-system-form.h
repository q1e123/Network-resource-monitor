#ifndef ADD_SYSTEM_FORM
#define ADD_SYSTEM_FORM


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "gui-element-id.h"
#include "database-structs.h"

class Add_System_From{
public:
    Add_System_From();
    Add_System_From(wxWindow *parent);
    ~Add_System_From();

    wxStaticBoxSizer* get_all();

    DB_Systems get_db_systems();
    
private:
    wxBoxSizer *sizer, *machine_id_sizer;
    wxStaticBoxSizer *static_box_sizer;
    wxStaticBox *static_box;
    wxButton *submit_button;
    wxTextCtrl *machine_id_input;
	wxStaticText *machine_id_text;
};

#endif