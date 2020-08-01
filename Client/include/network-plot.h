#ifndef NETWORK_PLOT_H
#define NETWORK_PLOT_H

#include "generic-plot.h"

#include <cstdio>
#include <string>
#include <map>

#include "network-usage.h"

class Network_Plot : public Generic_Plot{
public:
    Network_Plot();
    Network_Plot(wxNotebookPage *page);

    void add_point(double x, std::map<std::string, Network_Usage> y);
    void shrink();
    void update_gui(std::string interface);
private:
    std::map<std::string, std::vector<double>> y_points_rx, y_points_tx;
	mpFXYVector *rx_plot, *tx_plot;

};

#endif