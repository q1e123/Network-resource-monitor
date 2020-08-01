#ifndef CPU_PLOT_H
#define CPU_PLOT_H

#include "generic-plot.h"

#include <cstdio>

class Cpu_Plot : public Generic_Plot{
public:
    Cpu_Plot();
    Cpu_Plot(wxNotebookPage *page, size_t number_of_cpus, std::vector<wxColour> cpu_colors);

    void add_point(double x, std::vector<double> y);
    void shrink();
    void update_gui();
private:
    size_t number_of_cpus;
    std::vector<wxColour> cpu_colors
	std::vector<std::vector<double>> y_points; 
	std::vector<mpFXYVector*> plots;

};

#endif