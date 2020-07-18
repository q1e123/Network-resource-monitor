#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "app.h"
#include "mainframe.h"

IMPLEMENT_APP(App)

bool App::OnInit(){
	MainFrame *main_win = new MainFrame("Siemens", wxDefaultPosition, wxSize(800, 600));
	main_win->Show(true);
	SetTopWindow(main_win);

	return true;
}
