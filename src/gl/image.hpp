#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "glfunctions.hpp"
#include <wx/string.h>

/**
 * \brief Helper class for drawing images from files.
 */
class Image
{
    public:
        ~Image();
        GLubyte *GetData()
        {
            return m_data;
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
        GLubyte *m_data = NULL;
        bool m_hasAlpha;
        int m_width, m_height;
};

#endif