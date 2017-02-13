#pragma once
#include <vector>

class PolygonSelection
{
public:
    struct Poly
    {
        Poly(unsigned polyId) : id{ polyId }, vertex{ true, true, true } {}
        Poly(unsigned polyId, unsigned vertexId) : id{ polyId }, vertex{ false, false, false }
        {
            vertex[vertexId] = true;
        }
        unsigned id;
        bool vertex[3];
    };
    bool contains(unsigned polyId) const;
    bool contains(unsigned polyId, unsigned vertexId) const;

    void select(unsigned id);
    void select(unsigned id, unsigned vertexId);
    void selectAll(unsigned polyCount);

    void unselect(unsigned polyId);
    void unselect(unsigned polyId, unsigned vertexId);
    void unselectAll();

    bool empty() const;
    size_t PolygonSelection::size() const;

    auto begin() const { return m_selected.begin(); }
    auto end() const { return m_selected.end(); }
private:
    std::vector<Poly> m_selected;
};