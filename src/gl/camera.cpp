#include "camera.hpp"

Camera::Camera(): m_x(), m_y(), m_zoom()
{
}

bool Camera::CanZoomIn() const
{
    return m_zoom < ZOOM_UPPER_BOUND && fabsf(m_zoom - ZOOM_UPPER_BOUND) > EPSILON;
}

bool Camera::CanZoomOut() const
{
    return m_zoom > ZOOM_LOWER_BOUND && fabsf(m_zoom - ZOOM_LOWER_BOUND) > EPSILON;
}

float Camera::GetWidth(wxSize canvasSize) const
{
    float canvasWidth = (float) canvasSize.GetWidth();
    // TODO: fix magic number.
    return canvasWidth - canvasWidth * m_zoom / 100.0f;
}

float Camera::GetHeight(wxSize canvasSize) const
{
    float canvasHeight = (float) canvasSize.GetHeight();
    return canvasHeight - canvasHeight * m_zoom / 100.0f;
}

void Camera::Scroll(const wxRealPoint& delta)
{
    m_x += delta.x;
    m_y += delta.y;
}

void Camera::ZoomIn()
{
    m_zoom += ZOOM_DELTA;
}

void Camera::ZoomOut()
{
    m_zoom -= ZOOM_DELTA;
}
