#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "app.h"
#include "mainframe.h"

IMPLEMENT_APP(App)

bool App::OnInit(){
	MainFrame *main_window = new MainFrame("Siemens", wxDefaultPosition, wxSize(800, 600));
	main_window->Show(true);
	SetTopWindow(main_window);

	return true;
}
