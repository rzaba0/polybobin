#include "paletteframe.hpp"
#include "../constants.hpp"

PaletteFrame::PaletteFrame(wxWindow *parent)
    : MiniFrame(parent, ID_FRAME_PALETTE, wxT("Palette"), wxDefaultPosition, wxDefaultSize)
{
    m_colorPicker = new wxColourPickerCtrl(this, ID_COLOR_PICKER);

    Fit();
}

wxColor PaletteFrame::GetColor()
{
    return m_colorPicker->GetColour();
}