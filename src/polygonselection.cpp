#include "polygonselection.hpp"
#include <algorithm>

bool PolygonSelection::contains(unsigned polyId) const
{
    auto pred = [polyId](const Poly& poly) { return poly.id == polyId; };
    return std::find_if(m_selected.begin(), m_selected.end(), pred) != m_selected.end();
}

bool PolygonSelection::contains(unsigned polyId, unsigned vertexId) const
{
    auto pred = [polyId, vertexId](const Poly& poly) { return poly.id == polyId && poly.vertex[vertexId]; };
    return std::find_if(m_selected.begin(), m_selected.end(), pred) != m_selected.end();
}

void PolygonSelection::select(unsigned polyId)
{
    if (!contains(polyId)) m_selected.emplace_back(polyId);
}

void PolygonSelection::select(unsigned polyId, unsigned vertexId)
{
    auto pred = [polyId](const Poly& poly) { return poly.id == polyId; };
    auto it = std::find_if(m_selected.begin(), m_selected.end(), pred);
    if (it != m_selected.end())
    {
        it->vertex[vertexId] = true;
    } else
        m_selected.emplace_back(polyId, vertexId);
}

void PolygonSelection::selectAll(unsigned polyCount)
{
    unselectAll();
    m_selected.reserve(polyCount);
    for (unsigned id = 0; id < polyCount; id++)
        m_selected.emplace_back(id);
}

void PolygonSelection::unselect(unsigned polyId)
{
    auto pred = [polyId](const Poly& poly) { return poly.id == polyId; };
    m_selected.erase(std::remove_if(m_selected.begin(), m_selected.end(), pred), m_selected.end());
}

void PolygonSelection::unselect(unsigned polyId, unsigned vertexId)
{
    auto pred = [polyId](const Poly& poly) { return poly.id == polyId; };
    auto it = std::find_if(m_selected.begin(), m_selected.end(), pred);
    if (it != m_selected.end())
    {
        it->vertex[vertexId] = false;
        if (!it->vertex[0] && !it->vertex[1] && !it->vertex[2])
            m_selected.erase(it);
    }
}

void PolygonSelection::unselectAll()
{
    m_selected.clear();
}

bool PolygonSelection::empty() const
{
    return m_selected.empty();
}

size_t PolygonSelection::size() const
{
    return m_selected.size();
}