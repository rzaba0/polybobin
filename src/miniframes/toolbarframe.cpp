#include "toolbarframe.hpp"
#include "../constants.hpp"
#include <utility>
#include <wx/filename.h>
#include <wx/stdpaths.h>

ToolbarFrame::ToolbarFrame(wxWindow *parent, ToolSelectionCallback toolSelecitoncallback)
    : MiniFrame(parent, ID_FRAME_TOOLBAR, "Toolbar", wxDefaultPosition, wxDefaultSize)
    , m_toolSelectionCallback{std::move(toolSelecitoncallback)}
{
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString gfxToolsPath(f.GetPath());
    gfxToolsPath += "/" + PATH_GFX_TOOLS;

    wxImage toolsImage(gfxToolsPath, wxBITMAP_TYPE_PNG);

    wxGridSizer *gridSizer = new wxGridSizer(TOOLBAR_COLUMNS_COUNT);

    wxBitmap tmpBitmap;
    for (int i = 0; i < TOOLBAR_TOOLS_COUNT; ++i)
    {
        tmpBitmap = wxBitmap(toolsImage.GetSubImage(wxRect(0, i * TOOLBAR_TILE_HEIGHT,
                                                            TOOLBAR_TILE_WIDTH, TOOLBAR_TILE_HEIGHT)));

        // ID_TOOL_TRANSFORM is the first, so we use it as reference.
        m_toggleButtons[i] = new wxBitmapToggleButton(this, ID_TOOL_TRANSFORM + i, tmpBitmap,
            wxDefaultPosition, wxSize(TOOLBAR_TILE_WIDTH + TOOLBAR_TILE_PADDING, TOOLBAR_TILE_HEIGHT + TOOLBAR_TILE_PADDING));
        m_toggleButtons[i]->Bind(wxEVT_TOGGLEBUTTON, &ToolbarFrame::OnButtonToggled, this);

        gridSizer->Add(m_toggleButtons[i]);
    }

    m_selectedToolId = ID_TOOL_TRANSFORM;
    SetToggleButtonValue(m_selectedToolId, true);

    SetSizer(gridSizer);
    Fit();
}

void ToolbarFrame::OnButtonToggled(wxCommandEvent &event)
{
    int toggledButtonId = event.GetId();

    SetToggleButtonValue(m_selectedToolId, false);
    SetToggleButtonValue(toggledButtonId, true);
    m_toolSelectionCallback(toggledButtonId - ID_TOOL_TRANSFORM);
    m_selectedToolId = toggledButtonId;
}

void ToolbarFrame::SetToggleButtonValue(int toolId, bool value)
{
    int toggleButtonId = toolId - ID_TOOL_TRANSFORM;
    m_toggleButtons[toggleButtonId]->SetValue(value);
}
