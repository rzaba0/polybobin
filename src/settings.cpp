#include "settings.hpp"
#include "constants.hpp"
#include <wx/fileconf.h>

void Settings::LoadSettingsFromFile()
{
    wxFileConfig *settingsFile = new wxFileConfig(wxEmptyString, wxEmptyString,
        PATH_SETTINGS, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);

    if (!settingsFile->Read("SoldatPath", &m_soldatPath))
    {
        LoadDefaultSettings();
        delete settingsFile;
        throw wxT("Your " + PROGRAM_NAME + " settings file either doesn't exist " +
            "or is corrupted. Default settings will be used.");
    }

    delete settingsFile;
}

void Settings::SaveSettingsToFile()
{
    wxFileConfig *settingsFile = new wxFileConfig(wxEmptyString, wxEmptyString,
        PATH_SETTINGS, wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);

    settingsFile->Write("SoldatPath", m_soldatPath);
    settingsFile->Flush();

    delete settingsFile;
}

void Settings::LoadDefaultSettings()
{
    m_soldatPath = wxT("E:/Soldat171_v2/");
}