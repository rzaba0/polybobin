#ifndef MINIFRAME_HPP
#define MINIFRAME_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/minifram.h>

/**
 * \brief Generic mini frame with custom behavior.
 */
class MiniFrame: public wxMiniFrame
{
    public:
        MiniFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize &size);
        void ToggleVisibility();

    private:
        /**
         * \brief We want custom behavior when the "X" icon is pressed.
         */
        void OnClosed(wxCloseEvent &event);
};

#endif
