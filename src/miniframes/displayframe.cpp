#include "displayframe.hpp"
#include "../constants.hpp"

DisplayFrame::DisplayFrame(wxWindow *parent)
    : MiniFrame(parent, ID_FRAME_DISPLAY, wxT("Display"), wxDefaultPosition, wxDefaultSize)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    m_checkBoxes[DISPLAY_BACKGROUND] = new wxCheckBox(this, DISPLAY_BACKGROUND, "Background");
    m_checkBoxes[DISPLAY_POLYGONS] = new wxCheckBox(this, DISPLAY_POLYGONS, "Polygons");
    m_checkBoxes[DISPLAY_SCENERY] = new wxCheckBox(this, DISPLAY_SCENERY, "Scenery");
    m_checkBoxes[DISPLAY_SPAWNPOINTS] = new wxCheckBox(this, DISPLAY_SPAWNPOINTS, "Spawn points");
    m_checkBoxes[DISPLAY_WIREFRAME] = new wxCheckBox(this, DISPLAY_WIREFRAME, "Wireframe");

    for (unsigned int i = 0; i < DISPLAY_SETTINGS_COUNT; ++i)
    {
        sizer->Add(m_checkBoxes[i]);
    }
    SetSizer(sizer);

    Fit();
}

void DisplayFrame::UpdateCheckBoxes(DisplaySettings displaySettings)
{
    m_checkBoxes[DISPLAY_BACKGROUND]->SetValue(displaySettings.ShouldDisplay(DISPLAY_BACKGROUND));
    m_checkBoxes[DISPLAY_POLYGONS]->SetValue(displaySettings.ShouldDisplay(DISPLAY_POLYGONS));
    m_checkBoxes[DISPLAY_SCENERY]->SetValue(displaySettings.ShouldDisplay(DISPLAY_SCENERY));
    m_checkBoxes[DISPLAY_SPAWNPOINTS]->SetValue(displaySettings.ShouldDisplay(DISPLAY_SPAWNPOINTS));
    m_checkBoxes[DISPLAY_WIREFRAME]->SetValue(displaySettings.ShouldDisplay(DISPLAY_WIREFRAME));
}
