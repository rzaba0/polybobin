#include "mainframe.hpp"
#include "constants.hpp"
#include "menubar.hpp"
#include "notebook.hpp"
#include "preferences/preferencespagepaths.hpp"

MainFrame::MainFrame(Settings *settings)
    : wxFrame(NULL, wxID_ANY, PROGRAM_NAME, wxDefaultPosition, wxSize(WINDOW_WIDTH, WINDOW_HEIGHT))
{
    m_preferencesEditor = NULL;
    m_settings = settings;

    CreateStatusBar();
    SetStatusText("Welcome");

    MenuBar *menuBar = new MenuBar();
    menuBar->Bind(wxEVT_MENU, &MainFrame::OnMenuBarItemClicked, this);
    SetMenuBar(menuBar);

    m_displayFrame = new DisplayFrame(this);
    m_displayFrame->Show();
    // When the frame is closed, we want to update menu bar.
    m_displayFrame->Bind(wxEVT_CLOSE_WINDOW, &MenuBar::OnFrameClosed, menuBar);
    // When a checkbox belonging to this frame gets (un)checked, we want to update what is being drawn in the current workspace.
    m_displayFrame->Bind(wxEVT_CHECKBOX, &MainFrame::OnDisplayFrameCheckBoxClicked, this);

    m_paletteFrame = new PaletteFrame(this);
    m_paletteFrame->Show();
    m_paletteFrame->Bind(wxEVT_CLOSE_WINDOW, &MenuBar::OnFrameClosed, menuBar);

    m_toolbarFrame = new ToolbarFrame(this);
    m_toolbarFrame->Show();
    m_toolbarFrame->Bind(wxEVT_CLOSE_WINDOW, &MenuBar::OnFrameClosed, menuBar);

    wxPanel *notebookPanel = new wxPanel(this);
    wxBoxSizer *notebookPanelSizer = new wxBoxSizer(wxVERTICAL);
    m_notebook = new Notebook(notebookPanel, *this);
    m_notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainFrame::OnNotebookPageChanged, this);
    notebookPanelSizer->Add(m_notebook, 1, wxEXPAND);
    notebookPanel->SetSizer(notebookPanelSizer);

    AddWorkspace(m_settings->GetSoldatPath() + "maps/test.pms");
}

MainFrame::~MainFrame()
{
    // Fix for segmentation fault after closing the program with at least 2 tabs opened.
    m_notebook->Unbind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainFrame::OnNotebookPageChanged, this);

    if (m_preferencesEditor)
    {
        m_preferencesEditor->Dismiss();
    }
}

void MainFrame::AddWorkspace(wxString mapPath)
{
    try
    {
        m_notebook->AddWorkspace(*m_settings, mapPath);
    }
    catch (const std::runtime_error& error)
    {
        wxMessageBox(error.what(), "Workspace construction failed.");
        Close(true);

        return;
    }
}

void MainFrame::OnBackgroundColorChanged(wxColourPickerEvent &event)
{
    m_notebook->SetBackgroundColors(m_mapSettingsDialog->GetBackgroundBottomColor(),
                                    m_mapSettingsDialog->GetBackgroundTopColor());
}

void MainFrame::OnDisplayFrameCheckBoxClicked(wxCommandEvent &event)
{
    int displaySetting = event.GetId();
    bool isChecked = event.IsChecked();

    m_notebook->SetCurrentDisplaySetting(displaySetting, isChecked);
}

void MainFrame::OnGLCanvasLeftMouseButtonClicked(wxMouseEvent &event)
{
    wxColor selectedColor = m_paletteFrame->GetColor();
    int selectedToolId = m_toolbarFrame->GetSelectedToolId();
    wxPoint mousePositionOnCanvas = event.GetPosition();
    //m_notebook->HandleCurrentGLCanvasLeftMouseButtonClick(mousePositionOnCanvas, selectedToolId,
    //                                                      selectedColor);
}

