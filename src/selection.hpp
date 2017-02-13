#pragma once
#include <set>

/**
 * \brief Used to keep track of currently selected objects (such as polygons, scenery, ...).
 */
class Selection
{
public:
    Selection() = default;
    Selection(const Selection&) = default;
    Selection(Selection&&) = default;
    Selection(unsigned id);
    
    bool contains(unsigned id) const;

    void select(unsigned id);
    void selectAll(unsigned elementsCount);

    void unselect(unsigned id);
    void unselectAll();

    bool empty() const;

    auto begin() const { return m_selectedIds.begin(); }
    auto end() const { return m_selectedIds.end(); }
private:
    std::set<unsigned> m_selectedIds;
};