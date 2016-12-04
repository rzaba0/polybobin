#include "palette.hpp"
#include <wx/textfile.h>
#include <wx/tokenzr.h>

Palette::Palette(wxWindow *parent, unsigned int rows, unsigned int columns, wxString palettePath)
    : wxGridSizer(rows, columns, 0, 0)
{
    wxTextFile paletteFile;
    paletteFile.Open(palettePath);

    wxString line;
    unsigned int i = 0;
    for (line = paletteFile.GetFirstLine(); !paletteFile.Eof(); line = paletteFile.GetNextLine())
    {
        wxStringTokenizer tokenizer(line, RGB_DELIMITER);
        wxString r = tokenizer.GetNextToken();
        wxString g = tokenizer.GetNextToken();
        wxString b = tokenizer.GetNextToken();

        wxColor color(wxAtoi(r), wxAtoi(g), wxAtoi(b), 255);

        wxButton *item = new wxButton(parent, i++, wxEmptyString, wxDefaultPosition,
                                      wxSize(20, 20), wxBORDER_NONE);
        item->SetBackgroundColour(color);

        Add(item);
        m_paletteItems.push_back(item);
    }
}

wxButton *Palette::GetItem(unsigned int itemIndex)
{
    return m_paletteItems.at(itemIndex);
}

wxColor Palette::GetItemColor(unsigned int itemIndex)
{
    return GetItem(itemIndex)->GetBackgroundColour();
}

void Palette::SetItemColor(unsigned int itemIndex, wxColor newColor)
{
    GetItem(itemIndex)->SetBackgroundColour(newColor);
}