#ifndef SYSTEM_CARD_H
#define SYSTEM_CARD_H


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/notebook.h>

#include "system.h"
class System_Card{
public:
    System_Card();
    System_Card(wxNotebookPage *system_page, System *system);
    ~System_Card();

    wxStaticBoxSizer* get_all();

    void update();
private:
    System *system;
    wxStaticBoxSizer *static_box_sizer;
    wxStaticBox *static_box;
    wxStaticText *os_text, *header_text, *machine_id_text, *current_user_text;
    wxStaticText *avalabile_storage_text;
};

#endif