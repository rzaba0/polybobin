#include "camera.hpp"

Camera::Camera()
{
    m_x = 0.0;
    m_y = 0.0;
    m_zoom = 0.0f;
}

bool Camera::CanZoomIn()
{
    return m_zoom < ZOOM_UPPER_BOUND && fabsf(m_zoom - ZOOM_UPPER_BOUND) > EPSILON;
}

bool Camera::CanZoomOut()
{
    return m_zoom > ZOOM_LOWER_BOUND && fabsf(m_zoom - ZOOM_LOWER_BOUND) > EPSILON;
}

float Camera::GetX()
{
    return m_x;
}

float Camera::GetY()
{
    return m_y;
}

float Camera::GetWidth(wxSize canvasSize)
{
    float canvasWidth = (float) canvasSize.GetWidth();
    // TODO: fix magic number.
    return canvasWidth - canvasWidth * m_zoom / 100.0f;
}

float Camera::GetHeight(wxSize canvasSize)
{
    float canvasHeight = (float) canvasSize.GetHeight();
    return canvasHeight - canvasHeight * m_zoom / 100.0f;
}

float Camera::GetZoom()
{
    return m_zoom;
}

void Camera::ScrollX(float delta)
{
    m_x += delta;
}

void Camera::ScrollY(float delta)
{
    m_y += delta;
}

void Camera::ZoomIn()
{
    m_zoom += ZOOM_DELTA;
}

void Camera::ZoomOut()
{
    m_zoom -= ZOOM_DELTA;
}
