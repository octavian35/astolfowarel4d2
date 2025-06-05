#include "Fonts.h"
#include "../../SDK/SDK.h"

void Font::Create()
{
	Overlay = I::MatSystemSurface->CreateFont();
	MenuText = I::MatSystemSurface->CreateFont();
	ESP = I::MatSystemSurface->CreateFont();

	I::MatSystemSurface->SetFontGlyphSet(Overlay, "Tahoma", 14, 900, 0, 0, FONTFLAG_ANTIALIAS, 0, 0);
	I::MatSystemSurface->SetFontGlyphSet(MenuText, "Tahoma", 12, 550, 0, 0, FONTFLAG_ANTIALIAS, 0, 0);
	I::MatSystemSurface->SetFontGlyphSet(ESP, "Arial", 12, 700, 0, 0, FONTFLAG_DROPSHADOW, 0, 0);
}