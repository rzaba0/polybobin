#include "app.hpp"
#include "mainframe.hpp"

wxIMPLEMENT_APP(Polybobin);

bool Polybobin::OnInit()
{
    // TODO: check if the device supports OpenGL 3.3+. If not, return false.

    m_glContext = NULL;

    m_settings = new Settings();
    try
    {
        m_settings->LoadSettingsFromFile();
    }
    catch (const std::exception& error)
    {
        wxMessageBox(error.what(), "Settings");
    }

    MainFrame *frame = new MainFrame(m_settings);
    frame->Show();

    return true;
}

int Polybobin::OnExit()
{
    delete m_glContext;

    m_settings->SaveSettingsToFile();
    delete m_settings;

    return wxApp::OnExit();
}

wxGLContext &Polybobin::GetGLContext(GLCanvas *glCanvas)
{
    if (!m_glContext)
    {
        m_glContext = new wxGLContext(glCanvas);
    }

    m_glContext->SetCurrent(*glCanvas);
    return *m_glContext;
}
