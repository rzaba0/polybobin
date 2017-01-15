#include "selection.hpp"
#include <algorithm>

bool Selection::IsSelected(unsigned int id)
{
    return (std::find(m_selectedIds.begin(), m_selectedIds.end(), id) != m_selectedIds.end());
}

void Selection::Select(unsigned int id)
{
    m_selectedIds.push_back(id);
}

void Selection::SelectAll(unsigned int elementsCount)
{
    UnselectAll();
    m_selectedIds.reserve(elementsCount);
    for (unsigned int i = 0; i < elementsCount; ++i)
    {
        m_selectedIds.push_back(i);
    }
}

void Selection::Unselect(unsigned int id)
{
    m_selectedIds.erase(std::remove(m_selectedIds.begin(), m_selectedIds.end(), id), m_selectedIds.end());
}

void Selection::UnselectAll()
{
    m_selectedIds.clear();
}