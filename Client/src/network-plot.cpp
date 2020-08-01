#include "network-plot.h"

#include "colors.h"
#include "fonts.h"

Network_Plot::Network_Plot(){}
Network_Plot::Network_Plot(wxNotebookPage *page) : Generic_Plot(page){
	rx_plot = new mpFXYVector();
	rx_plot->SetContinuity(true);
	rx_plot->SetDrawOutsideMargins(false);
    std::vector<double> tmp;
	rx_plot->SetData(x_points, tmp);
	rx_plot->SetPen(wxPen(Colors::light_green, 3, wxPENSTYLE_SOLID));
	rx_plot->SetDrawOutsideMargins(false);	
	tx_plot = new mpFXYVector();
	tx_plot->SetContinuity(true);
	tx_plot->SetDrawOutsideMargins(false);
	tx_plot->SetData(x_points, tmp);
	tx_plot->SetPen(wxPen(Colors::light_red, 3, wxPENSTYLE_SOLID));
	tx_plot->SetDrawOutsideMargins(false);	
	window->AddLayer(rx_plot);
	window->AddLayer(tx_plot);
}

void Network_Plot::add_point(double x, std::map<std::string, Network_Usage> y){
    x_points.push_back(x);
    for (auto item : y) {
        y_points_rx[item.first].push_back(y[item.first].get_rx());
        y_points_tx[item.first].push_back(y[item.first].get_tx());
    }
}

void Network_Plot::shrink(){
    if(x_points.size()>PLOT_SIZE-1){
        x_points.erase(x_points.begin());
        for (auto item : y_points_rx) {
            y_points_rx[item.first].erase(y_points_rx[item.first].begin());
            y_points_tx[item.first].erase(y_points_tx[item.first].begin());
        }
    }
}


void Network_Plot::update_gui(std::string interface){
    if(interface != ""){
        auto rx = y_points_rx[interface];
        auto tx = y_points_tx[interface];
        rx_plot->SetData(x_points, rx);
        tx_plot->SetData(x_points, tx);
        double min_y = std::min(*std::min_element(rx.begin(), rx.end()), *std::min_element(tx.begin(), tx.end()));
        double max_y = std::max(*std::max_element(rx.begin(), rx.end()), *std::max_element(tx.begin(), tx.end()));
        window->Fit(double(x_points.back()- PLOT_SIZE / 2), double(x_points.back()), min_y - 5, max_y + 5);
    }
}