#include "mapsettingsdialog.hpp"
#include "constants.hpp"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/valnum.h>

const wxString MapSettingsDialog::JET_COUNT_NAMES[] = { "None", "Minimal", "Very low", "Low", "Normal", "High", "Very high", "Infinite", "Custom" };
const wxString MapSettingsDialog::JET_COUNT_VALUES[] = { "0", "12", "45", "95", "190", "320", "800", "25000" };

MapSettingsDialog::MapSettingsDialog(wxWindow *parent, Map &map, wxString soldatDirectoryPath)
    : wxDialog(parent, wxID_ANY, "Map settings")
    , m_map(map)
{
    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxSizer *descriptionSizer = new wxBoxSizer(wxHORIZONTAL);
    descriptionSizer->Add(new wxStaticText(this, wxID_ANY, "Description: ", wxDefaultPosition, wxSize(70, -1)));
    m_descriptionTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(245, -1));
    m_descriptionTextCtrl->SetMaxLength(DESCRIPTION_MAX_LENGTH);
    descriptionSizer->Add(m_descriptionTextCtrl);

    wxSizer *gridSizer = new wxGridSizer(2, 10, 25);

    wxSizer *weatherTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    weatherTypeSizer->Add(new wxStaticText(this, wxID_ANY, "Weather: ", wxDefaultPosition, wxSize(70, -1)));
    // Note: make sure these values match the enum in pmsenums.hpp
    const unsigned int WEATHER_TYPES_COUNT = 4;
    const wxString WEATHER_TYPE_NAMES[] = { "None", "Rain", "Sandstorm", "Snow" };
    m_weatherTypeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxSize(80, -1), WEATHER_TYPES_COUNT, WEATHER_TYPE_NAMES);
    weatherTypeSizer->Add(m_weatherTypeChoice);

    wxSizer *grenadesCountSizer = new wxBoxSizer(wxHORIZONTAL);
    grenadesCountSizer->Add(new wxStaticText(this, wxID_ANY, "Grenades' count: ", wxDefaultPosition, wxSize(100, -1)));
    const unsigned int MAX_GRENADES_COUNT = 12;
    wxString grenadesCountStrings[MAX_GRENADES_COUNT + 1];
    for (unsigned int i = 0; i <= MAX_GRENADES_COUNT; ++i)
    {
        grenadesCountStrings[i] << i;
    }
    m_grenadesCountChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxSize(40, -1), MAX_GRENADES_COUNT + 1, grenadesCountStrings);
    grenadesCountSizer->Add(m_grenadesCountChoice);

    wxSizer *stepTypeSizer = new wxBoxSizer(wxHORIZONTAL);
    stepTypeSizer->Add(new wxStaticText(this, wxID_ANY, "Step type: ", wxDefaultPosition, wxSize(70, -1)));
    // Note: make sure these values match the enum in pmsenums.hpp
    const unsigned int STEP_TYPES_COUNT = 3;
    const wxString STEP_TYPE_NAMES[] = { "Hard", "Soft", "None" };
    m_stepTypeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxSize(80, -1), STEP_TYPES_COUNT, STEP_TYPE_NAMES);
    stepTypeSizer->Add(m_stepTypeChoice);

    wxSizer *medikitsCountSizer = new wxBoxSizer(wxHORIZONTAL);
    medikitsCountSizer->Add(new wxStaticText(this, wxID_ANY, "Medikits' count: ", wxDefaultPosition, wxSize(100, -1)));
    const unsigned int MAX_MEDIKITS_COUNT = 12;
    wxString medikitsCountStrings[MAX_MEDIKITS_COUNT + 1];
    for (unsigned int i = 0; i <= MAX_MEDIKITS_COUNT; ++i)
    {
        medikitsCountStrings[i] << i;
    }
    m_medikitsCountChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition,
        wxSize(40, -1), MAX_MEDIKITS_COUNT + 1, medikitsCountStrings);
    medikitsCountSizer->Add(m_medikitsCountChoice);

    gridSizer->Add(weatherTypeSizer);
    gridSizer->Add(grenadesCountSizer);
    gridSizer->Add(stepTypeSizer);
    gridSizer->Add(medikitsCountSizer);

    wxSizer *jetCountSizer = new wxBoxSizer(wxHORIZONTAL);
    jetCountSizer->Add(new wxStaticText(this, wxID_ANY, "Jet fuel: ",wxDefaultPosition, wxSize(70, -1)));
    m_jetCountChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(80, -1),
        JET_OPTIONS_COUNT + 1, JET_COUNT_NAMES);
    m_jetCountChoice->Bind(wxEVT_CHOICE, &MapSettingsDialog::OnJetCountSelected, this);
    jetCountSizer->Add(m_jetCountChoice);

    wxIntegerValidator<int> jetCountTextValidator;
    jetCountTextValidator.SetMin(0);
    jetCountTextValidator.SetMax(25000);
    m_jetCountTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, 0L, jetCountTextValidator);
    m_jetCountTextCtrl->Bind(wxEVT_TEXT, &MapSettingsDialog::OnJetCountTextChanged, this);
    jetCountSizer->AddSpacer(25);
    jetCountSizer->Add(m_jetCountTextCtrl);

    wxSizer *backgroundColorsSizer = new wxBoxSizer(wxHORIZONTAL);
    backgroundColorsSizer->Add(new wxStaticText(this, wxID_ANY, "Background colors: ", wxDefaultPosition, wxSize(140, -1)));
    wxSizer *colorPickersSizer = new wxBoxSizer(wxVERTICAL);
    m_backgroundTopColorPicker = new wxColourPickerCtrl(this, wxID_ANY);
    m_backgroundBottomColorPicker = new wxColourPickerCtrl(this, wxID_ANY);
    colorPickersSizer->Add(m_backgroundTopColorPicker);
    colorPickersSizer->Add(m_backgroundBottomColorPicker);
    backgroundColorsSizer->Add(colorPickersSizer);

    wxSizer *textureSizer = new wxBoxSizer(wxHORIZONTAL);
    textureSizer->Add(new wxStaticText(this, wxID_ANY, "Texture: ", wxDefaultPosition, wxSize(70, -1)));
    wxArrayString texturesFilenames;
    wxDir::GetAllFiles(soldatDirectoryPath + "textures/", &texturesFilenames,
                       wxEmptyString, wxDIR_FILES);
    // wxDir::GetAllFiles returns absolute paths. We only care about filenames.
    for (unsigned int i = 0; i < texturesFilenames.GetCount(); ++i)
    {
        wxString path = texturesFilenames.Item(i);
        texturesFilenames.Item(i).Remove(0,
                                         path.Last(wxFileName::GetPathSeparator()) + 1);
    }
    m_textureChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(180, -1), texturesFilenames);
    textureSizer->Add(m_textureChoice);

    sizer->AddSpacer(10);
    sizer->Add(descriptionSizer);
    sizer->AddSpacer(10);
    sizer->Add(gridSizer);
    sizer->AddSpacer(10);
    sizer->Add(jetCountSizer);
    sizer->AddSpacer(20);
    sizer->Add(backgroundColorsSizer);
    sizer->AddSpacer(20);
    sizer->Add(textureSizer);
    sizer->AddSpacer(20);
    sizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxALIGN_RIGHT);

    SetSizerAndFit(sizer);
}

