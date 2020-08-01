#ifndef RAM_PERFORMANCE_H
#define RAM_PERFORMANCE_H

#include "ram-plot.h"

#include "system.h"

class Ram_Performance{
public:
    Ram_Performance();
    Ram_Performance(wxNotebookPage *page, System *system);
    wxBoxSizer* get_all();

    void update_data(double x);
    void update_gui();
private:
    System *system;
    wxStaticText *header_text, *total_text, *avalabile_text, *used_text;
    wxBoxSizer *box_sizer;
    Ram_Plot *plot;
};

#endif