#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

/**
 * \brief Keeps track of which chunk of map is currently being displayed.
 */
class Camera
{
    public:
        Camera();

        bool CanZoomIn(), CanZoomOut();
        float GetX(), GetY();
        float GetWidth(wxSize canvasSize), GetHeight(wxSize canvasSize);
        float GetZoom();
        void ScrollX(float delta), ScrollY(float delta);
        void ZoomIn(), ZoomOut();

    private:
        float m_x, m_y;
        /**
         * Equal to 0.0 by default. Increases when zooming in and decreases when zooming out.
         */
        float m_zoom;

        static constexpr float EPSILON = 0.000001;

        static constexpr float ZOOM_DELTA = 10.0;

        static constexpr float ZOOM_LOWER_BOUND = -500.0;
        static constexpr float ZOOM_UPPER_BOUND = 100.0;
};

#endif
