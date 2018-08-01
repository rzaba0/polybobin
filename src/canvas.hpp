#pragma once

#include "map/pmsstructs.hpp"

class Canvas
{
public:
    virtual int AddPolygon(PMSPolygon polygon, PMSVertex firstVertex) = 0;
    virtual int AddScenery(PMSScenery newScenery) = 0;
    virtual int AddSceneryType(wxString sceneryName) = 0;
    virtual int AddSpawnPoint(PMSSpawnPoint spawnPoint) = 0;

    virtual void EditPolygonVertex(unsigned polygonIndex, PMSPolygonType polygonType, unsigned vertexIndex, PMSVertex vertex) = 0;
    virtual void EditScenery(unsigned sceneryIndex, PMSScenery scenery) = 0;
    virtual void EditSpawnPoint(unsigned int spawnPointIdx, PMSSpawnPoint spawnPoint) = 0;

    virtual void RemovePolygons(const wxVector<unsigned int> &polygons) = 0;
    virtual void RemoveSpawnPoints(wxVector<unsigned int> spawnPoints) = 0;
    virtual void RemoveSceneries(const wxVector<unsigned int> &sceneries) = 0;
    virtual void RemoveScenery(unsigned int sceneryId) = 0;

    virtual void UpdatePolygonSelectionForRedraw() = 0;
    virtual const PMSPolygon& GetPolygon(unsigned polygonIndex) const = 0;
    virtual unsigned GetPolygonCount() const = 0;
    virtual const PMSScenery& GetScenery(unsigned sceneryIndex) const = 0;
    virtual unsigned GetSceneryCount() const = 0;

    virtual wxRealPoint GetMousePositionOnMap(wxPoint positionOnCanvas) const = 0;

    virtual void SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor) = 0;
    virtual void SetPolygonsTexture(wxString textureFilename) = 0;
    virtual void SetTextureTransformationMode(bool mode = true) = 0;

    virtual void HandleLeftMouseButtonClick(const wxMouseEvent& event) = 0;
    virtual void HandleMouseMotion(const wxMouseEvent &event) = 0;
    virtual void HandleRightMouseButtonRelease(const wxMouseEvent& event) = 0;
    virtual void HandleKeyPress(const wxKeyEvent& event) = 0;

    virtual void Draw() = 0;

    virtual ~Canvas() = default;
};
