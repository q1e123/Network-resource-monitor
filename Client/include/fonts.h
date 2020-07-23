#ifndef FONTS_H
#define FONTS_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace Fonts{
	extern wxFont h1(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	extern wxFont h2(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	extern wxFont normal_bold(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	extern wxFont normal(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
}

#endif
