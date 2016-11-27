#ifndef PALETTEFRAME_HPP
#define PALETTEFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/clrpicker.h>
#include "miniframe.hpp"

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
};

#endif