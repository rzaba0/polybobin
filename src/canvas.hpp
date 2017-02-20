#pragma once

#include "map/pmsstructs.hpp"

class Canvas
{
public:
    virtual int AddPolygon(PMSPolygon polygon, PMSVertex firstVertex) = 0;
    virtual void EditPolygonVertex(unsigned polygonIndex, PMSPolygonType polygonType, unsigned vertexIndex, PMSVertex vertex) = 0;
    virtual const PMSPolygon& GetPolygon(unsigned polygonIndex) const = 0;
    virtual unsigned GetPolygonCount() const = 0;

    virtual void SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor) = 0;
    virtual void SetPolygonsTexture(wxString textureFilename) = 0;

    virtual void HandleLeftMouseButtonClick(const wxMouseEvent& event) = 0;
    virtual void HandleMouseMotion(const wxMouseEvent &event) = 0;
    virtual void HandleRightMouseButtonRelease(const wxMouseEvent& event) = 0;

    virtual void Draw() = 0;

    virtual ~Canvas() = default;
};