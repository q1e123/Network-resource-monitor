#include "network-performance.h"

#include <string>

Network_Performance::Network_Performance(){}

Network_Performance::Network_Performance(wxNotebookPage *page, System *system){
    this->system = system;
    header_text = new wxStaticText(page, wxID_ANY, "Networking");
	header_text->SetFont(Fonts::h2);
	header_text->SetForegroundColour(Colors::white); 
	rx_tx_box_sizer = new wxBoxSizer(wxHORIZONTAL); 
	rx_text = new wxStaticText(page, wxID_ANY, "In: 0");
	rx_text->SetFont(Fonts::normal_bold);
	rx_text->SetForegroundColour(Colors::light_green);
	tx_text = new wxStaticText(page, wxID_ANY, "Out: 0");
	tx_text->SetFont(Fonts::normal_bold);
	tx_text->SetForegroundColour(Colors::light_red);
	box_sizer = new wxBoxSizer(wxVERTICAL); 

    plot = new Network_Plot(page);

	for(auto interface : system->get_network_interfaces()){
		choices.push_back(interface);
	}
	interface_combo_box = new wxComboBox(page, wxID_ANY, "Select interface");
	interface_combo_box->Set(choices);

	rx_tx_box_sizer->Add(rx_text, 0, wxALL | wxEXPAND, 40);
	rx_tx_box_sizer->Add(tx_text, 0, wxALL | wxEXPAND, 40);
	box_sizer->Add(header_text, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(interface_combo_box, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(rx_tx_box_sizer, 0, wxALL | wxEXPAND, 5);
	box_sizer->Add(plot->get_all(), 0, wxALL | wxEXPAND, 5);
}


void Network_Performance::update_data(double x){
    plot->add_point(x, system->get_network_usage());
    plot->shrink();
}

void Network_Performance::update_gui(){
    std::string interface_choice = std::string(interface_combo_box->GetStringSelection());
    if(interface_choice != ""){
        plot->update_gui(interface_choice);
		double rx, tx;
		rx = system->get_network_usage()[interface_choice].get_rx();
		tx = system->get_network_usage()[interface_choice].get_tx();
        rx_text->SetLabel("IN: " + std::to_string(rx).substr(0, std::to_string(rx).size() - 4));
        tx_text->SetLabel("OUT: " + std::to_string(tx).substr(0, std::to_string(tx).size() - 4));
    }
}

wxBoxSizer* Network_Performance::get_all(){
	return box_sizer;
}