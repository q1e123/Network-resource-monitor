#ifndef PERFORMANCE_PAGE_H
#define PERFORMANCE_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "ram-performance.h"
#include "cpu-performance.h"
#include "system.h"

class Performance_Page{
public:
    Performance_Page();
    Performance_Page(wxNotebook *main_notebook, System *system);

    wxNotebookPage* get_all();
    void update_data();
    void update_gui();
private:
    System *system;
    wxNotebookPage *page;
    wxBoxSizer *box_sizer;
    wxStaticBox *static_box;
    wxStaticText *header_text;
    Ram_Performance *ram_performance;
    Cpu_Performance *cpu_performance;
    double t;

};

#endif