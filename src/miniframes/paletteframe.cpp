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

    opacitySizer->Add(m_opacitySlider);
    opacitySizer->Add(m_opacityText);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_colorPicker, 1, wxALIGN_CENTER_HORIZONTAL);
    sizer->AddSpacer(20);
    sizer->Add(opacitySizer);

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