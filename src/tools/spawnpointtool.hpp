#pragma once

#include "tool.hpp"
#include "../gl/glcanvas.hpp"
#include "../map/pmsstructs.hpp"

class SpawnPointTool : public Tool
{
    public:
        SpawnPointTool(GLCanvas &canvas);
        void OnSelect() override;
        void OnUnselect() override;
        void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) override;
        void OnCanvasLeftMouseButtonRelease(const wxMouseEvent &event) override;
        void OnCanvasMouseMotion(const wxMouseEvent &event) override;
        void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) override;
        void OnTimerTick() override;
        ~SpawnPointTool() override = default;

    private:
        GLCanvas &m_canvas;
        wxRealPoint m_mousePositionOnMap;
        int m_spawnPointIdx;
        PMSSpawnPointType m_spawnPointType;

        void OnNewSpawnPointTypeSelected(wxCommandEvent &event);
};