void MapSettingsDialog::OnJetCountSelected(wxCommandEvent &event)
{
    int selectionId = event.GetSelection();

    // We don't care about "Custom".
    if (selectionId < JET_OPTIONS_COUNT)
    {
        m_jetCountTextCtrl->ChangeValue(JET_COUNT_VALUES[selectionId]);
    }
}

void MapSettingsDialog::OnJetCountTextChanged(wxCommandEvent &event)
{
    for (unsigned int i = 0; i < JET_OPTIONS_COUNT; ++i)
    {
        if (event.GetString().IsSameAs(JET_COUNT_VALUES[i]))
        {
            m_jetCountChoice->SetSelection(i);
            return;
        }
    }

    // Set it to "Custom".
    m_jetCountChoice->SetSelection(JET_OPTIONS_COUNT);
}

bool MapSettingsDialog::TransferDataFromWindow()
{
    wxColor backgroundBottomColor = m_backgroundBottomColorPicker->GetColour();
    m_map.SetBackgroundBottomColor(PMSColor(backgroundBottomColor.Red(),
                                            backgroundBottomColor.Green(),
                                            backgroundBottomColor.Blue()));

    wxColor backgroundTopColor = m_backgroundTopColorPicker->GetColour();
    m_map.SetBackgroundTopColor(PMSColor(backgroundTopColor.Red(),
                                         backgroundTopColor.Green(),
                                         backgroundTopColor.Blue()));

    m_map.SetDescription(m_descriptionTextCtrl->GetValue().mb_str());
    m_map.SetGrenadesCount(m_grenadesCountChoice->GetSelection());
    m_map.SetJetCount(wxAtoi(m_jetCountTextCtrl->GetValue()));
    m_map.SetMedikitsCount(m_medikitsCountChoice->GetSelection());
    m_map.SetStepType((PMSStepType) m_stepTypeChoice->GetSelection());
    m_map.SetTextureName(m_textureChoice->GetStringSelection().mb_str());
    m_map.SetWeatherType((PMSWeatherType) m_weatherTypeChoice->GetSelection());

    return true;
}

bool MapSettingsDialog::TransferDataToWindow()
{
    PMSColor backgroundBottomColor = m_map.GetBackgroundBottomColor();
    m_backgroundBottomColorPicker->SetColour(wxColor(backgroundBottomColor.red,
                                                     backgroundBottomColor.green,
                                                     backgroundBottomColor.blue));

    PMSColor backgroundTopColor = m_map.GetBackgroundTopColor();
    m_backgroundTopColorPicker->SetColour(wxColor(backgroundTopColor.red,
                                                  backgroundTopColor.green,
                                                  backgroundTopColor.blue));

    m_descriptionTextCtrl->SetValue(m_map.GetDescription());
    m_grenadesCountChoice->SetSelection(m_map.GetGrenadesCount());
    m_jetCountTextCtrl->SetValue(wxString::Format("%d", m_map.GetJetCount()));
    m_medikitsCountChoice->SetSelection(m_map.GetMedikitsCount());
    m_stepTypeChoice->SetSelection(m_map.GetStepType());
    m_textureChoice->SetStringSelection(m_map.GetTextureName());
    m_weatherTypeChoice->SetSelection(m_map.GetWeatherType());

    return true;
}