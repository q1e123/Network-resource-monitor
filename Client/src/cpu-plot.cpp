#include "cpu-plot.h"

Cpu_Plot::Cpu_Plot(){}
Cpu_Plot::Cpu_Plot(wxNotebookPage *page, size_t number_of_cpus, std::vector<wxColour> cpu_colors) : Generic_Plot(page){
    this->number_of_cpus = number_of_cpus;
    this->cpu_colors = cpu_colors;
    for(size_t i = 0; i < number_of_cpus;++i){
		mpFXYVector *tmp = new mpFXYVector();
		std::vector<double> points;
        y_points.push_back(points);
        tmp->SetContinuity(true);
		tmp->SetDrawOutsideMargins(false);
		tmp->SetData(x_points, y_points[i]);
		tmp->SetPen(wxPen(cpu_colors[i], 3, wxPENSTYLE_SOLID));
		tmp->SetDrawOutsideMargins(false);	
		plots.push_back(tmp);
		window->AddLayer(plots[i]);
	}
}

void Cpu_Plot::add_point(double x, std::vector<double> y){
    x_points.push_back(x);
    for(size_t i = 0; i < y.size(); ++i){
        y_points[i].push_back(y[i]);
    }
}

void Cpu_Plot::shrink(){
    if(x_points.size()>PLOT_SIZE-1){
        x_points.erase(x_points.begin());
        for(auto data_set : y_points){
            data_set.erase(data_set.begin());
        }
    }
}


void Cpu_Plot::update_gui(){
    for(size_t i = 0; i < plots.size(); ++i){
        plots[i]->SetData(x_points, y_points[i]);
    }

    window->Fit(double(x_points.back() - PLOT_SIZE / 2), double(x_points.back()), -5, 105);
}