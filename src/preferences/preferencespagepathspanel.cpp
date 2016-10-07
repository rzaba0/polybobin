#include "preferencespagepathspanel.hpp"
#include <wx/preferences.h>

PreferencesPagePathsPanel::PreferencesPagePathsPanel(wxWindow *parent, Settings *settings)
    : wxPanel(parent)
{
    m_settings = settings;

    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddSpacer(10);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Soldat"));
    m_soldatPathPicker = new wxDirPickerCtrl(this, wxID_ANY, settings->GetSoldatPath());
    sizer->Add(m_soldatPathPicker);
    sizer->AddSpacer(10);

    SetSizerAndFit(sizer);

    /**
     * According to wxWidgets sample, the preferences get updated differently according to platform/system:
     * 1) On some platforms (OS X, GNOME), the preferences get updated as soon as the user makes a change in preferences editor
     *      -> In order to handle this case, we bind event handlers to widgets.
     * 2) On other platforms, the preferences get updated only after "OK" button has been pressed
     *      -> In order to handle this case, we override wxWindow's TransferDataFromWindow method.
     */
    if (wxPreferencesEditor::ShouldApplyChangesImmediately())
    {
        m_soldatPathPicker->Bind(wxEVT_DIRPICKER_CHANGED, &PreferencesPagePathsPanel::OnSoldatPathChanged, this);
    }
}

bool PreferencesPagePathsPanel::TransferDataFromWindow()
{
    wxFileDirPickerEvent dummy;
    OnSoldatPathChanged(dummy);

    return true;
}

void PreferencesPagePathsPanel::OnSoldatPathChanged(wxFileDirPickerEvent &event)
{
    wxString soldatPath = m_soldatPathPicker->GetDirName().GetFullPath();
    m_settings->SetSoldatPath(soldatPath);
}