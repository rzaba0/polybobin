#include "glmanager.hpp"
#include "glfunctions.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "../constants.hpp"

GLManager::GLManager(Settings settings, Map &map)
    : m_glOutlineScenerySelection(PMSColor(129, 23, 23, 255),
        PMSColor(176, 31, 31, 255),
        PMSColor(255, 44, 44, 255))
    , m_glOutlineSceneryWireframe(PMSColor(129, 129, 129, 255),
        PMSColor(168, 168, 168, 255),
        PMSColor(210, 210, 210, 255))
    , m_map(map)
    , m_transformFrameVisible(false)
    , m_textureTransformationMode(false)
{
    m_glReady = false;
    m_settings = settings;
}

void GLManager::ApplyPolygonSelection(const PolygonSelection& selectedPolygons)
{
    m_glSelectionPolygons.ApplySelection(selectedPolygons);
    m_glOutlinePolygons.ApplySelection(selectedPolygons);
}

void GLManager::AddPolygon(PMSPolygonType polygonType, PMSVertex firstVertex)
{
    m_glBackground.UpdateVBO(m_map.GetBackgroundTopColor(), m_map.GetBackgroundBottomColor(),
                             m_map.GetBoundaries());
    m_glPolygons.AddPolygon(firstVertex);
    m_glOutlinePolygons.AddPolygon(polygonType, firstVertex);
    m_glSelectionPolygons.AddPolygon(polygonType, firstVertex);
}

void GLManager::AddSpawnPoint(PMSSpawnPoint spawnPoint)
{
    m_glSpawnPoints.AddSpawnPoint(spawnPoint);
}

void GLManager::EditPolygonVertex(unsigned int polygonIndex, PMSPolygonType polygonType,
                                  unsigned int vertexIndex, PMSVertex vertex)
{
    m_glBackground.UpdateVBO(m_map.GetBackgroundTopColor(), m_map.GetBackgroundBottomColor(),
                             m_map.GetBoundaries());
    m_glPolygons.EditPolygonVertex(polygonIndex, vertexIndex, vertex);
    if (!m_textureTransformationMode)
        m_glOutlinePolygons.EditPolygonVertex(polygonIndex, polygonType, vertexIndex, vertex);
    m_glSelectionPolygons.EditPolygonVertex(polygonIndex, polygonType, vertexIndex, vertex);
}

void GLManager::ResetPolygons(wxVector<PMSPolygon> polygons)
{
    m_glPolygons.ResetPolygons(polygons);
    m_glOutlinePolygons.ResetPolygons(polygons);
    m_glSelectionPolygons.ResetPolygons(polygons);
}

void GLManager::EditScenery(unsigned int sceneryIndex, PMSScenery scenery)
{
    m_glScenery.EditScenery(sceneryIndex, scenery);
    m_glOutlineScenerySelection.EditScenery(sceneryIndex, scenery);
    m_glOutlineSceneryWireframe.EditScenery(sceneryIndex, scenery);
}

void GLManager::EditSpawnPoint(unsigned int spawnPointIdx, PMSSpawnPoint spawnPoint)
{
    m_glSpawnPoints.EditSpawnPoint(spawnPointIdx, spawnPoint);
}

void GLManager::ResetSpawnPoints(wxVector<PMSSpawnPoint> spawnPoints)
{
    m_glSpawnPoints.ResetSpawnPoints(spawnPoints);
}

void GLManager::ResetSceneries(wxVector<PMSScenery> sceneryInstances)
{
    m_glScenery.ResetSceneries(sceneryInstances);
    m_glOutlineScenerySelection.ResetSceneries(sceneryInstances);
    m_glOutlineSceneryWireframe.ResetSceneries(sceneryInstances);
}

unsigned int GLManager::GetTextureWidth()
{
    return m_glPolygons.GetTextureWidth();
}

unsigned int GLManager::GetTextureHeight()
{
    return m_glPolygons.GetTextureHeight();
}

void GLManager::Init()
{
    m_glReady = InitGLPointers();

    if (!m_glReady)
    {
        throw std::runtime_error("Could not retrieve OpenGL functions' addresses.");
    }
}

bool GLManager::IsGLReady()
{
    return m_glReady;
}

