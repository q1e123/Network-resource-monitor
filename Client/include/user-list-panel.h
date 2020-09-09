#ifndef USER_LIST_PANEL_H
#define USER_LIST_PANEL_H


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"

class User_List_Panel{
public:
    User_List_Panel();
    User_List_Panel(wxNotebookPage *system_page, System *system);
    ~User_List_Panel();

    void update(System *system = nullptr);
    wxScrolledWindow* get_all();
private:
    System *system;
    wxScrolledWindow *panel;
    wxStaticText *user_list_text, *last_log_text;
    wxBoxSizer *box_sizer, *last_log_box_sizer, *user_list_box_sizer;
};

#endif