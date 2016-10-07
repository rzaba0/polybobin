#ifndef PREFERENCESPAGEPATHSPANEL_HPP
#define PREFERENCESPAGEPATHSPANEL_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/filepicker.h>
#include "../settings.hpp"

/**
 * \brief Panel that is displayed in "Paths" tab inside preferences editor.
 */
class PreferencesPagePathsPanel: public wxPanel
{
    public:
        PreferencesPagePathsPanel(wxWindow *parent, Settings *settings);
        virtual bool TransferDataFromWindow();

    private:
        Settings *m_settings;
        wxDirPickerCtrl *m_soldatPathPicker;

        void OnSoldatPathChanged(wxFileDirPickerEvent &event);
};

#endif