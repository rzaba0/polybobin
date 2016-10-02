#include "miniframe.hpp"

MiniFrame::MiniFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize &size)
    : wxMiniFrame(parent, id, title, position, size, wxCAPTION | wxCLOSE_BOX | wxFRAME_NO_TASKBAR)
{
    Bind(wxEVT_CLOSE_WINDOW, &MiniFrame::OnClosed, this);
}

void MiniFrame::ToggleVisibility()
{
    Show(!IsShown());
}

void MiniFrame::OnClosed(wxCloseEvent &event)
{
    Show(false);
}
