#ifndef REAL_TIME_PERFORMANCE_PAGE_H
#define REAL_TIME_PERFORMANCE_PAGE_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include <string>
#include "system.h"

class Real_Time_Performance_Page{
public:
	Real_Time_Performance_Page(wxNotebook *main_notebook, System *system);
	Real_Time_Performance_Page();
	wxNotebookPage *get_all();
	void update(System *system);

private:
	wxBoxSizer *sizer;
	wxStaticBox *card_static;
	wxStaticBoxSizer *card_sbox;
    wxNotebookPage *page;
	wxStaticText *ram_total_text, *ram_avalabile_text, *ram_used_text, *cpu_text;
};

#endif
