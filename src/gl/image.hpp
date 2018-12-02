#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "glfunctions.hpp"
#include <wx/string.h>
#include <memory>
#include <vector>

/**
 * \brief Helper class for drawing images from files.
 */
class Image
{
    public:
        const GLubyte* GetData() const
        {
            return m_data.data();
        }

        int GetWidth()
        {
            return m_width;
        }

        int GetHeight()
        {
            return m_height;
        }

        bool HasAlpha()
        {
            return m_hasAlpha;
        }

        void OpenAndResize(wxString path);

    private:
        std::vector<GLubyte> m_data;
        bool m_hasAlpha;
        int m_width, m_height;
};

#endif
