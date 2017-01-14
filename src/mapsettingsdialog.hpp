#ifndef MAPSETTINGSDIALOG_HPP
#define MAPSETTINGSDIALOG_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/clrpicker.h>
#include "map/map.hpp"

class MapSettingsDialog: public wxDialog
{
    public:
        MapSettingsDialog(wxWindow *parent, Map *map, wxString soldatDirectoryPath);

        wxColor GetBackgroundBottomColor() { return m_backgroundBottomColorPicker->GetColour(); }
        wxColor GetBackgroundTopColor() { return m_backgroundTopColorPicker->GetColour(); }

        wxColourPickerCtrl *GetBackgroundBottomColorPicker() { return m_backgroundBottomColorPicker; }
        wxColourPickerCtrl *GetBackgroundTopColorPicker() { return m_backgroundTopColorPicker; }

        wxChoice *GetTextureChoice() { return m_textureChoice; }

    private:
        Map *m_map;

        wxColourPickerCtrl *m_backgroundBottomColorPicker, *m_backgroundTopColorPicker;
        wxTextCtrl *m_descriptionTextCtrl;
        wxChoice *m_grenadesCountChoice;
        wxChoice *m_jetCountChoice;
        wxTextCtrl *m_jetCountTextCtrl;
        wxChoice *m_medikitsCountChoice;
        wxChoice *m_stepTypeChoice;
        wxChoice *m_textureChoice;
        wxChoice *m_weatherTypeChoice;

        static const unsigned int JET_OPTIONS_COUNT = 8;
        static const wxString JET_COUNT_NAMES[];
        static const wxString JET_COUNT_VALUES[];

        void OnJetCountSelected(wxCommandEvent &event);
        void OnJetCountTextChanged(wxCommandEvent &event);

        bool TransferDataFromWindow();
        bool TransferDataToWindow();
};

#endif