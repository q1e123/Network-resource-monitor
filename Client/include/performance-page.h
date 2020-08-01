#ifndef PERFORMANCE_PAGE_H
#define PERFORMANCE_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

class Performance_Page{
public:
    Performance_Page();
    Performance_Page(wxNotebook *main_notebook, System *system);

    wxNotebookPage* get_all();
    void update_points();
    void update_gui();
private:

}

#endif