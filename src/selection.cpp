#include "selection.hpp"

bool Selection::IsSelected(unsigned int id)
{
    for (unsigned int i = 0; i < m_selectedIds.size(); ++i)
    {
        if (m_selectedIds[i] == id)
        {
            return true;
        }
    }

    return false;
}

void Selection::Select(unsigned int id)
{
    m_selectedIds.push_back(id);
}

void Selection::SelectAll(unsigned int elementsCount)
{
    UnselectAll();

    for (unsigned int i = 0; i < elementsCount; ++i)
    {
        m_selectedIds.push_back(i);
    }
}

void Selection::Unselect(unsigned int id)
{
    for (unsigned int i = 0; i < m_selectedIds.size(); ++i)
    {
        if (m_selectedIds[i] == id)
        {
            m_selectedIds.erase(m_selectedIds.begin() + i);
            return;
        }
    }
}

void Selection::UnselectAll()
{
    m_selectedIds.clear();
}