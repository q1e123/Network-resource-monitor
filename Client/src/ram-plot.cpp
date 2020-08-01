#include "ram-plot.h"

#include "colors.h"
#include "fonts.h"

Ram_Plot::Ram_Plot(){}
Ram_Plot::Ram_Plot(wxNotebookPage *page) : Generic_Plot(page){
    plot = new mpFXYVector();
	plot->SetContinuity(true);
	plot->SetDrawOutsideMargins(false);
	plot->SetData(x_points, y_points);
	plot->SetPen(wxPen(Colors::light_blue, 3, wxPENSTYLE_SOLID));
	plot->SetDrawOutsideMargins(false);	
	window->AddLayer(plot);
}

void Ram_Plot::add_point(double x, double y){
    x_points.push_back(x);
    y_points.push_back(y);
}

void Ram_Plot::shrink(){
    if(x_points.size()>PLOT_SIZE-1){
        x_points.erase(x_points.begin());
        y_points.erase(y_points.begin());
    }
}

void Ram_Plot::update_gui(){
    plot->SetData(x_points, y_points);
    window->Fit(double(x_points.back() - PLOT_SIZE / 2), double(x_points.back()), -5, 105);
}