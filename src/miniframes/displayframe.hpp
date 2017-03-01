#ifndef DISPLAYFRAME_HPP
#define DISPLAYFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "miniframe.hpp"
#include "../constants.hpp"
#include "../gl/displaysettings.hpp"

/**
 * \brief Mini frame with settings regarding what should be displayed in a workspace.
 */
class DisplayFrame: public MiniFrame
{
    public:
        DisplayFrame(wxWindow *parent);
        void UpdateCheckBoxes(const DisplaySettings& displaySettings);

    private:
        wxCheckBox *m_checkBoxes[DISPLAY_SETTINGS_COUNT];
};

#endif
