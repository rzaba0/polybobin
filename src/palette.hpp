#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class Palette: public wxGridSizer
{
    public:
        Palette(wxWindow *parent, unsigned int rows, unsigned int columns, wxString palettePath);
        wxButton *GetItem(unsigned int itemIndex);
        wxColor GetItemColor(unsigned int itemIndex);
        void SetItemColor(unsigned int itemIndex, wxColor newColor);

    private:
        const wxString RGB_DELIMITER = ",";

        wxVector<wxButton*> m_paletteItems;
};

#endif