#include "camera.hpp"

Camera::Camera(): m_x(), m_y(), m_zoom()
{
}

bool Camera::CanZoomIn()
{
    return m_zoom < ZOOM_UPPER_BOUND && fabsf(m_zoom - ZOOM_UPPER_BOUND) > EPSILON;
}

bool Camera::CanZoomOut()
{
    return m_zoom > ZOOM_LOWER_BOUND && fabsf(m_zoom - ZOOM_LOWER_BOUND) > EPSILON;
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
