#include "settings.hpp"
#include "constants.hpp"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>

void Settings::LoadSettingsFromFile()
{
    wxFileConfig settingsFile(wxEmptyString, wxEmptyString,
        GetSettingsPath(), wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    if (!settingsFile.Read("SoldatPath", &m_soldatPath))
    {
        LoadDefaultSettings();
        throw std::runtime_error(std::string("Your ") + PROGRAM_NAME + " settings file either doesn't exist " +
            "or is corrupted. Default settings will be used.");
    }

}

void Settings::SaveSettingsToFile()
{
    wxFileConfig settingsFile(wxEmptyString, wxEmptyString,
        GetSettingsPath(), wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
    settingsFile.Write("SoldatPath", m_soldatPath);
    settingsFile.Flush();
}

void Settings::LoadDefaultSettings()
{
    m_soldatPath = wxT("E:/Soldat171_v2/");
}

const wxString Settings::GetSettingsPath() const
{
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString settingsPath(f.GetPath());
    settingsPath += "/" + PATH_SETTINGS;

    return settingsPath;
}

