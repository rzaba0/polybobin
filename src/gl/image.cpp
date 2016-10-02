#include "image.hpp"
#include <wx/image.h>

Image::~Image()
{
    delete[] m_data;
}

void Image::OpenAndResize(wxString path)
{
    wxImage image;
    if (!image.LoadFile(path))
    {
        /**
         * If the file could not be found, we also check if there is a .png file with the
         * same filename. Apparently, Soldat has the same behavior.
         */
         path = path.BeforeLast('.').Append(".png");

         if (!image.LoadFile(path))
         {
             throw wxString("Could not load image ") + path;
         }
    }
    
    m_width = image.GetWidth();
    m_height = image.GetHeight();
    m_hasAlpha = image.HasAlpha();
    GLubyte *unresizedData = image.GetData();
    // According to wxWidgets docs, alpha channel has to be allocated with malloc.
    GLubyte *alphaChannel = (GLubyte*) malloc(sizeof(GLubyte) * m_width * m_height);

    int x, y;
    bool updateAlphaChannel = false;
    for (y = 0; y < m_height; ++y)
    {
        for (x = 0; x < m_width; ++x)
        {
            // If the pixel is fully green, we set its alpha channel so that it's invisible.
            if (unresizedData[(x + y * m_width) * 3 + 0] == 0 &&
                unresizedData[(x + y * m_width) * 3 + 1] == 255 &&
                unresizedData[(x + y * m_width) * 3 + 2] == 0)
            {
                image.SetRGB(x, y, 0, 0, 0);
                updateAlphaChannel = true;
                alphaChannel[x + y * m_width] = 0;
            }
            else
            {
                alphaChannel[x + y * m_width] = m_hasAlpha ? image.GetAlpha(x, y) : 255;
            }
        }
    }
    if (updateAlphaChannel)
    {
        // According to wxWidgets docs, wxImage will take care of freeing alphaChannel.
        image.SetAlpha(alphaChannel);
        m_hasAlpha = true;
    }
    else
    {
        free(alphaChannel);
    }

    float powerOfTwoWidth = std::log((float) m_width) / std::log(2.0);
    float powerOfTwoHeight = std::log((float) m_height) / std::log(2.0);
    bool rescaleNeeded = false;

    if ((int) powerOfTwoWidth != powerOfTwoWidth)
    {
        m_width = (int) std::pow(2.0, (int) (std::ceil(powerOfTwoWidth)));
        rescaleNeeded = true;
    }
    if ((int) powerOfTwoHeight != powerOfTwoHeight)
    {
        m_height = (int) std::pow(2.0, (int) (std::ceil(powerOfTwoHeight)));
        rescaleNeeded = true;
    }

    if (rescaleNeeded)
    {
        image.Rescale(m_width, m_height, wxIMAGE_QUALITY_HIGH);
    }

    // Here we create a single array that will be used in OpenGL rendering.
    GLubyte *imageData = image.GetData();
    GLubyte *imageAlpha = image.GetAlpha();
    int bytesPerPixel = m_hasAlpha ? 4 : 3;
    int imageSize = m_width * m_height * bytesPerPixel;
    m_data = new GLubyte[imageSize];

    for (y = 0; y < m_height; ++y)
    {
        for (x = 0; x < m_width; ++x)
        {
            m_data[(x + y * m_width) * bytesPerPixel + 0] = imageData[(x + y * m_width) * 3 + 0];
            m_data[(x + y * m_width) * bytesPerPixel + 1] = imageData[(x + y * m_width) * 3 + 1];
            m_data[(x + y * m_width) * bytesPerPixel + 2] = imageData[(x + y * m_width) * 3 + 2];

            if (m_hasAlpha)
            {
                m_data[(x + y * m_width) * bytesPerPixel + 3] = imageAlpha[x + y * m_width];
            }
        }
    }
}