void MainFrame::OnGLCanvasMouseMotion(wxMouseEvent &event)
{
    char statusText[255];
    wxPoint mousePositionOnMap = m_notebook->GetCurrentMousePositionOnMap();

    sprintf(statusText, "%d %d", mousePositionOnMap.x, mousePositionOnMap.y);
    SetStatusText(statusText);

    wxColor selectedColor = m_paletteFrame->GetColor();
    //m_notebook->HandleCurrentGLCanvasMouseMotion(event, selectedColor);
}

void MainFrame::OnGLCanvasRightMouseButtonReleased(wxMouseEvent &event)
{
    int selectedToolId = m_toolbarFrame->GetSelectedToolId();
    //m_notebook->HandleCurrentGLCanvasRightMouseButtonRelease(selectedToolId);
}

void MainFrame::OnMenuBarItemClicked(wxCommandEvent &event)
{
    wxWindowID menuBarItemId = event.GetId();

    switch (menuBarItemId)
    {
        case ID_MENU_FILE_NEW:
            AddWorkspace(wxEmptyString);
            break;

        case ID_MENU_FILE_OPEN_COMPILED:
            {
                wxString path = wxFileSelector(wxT("Open PMS file"), wxEmptyString,
                                wxEmptyString, wxEmptyString, wxT("*.pms"), wxFD_FILE_MUST_EXIST);
                if (!path.IsEmpty())
                {
                    AddWorkspace(path);
                }
            }
            break;

        case ID_MENU_FILE_SAVE_AS_PMS:
            {
                wxString path = wxFileSelector(wxT("Save as PMS"), m_settings->GetSoldatPath() + "maps/",
                    wxEmptyString, wxT(".pms"), wxT(".pms"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
                if (!path.IsEmpty())
                {
                    m_notebook->SaveCurrentMapAsPMS(path);
                }
            }
            break;

        case ID_MENU_EDIT_SELECT_ALL:
            m_notebook->SelectAll();
            break;

        case ID_MENU_EDIT_MAP_SETTINGS:
            {
                m_mapSettingsDialog = new MapSettingsDialog(this, m_notebook->GetCurrentMap(),
                                                            m_settings->GetSoldatPath());
                m_mapSettingsDialog->GetBackgroundBottomColorPicker()->Bind(wxEVT_COLOURPICKER_CHANGED,
                    &MainFrame::OnBackgroundColorChanged, this);
                m_mapSettingsDialog->GetBackgroundTopColorPicker()->Bind(wxEVT_COLOURPICKER_CHANGED,
                    &MainFrame::OnBackgroundColorChanged, this);
                m_mapSettingsDialog->GetTextureChoice()->Bind(wxEVT_CHOICE,
                    &MainFrame::OnPolygonsTextureChanged, this);
                m_mapSettingsDialog->ShowModal();
            }
            break;

        case ID_MENU_EDIT_PREFERENCES:
            m_preferencesEditor = new wxPreferencesEditor();
            m_preferencesEditor->AddPage(new PreferencesPagePaths(m_settings));
            m_preferencesEditor->Show(this);
            break;

        case ID_MENU_WINDOWS_SHOW_ALL:
            ShowAllMiniFrames(true);
            break;

        case ID_MENU_WINDOWS_HIDE_ALL:
            ShowAllMiniFrames(false);
            break;

        case ID_MENU_WINDOWS_DISPLAY:
            m_displayFrame->ToggleVisibility();
            break;

        case ID_MENU_WINDOWS_PALETTE:
            m_paletteFrame->ToggleVisibility();
            break;

        case ID_MENU_WINDOWS_TOOLBAR:
            m_toolbarFrame->ToggleVisibility();
            break;
    }

    // Pass the event to following event handler.
    event.Skip();
}

void MainFrame::OnNotebookPageChanged(wxBookCtrlEvent &event)
{
    m_displayFrame->UpdateCheckBoxes(m_notebook->GetCurrentDisplaySettings());
    event.Skip();
}

void MainFrame::OnPolygonsTextureChanged(wxCommandEvent &event)
{
    wxString textureFilename = event.GetString();
    m_notebook->SetPolygonsTexture(textureFilename);
}

void MainFrame::ShowAllMiniFrames(bool show)
{
    m_displayFrame->Show(show);
    m_paletteFrame->Show(show);
    m_toolbarFrame->Show(show);
}
