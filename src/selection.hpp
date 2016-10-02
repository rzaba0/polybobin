#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <wx/vector.h>

/**
 * \brief Used to keep track of currently selected objects (such as polygons, scenery, ...).
 */
class Selection
{
public:
        wxVector<unsigned int> GetSelectedIds()
        {
            return m_selectedIds;
        }

        bool IsSelected(unsigned int id);

        void Select(unsigned int id);
        void SelectAll(unsigned int elementsCount);

        void Unselect(unsigned int id);
        void UnselectAll();

    private:
        wxVector<unsigned int> m_selectedIds;
};

#endif