#include "selection.hpp"
#include <algorithm>

Selection::Selection(unsigned id)
{
    m_selectedIds.emplace(id);
}

bool Selection::contains(unsigned id) const
{
    return m_selectedIds.find(id) != m_selectedIds.end();
}

void Selection::select(unsigned id)
{
    m_selectedIds.insert(id);
}

void Selection::selectAll(unsigned elementsCount)
{
    unselectAll();
    for (unsigned i(0); i < elementsCount; i++)
        m_selectedIds.emplace_hint(m_selectedIds.end(), i);
}

void Selection::unselect(unsigned id)
{
    m_selectedIds.erase(id);
}

void Selection::unselectAll()
{
    m_selectedIds.clear();
}

bool Selection::empty() const
{
    return m_selectedIds.empty();
}