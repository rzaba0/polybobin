#ifndef MENUBAR_HPP
#define MENUBAR_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

/**
 * \brief Represents menu bar on top of the window. It contains drop-down
 * menus (such as File, Edit, etc...).
 */
class MenuBar: public wxMenuBar
{
    public:
        MenuBar();

        /**
         * \brief Called when frames such as DisplayFrame or ToolbarFrame are closed.
         * We set the binding in MainFrame class.
         */
        void OnFrameClosed(wxCloseEvent &event);

    private:
        wxMenuItem *m_menuItemWindowsDisplay,
                   *m_menuItemWindowsToolbar;

        void CheckAllMenuItems(bool check);
        void OnHideAllWindows(wxCommandEvent &event);
        void OnShowAllWindows(wxCommandEvent &event);
};

#endif
