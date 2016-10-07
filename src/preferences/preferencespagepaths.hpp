#ifndef PREFERENCESPAGEPATHS_HPP
#define PREFERENCESPAGEPATHS_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/preferences.h>
#include "preferencespagepathspanel.hpp"
#include "../settings.hpp"

/**
 * \brief Represents the "Paths" tab inside preferences editor.
 */
class PreferencesPagePaths: public wxPreferencesPage
{
    public:
        PreferencesPagePaths(Settings *settings)
        {
            m_settings = settings;
        }

        virtual wxString GetName() const
        {
            return "Paths";
        }

        virtual wxWindow *CreateWindow(wxWindow *parent)
        {
            return new PreferencesPagePathsPanel(parent, m_settings);
        }

    private:
        Settings *m_settings;
};

#endif