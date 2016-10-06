#ifndef APP_HPP
#define APP_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "gl/glcanvas.hpp"
#include <wx/glcanvas.h>
#include "settings.hpp"

/**
 * \brief Main app class.
 */
class Polybobin: public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();

        /**
         * \brief Retrieves a reference to the OpenGL context that we are
         * using across the program.
         * @param glCanvas OpenGL canvas that is used to initialize the
         * OpenGL context.
         * @returns Reference to the OpenGL context.
         */
        wxGLContext &GetGLContext(GLCanvas *glCanvas);

    private:
        wxGLContext *m_glContext;
        Settings *m_settings;
};

wxDECLARE_APP(Polybobin);

#endif
