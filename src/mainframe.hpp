#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/preferences.h>
#include "mapsettingsdialog.hpp"
#include "miniframes/displayframe.hpp"
#include "miniframes/paletteframe.hpp"
#include "miniframes/toolbarframe.hpp"
#include "miniframes/sceneryframe.hpp"
#include "notebook.hpp"
#include "settings.hpp"

/**
 * \brief Main wxFrame that is going to be shown during the entire execution of the program.
 * It also handles communication between widgets.
 */
class MainFrame: public wxFrame
{
    public:
        MainFrame(Settings *settings);
        ~MainFrame();
        const DisplayFrame& GetDisplayFrame() const { return *m_displayFrame; }
        const PaletteFrame& GetPaletteFrame() const { return *m_paletteFrame; }
        const ToolbarFrame& GetToolbarFrame() const { return *m_toolbarFrame; }
        SceneryFrame& GetSceneryFrame() const { return *m_sceneryFrame; }
        const Settings& GetSettings() const { return *m_settings; }

    private:
        MapSettingsDialog *m_mapSettingsDialog;

        DisplayFrame *m_displayFrame;
        PaletteFrame *m_paletteFrame;
        ToolbarFrame *m_toolbarFrame;
        SceneryFrame *m_sceneryFrame;

        Notebook *m_notebook;
        wxPreferencesEditor *m_preferencesEditor;
        Settings *m_settings;

        void AddWorkspace(wxString mapPath);

        void OnBackgroundColorChanged(wxColourPickerEvent &event);
        void OnDisplayFrameCheckBoxClicked(wxCommandEvent &event);
        void OnMenuBarItemClicked(wxCommandEvent &event);
        void OnNotebookPageChanged(wxBookCtrlEvent &event);
        void OnPolygonsTextureChanged(wxCommandEvent &event);

        void ShowAllMiniFrames(bool show);
};

#endif