void GLManager::Render(Camera camera, wxSize canvasSize, DisplaySettings displaySettings,
                       const PolygonSelection& selectedPolygons, const Selection& selectedScenery,
                       bool addingPolygon)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 transform = glm::ortho(camera.GetX(),
                                     camera.GetX() + camera.GetWidth(canvasSize),
                                     camera.GetY() + camera.GetHeight(canvasSize),
                                     camera.GetY());

    glm::mat4 translation = glm::lookAt(glm::vec3(camera.GetX(), camera.GetY(), 2.0f),
                              glm::vec3(camera.GetX(), camera.GetY(), 1.0f),
                              glm::vec3(0, 1, 0));

    if (displaySettings.ShouldDisplay(DISPLAY_BACKGROUND))
    {
        m_glBackground.Render(transform);
    }

    if (displaySettings.ShouldDisplay(DISPLAY_SCENERY))
    {
        m_glScenery.Render(transform, lBEHIND_ALL);
        m_glScenery.Render(transform, lBEHIND_MAP);
    }

    if (displaySettings.ShouldDisplay(DISPLAY_POLYGONS))
    {
        m_glPolygons.Render(transform);
    }

    if (displaySettings.ShouldDisplay(DISPLAY_SCENERY))
    {
        m_glScenery.Render(transform, lBEHIND_NONE);
    }

    if (displaySettings.ShouldDisplay(DISPLAY_SPAWNPOINTS))
    {
        m_glSpawnPoints.Render(transform);
    }

    if (displaySettings.ShouldDisplay(DISPLAY_WIREFRAME))
    {
        m_glOutlinePolygons.RenderAll(transform);
        m_glOutlineSceneryWireframe.RenderAll(transform);
    }

    if (!selectedPolygons.empty())
    {
        m_glOutlinePolygons.RenderSelected(transform, selectedPolygons);

        if (!m_textureTransformationMode)
            m_glSelectionPolygons.RenderSelected(transform, selectedPolygons);
    }

    if (!selectedScenery.empty())
    {
       m_glOutlineScenerySelection.RenderSelected(transform, selectedScenery);
    }

    // We draw outline around the polygon that is currently being added.

    if (addingPolygon)
    {
        // TODO:
        //m_glOutlinePolygons.RenderSelected(transform, m_map.GetPolygonsCount() - 1);
    }

    if (m_transformFrameVisible)
    {
        m_glTransformFrame.Render(transform, translation);
    }
}

void GLManager::SetBackgroundColors(wxColor backgroundBottomColor, wxColor backgroundTopColor)
{
    m_glBackground.UpdateVBO(PMSColor(backgroundTopColor.Red(), backgroundTopColor.Green(), backgroundTopColor.Blue()),
                             PMSColor(backgroundBottomColor.Red(), backgroundBottomColor.Green(), backgroundBottomColor.Blue()),
                             m_map.GetBoundaries());
}

void GLManager::SetPolygonsTexture(wxString textureFilename)
{
    m_glPolygons.ReplaceTexture(m_settings.GetSoldatPath() + "textures/", textureFilename);
}

void GLManager::SetTransformFrameVisible(bool mode)
{
    m_transformFrameVisible = mode;
}

void GLManager::SetTransformFramePosition(wxPoint bottomLeft,
                       wxPoint bottomRight,
                       wxPoint topLeft,
                       wxPoint topRight)
{
    m_glTransformFrame.Move(bottomLeft, bottomRight, topLeft, topRight);
}

void GLManager::SetupShaders()
{
    m_glBackground.SetupShaderProgram();
    m_glPolygons.SetupShaderProgram();
    m_glScenery.SetupShaderProgram();
    m_glSpawnPoints.SetupShaderProgram();

    m_glOutlinePolygons.SetupShaderProgram();
    m_glOutlineScenerySelection.SetupShaderProgram();
    m_glOutlineSceneryWireframe.SetupShaderProgram();

    m_glSelectionPolygons.SetupShaderProgram();

    m_glTransformFrame.SetupShaderProgram();
}

void GLManager::SetupTextures()
{
    m_glPolygons.SetupTexture(m_settings.GetSoldatPath() + "textures/", m_map.GetTextureName());
    m_glScenery.SetupTextures(m_settings.GetSoldatPath() + "scenery-gfx/", m_map.GetSceneryTypes());
    m_glSpawnPoints.SetupTextures();

    m_glSelectionPolygons.SetupTexture();
}

void GLManager::SetupVertices()
{
    m_glBackground.SetupVAO(m_map.GetBackgroundTopColor(),
                            m_map.GetBackgroundBottomColor(),
                            m_map.GetBoundaries());
    m_glPolygons.SetupVAO(m_map.GetPolygons());
    m_glScenery.SetupVAO(m_map.GetSceneryInstances());
    m_glSpawnPoints.SetupVAO(m_map.GetSpawnPoints());

    m_glOutlinePolygons.SetupVAO(m_map.GetPolygons());
    m_glOutlineScenerySelection.SetupVAO(m_map.GetSceneryInstances());
    m_glOutlineSceneryWireframe.SetupVAO(m_map.GetSceneryInstances());

    m_glSelectionPolygons.SetupVAO(m_map.GetPolygons());

    m_glTransformFrame.SetupVAO();
}

void GLManager::AddSceneryTexture(wxString sceneryName)
{
    m_glScenery.AddTexture(m_settings.GetSoldatPath() + "scenery-gfx/" + sceneryName);
}

void GLManager::SetFillPolygonSelection(const PolygonSelection& selectedPolygons, bool mode)
{
    auto polygons = m_map.GetPolygons();

    if (m_textureTransformationMode != mode)
    {
        for (const auto& poly : selectedPolygons)
        {
            for (unsigned i = 0; i < 3; i++)
            {
                if (mode)
                {
                    PMSColor color(255, 0, 0);

                    if (poly.vertex[i])
                        m_glOutlinePolygons.SetVertexColor(poly.id, i, color);
                }
                else
                {
                    m_glOutlinePolygons.RestoreVertexColor(poly.id, i, polygons[poly.id].polygonType); // TODO
                }
            }
        }
        
        m_textureTransformationMode = mode;
    }
}
