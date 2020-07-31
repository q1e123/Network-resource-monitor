#ifndef PROCESS_LIST_PANEL_H
#define PROCESS_LIST_PANEL_H


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"
#include "process-sort-type.h"

class Process_List_Panel{
public:
    Process_List_Panel();
    Process_List_Panel(wxNotebookPage *system_page, System *system);
    ~Process_List_Panel();

    void update(Process_Sort_Type sort_type);
    wxScrolledWindow* get_all();
private:
    System *system;
    wxScrolledWindow *panel;
    wxStaticText *name_text, *pid_text, *cpu_text, *ram_text;
    wxBoxSizer *box_sizer, *cpu_box_sizer, *name_box_sizer, *pid_box_sizer, *ram_box_sizer;
    wxButton *sort_by_name_button, *sort_by_pid_button, *sort_by_cpu_button, *sort_by_ram_button;
};

#endif