#include "menubar.hpp"
#include "constants.hpp"

MenuBar::MenuBar()
    : wxMenuBar()
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_MENU_FILE_NEW, "&New\tCtrl-N", "Create new map");
    menuFile->Append(ID_MENU_FILE_OPEN_COMPILED, "Open compiled", "Open compiled map");
    menuFile->Append(ID_MENU_FILE_SAVE_AS_PMS, "Save as PMS", "Save map as PMS");

    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(ID_MENU_EDIT_SELECT_ALL, "Select all\tCtrl-A", "Select all");
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_MENU_EDIT_PREFERENCES, "Preferences");

    wxMenu *menuWindows = new wxMenu;
    menuWindows->Append(ID_MENU_WINDOWS_HIDE_ALL, "Hide all", "Hide all windows");
    menuWindows->Append(ID_MENU_WINDOWS_SHOW_ALL, "Show all", "Show all windows");
    menuWindows->AppendSeparator();
    m_menuItemWindowsDisplay = menuWindows->AppendCheckItem(ID_MENU_WINDOWS_DISPLAY, "Display", "Show/hide display");
    m_menuItemWindowsDisplay->Check(true);
    m_menuItemWindowsToolbar = menuWindows->AppendCheckItem(ID_MENU_WINDOWS_TOOLBAR, "Toolbar", "Show/hide toolbar");
    m_menuItemWindowsToolbar->Check(true);

    Append(menuFile, "File");
    Append(menuEdit, "Edit");
    Append(menuWindows, "Windows");

    // Callbacks when user clicks "Hide all" or "Show all" buttons.
    Bind(wxEVT_MENU, &MenuBar::OnHideAllWindows, this, ID_MENU_WINDOWS_HIDE_ALL);
    Bind(wxEVT_MENU, &MenuBar::OnShowAllWindows, this, ID_MENU_WINDOWS_SHOW_ALL);
}

void MenuBar::OnFrameClosed(wxCloseEvent &event)
{
    int frameId = event.GetId();

    switch (frameId)
    {
        case ID_FRAME_TOOLBAR:
            m_menuItemWindowsToolbar->Check(false);
            break;

        case ID_FRAME_DISPLAY:
            m_menuItemWindowsDisplay->Check(false);
            break;
    }

    // We pass the event to following event handlers.
    event.Skip();
}

void MenuBar::CheckAllMenuItems(bool check)
{
    m_menuItemWindowsToolbar->Check(check);
    m_menuItemWindowsDisplay->Check(check);
}

void MenuBar::OnHideAllWindows(wxCommandEvent &event)
{
    CheckAllMenuItems(false);
}

void MenuBar::OnShowAllWindows(wxCommandEvent &event)
{
    CheckAllMenuItems(true);
}
