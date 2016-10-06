#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

/**
 * \brief Represents settings INI file.
 */
class Settings
{
    public:
        wxString GetSoldatPath()
        {
            return m_soldatPath;
        }

        void LoadSettingsFromFile();
        void SaveSettingsToFile();

    private:
        wxString m_soldatPath;

        void LoadDefaultSettings();
};

#endif