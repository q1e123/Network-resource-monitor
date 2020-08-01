#ifndef CPU_PERFORMANCE_H
#define CPU_PERFORMANCE_H

#include "cpu-plot.h"

#include <vector>

#include "system.h"
#include "colors.h"
#include "fonts.h"

class Cpu_Performance{
public:
    Cpu_Performance();
    Cpu_Performance(wxNotebookPage *page, System *system);
    wxBoxSizer* get_all();

    void update_data(double x);
    void update_gui();
private:
    System *system;
    wxStaticText *header_text;
    std::vector<wxStaticText*> usage_texts;
    wxBoxSizer *box_sizer, *usage_box_sizer;
    Cpu_Plot *plot;
    std::vector<wxColour> colors;
    void init_colors();
};

#endif