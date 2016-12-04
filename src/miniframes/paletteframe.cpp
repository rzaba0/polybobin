#include "paletteframe.hpp"
#include "../constants.hpp"
#include <wx/valnum.h>

PaletteFrame::PaletteFrame(wxWindow *parent)
    : MiniFrame(parent, ID_FRAME_PALETTE, wxT("Palette"), wxDefaultPosition, wxDefaultSize)
{
    m_colorPicker = new wxColourPickerCtrl(this, ID_COLOR_PICKER);

    wxStaticBoxSizer *opacitySizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Opacity");
    m_opacitySlider = new wxSlider(this, wxID_ANY, 100, 0, 100);
    Bind(wxEVT_SLIDER, &PaletteFrame::OnOpacitySliderMoved, this);

    wxIntegerValidator<int> opacityTextValidator;
    opacityTextValidator.SetMin(0);
    opacityTextValidator.SetMax(100);
    m_opacityText = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition,
        wxSize(40, 20), 0L, opacityTextValidator);
    Bind(wxEVT_TEXT, &PaletteFrame::OnOpacityTextChanged, this);

    opacitySizer->Add(m_opacitySlider, 1, wxEXPAND);
    opacitySizer->Add(m_opacityText);

    const unsigned int PALETTE_WIDTH = 12,
                       PALETTE_HEIGHT = 6;
    m_palette = new Palette(this, PALETTE_HEIGHT, PALETTE_WIDTH, PATH_PALETTES_DIRECTORY + "default.txt");
    for (unsigned int i = 0; i < m_palette->GetItemCount(); ++i)
    {
        m_palette->GetItem(i)->Bind(wxEVT_LEFT_DOWN, &PaletteFrame::OnPaletteItemLeftMouseButtonClicked, this);
        m_palette->GetItem(i)->Bind(wxEVT_RIGHT_DOWN, &PaletteFrame::OnPaletteItemRightMouseButtonClicked, this);
    }

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_colorPicker, 1, wxALIGN_CENTER_HORIZONTAL);
    sizer->AddSpacer(20);
    sizer->Add(opacitySizer, 0, wxEXPAND);
    sizer->AddSpacer(20);
    sizer->Add(m_palette);

    SetSizerAndFit(sizer);
}

wxColor PaletteFrame::GetColor()
{
    wxColor color = m_colorPicker->GetColour();
    int opacity = m_opacitySlider->GetValue();

    unsigned char alpha = (opacity * 255) / 100;
    return wxColor(color.Red(), color.Green(), color.Blue(), alpha);
}

void PaletteFrame::OnOpacitySliderMoved(wxCommandEvent &event)
{
    int newValue = event.GetSelection();
    m_opacityText->ChangeValue(wxString::Format("%d", newValue));
}

void PaletteFrame::OnOpacityTextChanged(wxCommandEvent &event)
{
    wxString newText = event.GetString();
    long newValue;
    newText.ToLong(&newValue);
    m_opacitySlider->SetValue(newValue);
}

void PaletteFrame::OnPaletteItemLeftMouseButtonClicked(wxMouseEvent &event)
{
    m_colorPicker->SetColour(m_palette->GetItemColor(event.GetId()));
}

void PaletteFrame::OnPaletteItemRightMouseButtonClicked(wxMouseEvent &event)
{
    m_palette->SetItemColor(event.GetId(), m_colorPicker->GetColour());
}
