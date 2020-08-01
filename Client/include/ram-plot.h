#ifndef RAM_PLOT_H
#define RAM_PLOT_H

#include "generic-plot.h"

class Ram_Plot : public Generic_Plot{
public:
    Ram_Plot();
    Ram_Plot(wxNotebookPage *page);

    void add_point(double x, double y);
    void shrink();
    void update_gui();
private:
    std::vector<double> y_points;
    mpFXYVector *plot;

};

#endif