#include "generic-plot.h"

#include "colors.h"
#include "fonts.h"

Generic_Plot::Generic_Plot(){}

Generic_Plot::Generic_Plot(wxNotebookPage *page){
    window = new mpWindow(page, wxID_ANY,wxPoint(0,0), wxSize(500,500),wxBORDER_SIMPLE); 
	window->SetColourTheme(Colors::dark_gray, Colors::light_green, Colors::light_red);
	y_axis = new mpScaleY("",mpALIGN_LEFT);
	window->AddLayer(y_axis);
}

mpWindow* Generic_Plot::get_all(){
	return window;
}
