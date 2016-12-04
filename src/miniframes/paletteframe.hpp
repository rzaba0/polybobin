#ifndef PALETTEFRAME_HPP
#define PALETTEFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/clrpicker.h>
#include "miniframe.hpp"
#include "../palette.hpp"

/**
* \brief Mini frame with color picker and palette.
*/
class PaletteFrame: public MiniFrame
{
    public:
        PaletteFrame(wxWindow *parent);

        wxColor GetColor();

    private:
        wxColourPickerCtrl *m_colorPicker;
        wxSlider *m_opacitySlider;
        wxTextCtrl *m_opacityText;
        Palette *m_palette;

        void OnOpacitySliderMoved(wxCommandEvent &event);
        void OnOpacityTextChanged(wxCommandEvent &event);

        void OnPaletteItemLeftMouseButtonClicked(wxMouseEvent &event);
        void OnPaletteItemRightMouseButtonClicked(wxMouseEvent &event);
};

#endif