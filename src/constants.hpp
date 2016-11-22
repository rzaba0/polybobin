#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

const wxString PROGRAM_NAME = wxT("polybobin");
const unsigned int WINDOW_WIDTH = 640;
const unsigned int WINDOW_HEIGHT = 480;

const wxString PATH_GFX_TOOLS = wxT("../gfx/tools.png"),
               PATH_GFX_SELECTION_GRID = wxT("../gfx/selectiongrid.png"),
               PATH_GFX_SPAWNPOINTS = wxT("../gfx/spawnpoints.png"),
               PATH_SETTINGS = wxT("settings.ini");

const wxKeyCode ADD_SELECTION_KEY = WXK_SHIFT,
                REMOVE_SELECTION_KEY = WXK_CONTROL,
                MOVE_SELECTED_KEY = WXK_SPACE;

const float EPSILON = 0.00001f;

enum WxWidgetsId
{
    ID_MENU_FILE_NEW = 1,
    ID_MENU_FILE_OPEN_COMPILED,
    ID_MENU_FILE_SAVE_AS_PMS,

    ID_MENU_EDIT_SELECT_ALL,
    ID_MENU_EDIT_PREFERENCES,

    ID_MENU_WINDOWS_HIDE_ALL,
    ID_MENU_WINDOWS_SHOW_ALL,
    ID_MENU_WINDOWS_DISPLAY,
    ID_MENU_WINDOWS_TOOLBAR,

    ID_FRAME_DISPLAY,
    ID_FRAME_TOOLBAR,

    ID_TOOL_TRANSFORM,
    ID_TOOL_CREATE_POLYGON,
    ID_TOOL_VERTEX_SELECTION,
    ID_TOOL_SELECTION,
    ID_TOOL_VERTEX_COLOR,
    ID_TOOL_POLYGON_COLOR,
    ID_TOOL_TEXTURE,
    ID_TOOL_SCENERY,
    ID_TOOL_WAYPOINT,
    ID_TOOL_OBJECT,
    ID_TOOL_COLOR_PICKER,
    ID_TOOL_SKETCH,
    ID_TOOL_LIGHT,
    ID_TOOL_DEPTH_MAP,

    ID_GL_CANVAS,

    ID_POLYGON_TYPE_NORMAL,
    ID_POLYGON_TYPE_ONLY_BULLETS_COLLIDE,
    ID_POLYGON_TYPE_ONLY_PLAYERS_COLLIDE,
    ID_POLYGON_TYPE_NO_COLLIDE,
    ID_POLYGON_TYPE_ICE,
    ID_POLYGON_TYPE_DEADLY,
    ID_POLYGON_TYPE_BLOODY_DEADLY,
    ID_POLYGON_TYPE_HURTS,
    ID_POLYGON_TYPE_REGENERATES,
    ID_POLYGON_TYPE_LAVA,
    ID_POLYGON_TYPE_ALPHABULLETS,
    ID_POLYGON_TYPE_ALPHAPLAYERS,
    ID_POLYGON_TYPE_BRAVOBULLETS,
    ID_POLYGON_TYPE_BRAVOPLAYERS,
    ID_POLYGON_TYPE_CHARLIEBULLETS,
    ID_POLYGON_TYPE_CHARLIEPLAYERS,
    ID_POLYGON_TYPE_DELTABULLETS,
    ID_POLYGON_TYPE_DELTAPLAYERS,
    ID_POLYGON_TYPE_BOUNCY,
    ID_POLYGON_TYPE_EXPLOSIVE,
    ID_POLYGON_TYPE_HURTFLAGGERS,
    ID_POLYGON_TYPE_FLAGGERCOLLIDES,
    ID_POLYGON_TYPE_NONFLAGGERCOLLIDES,
    ID_POLYGON_TYPE_FLAGCOLLIDES
};

const unsigned int DISPLAY_SETTINGS_COUNT = 5;
/**
 * The values below are used:
 * 1) As ids for checkboxes inside DisplayFrame.
 * 2) In DisplaySettings class.
 */
enum
{
    DISPLAY_BACKGROUND,
    DISPLAY_POLYGONS,
    DISPLAY_SCENERY,
    DISPLAY_SPAWNPOINTS,
    DISPLAY_WIREFRAME
};

const wxString POLYGON_TYPES_NAMES[] =
{
    "Normal",
    "Only bullets collide",
    "Only players collide",
    "Doesn't collide",
    "Ice",
    "Deadly",
    "Bloody deadly",
    "Hurts",
    "Regenerates",
    "Lava",
    "Alpha bullets collide",
    "Alpha players collide",
    "Bravo bullets collide",
    "Bravo players collide",
    "Charlie bullets collide",
    "Charlie players collide",
    "Delta bullets collide",
    "Delta players collide",
    "Bouncy",
    "Explosive",
    "Hurts flaggers",
    "Flaggers collide",
    "Non-flaggers collide",
    "Flag collides"
};

#endif
