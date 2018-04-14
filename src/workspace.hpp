#ifndef WORKSPACE_HPP
#define WORKSPACE_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "gl/displaysettings.hpp"
#include "gl/glcanvas.hpp"
#include "map/map.hpp"
#include "settings.hpp"
#include "eventdispatcher.hpp"
#include "selectionmanager.hpp"
#include "miniframes\sceneryframe.hpp"
#include <memory>

/**
 * \brief Represents a page inside our Notebook class.
 *  It contains OpenGL canvas in which we draw the map.
 */
class MainFrame;

class Workspace: public wxWindow
{
    public:
        Workspace(wxWindow *notebook, MainFrame& mainFrame, Settings settings, wxString mapPath);
        ~Workspace() = default;

        const DisplaySettings& GetDisplaySettings();
        void SetDisplaySetting(int setting, bool display);

        GLCanvas& GetGLCanvas() { return *m_glCanvas; }
        Map &GetMap() { return m_map; }

        void GiveFocusToGLCanvas();

        void SaveMapAsPMS(const wxString& destinationPath);

        void SelectAll();
        void SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor);
        void SetPolygonsTexture(const wxString& textureFilename);

        void SelectTool(int toolId);
    private:
        GLCanvas* m_glCanvas;
        Map m_map;
        DisplaySettings m_displaySettings;
        SceneryFrame& m_sceneryFrame;
        std::unique_ptr<EventDispatcher> m_eventDispatcher;
        std::unique_ptr<SelectionManager> m_selectionManager;
};

#endif
