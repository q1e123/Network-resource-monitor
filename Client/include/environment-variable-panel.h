#ifndef ENVIRONMENT_VARIABLE_PANEL_H
#define ENVIRONMENT_VARIABLES_PANEL_H


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"

class Environment_Variable_Panel{
public:
    Environment_Variable_Panel();
    Environment_Variable_Panel(wxNotebookPage *system_page, System *system);
    ~Environment_Variable_Panel();

    void update();
    wxScrolledWindow* get_all();
private:
    System *system;
    wxScrolledWindow *panel;
    wxStaticText *environment_variable_text, *value_text;
    wxBoxSizer *box_sizer, *environment_variable_box_sizer, *value_box_sizer;
};

#endif