#ifndef GENERIC_PLOT_H
#define GENERIC_PLOT_H

#include <vector>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif 
#include <wx/notebook.h>
#include "mathplot.h"

#define PLOT_SIZE 60

class Generic_Plot{
public:
    Generic_Plot();
    Generic_Plot(wxNotebookPage *page);

    mpWindow *get_plot();
protected:
    std::vector<double> x_points;

    mpWindow *window;
    mpScaleY *y_axis;
};

#endif