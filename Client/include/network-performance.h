#ifndef NETWORK_PERFORMANNCE_H
#define NETWORK_PERFORMANNCE_H

#include <vector>
#include <string>

#include "system.h"
#include "colors.h"
#include "fonts.h"
#include "network-plot.h"


class Network_Performance{
public:
    Network_Performance();
    Network_Performance(wxNotebookPage *page, System *system);
    wxBoxSizer* get_all();

    void update_data(double x);
    void update_gui();
private:
    System *system;
    wxStaticText *header_text, *rx_text, *tx_text;
    wxComboBox *interface_combo_box;
    wxBoxSizer *box_sizer, *rx_tx_box_sizer;
    Network_Plot *plot;
    std::vector<wxString> choices;
};

#endif