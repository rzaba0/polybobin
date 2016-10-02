#ifndef TOOLBARFRAME_HPP
#define TOOLBARFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/tglbtn.h>
#include "miniframe.hpp"

/**
 * \brief Mini frame with tool selection.
 */
class ToolbarFrame: public MiniFrame
{
    public:
        ToolbarFrame(wxWindow *parent);
        int GetSelectedToolId() { return m_selectedToolId; }

    private:
        static const int TOOLBAR_TOOLS_COUNT = 14;
        static const int TOOLBAR_TILE_WIDTH = 32,
                  TOOLBAR_TILE_HEIGHT = 32;
        static const int TOOLBAR_TILE_PADDING = 5;
        static const int TOOLBAR_COLUMNS_COUNT = 2;

        int m_selectedToolId;
        wxToggleButton *m_toggleButtons[TOOLBAR_TOOLS_COUNT];

        void OnButtonToggled(wxCommandEvent &event);
        void SetToggleButtonValue(int toolId, bool value);
};

#endif
