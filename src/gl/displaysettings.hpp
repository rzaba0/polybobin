#ifndef DISPLAYSETTINGS_HPP
#define DISPLAYSETTINGS_HPP

#include "../constants.hpp"

/**
 * \brief Used to keep track of preferences regarding what should be displayed in the canvas.
 */
 class DisplaySettings
 {
    public:
        DisplaySettings();
        void SetDisplaySetting(int setting, bool display);
        bool ShouldDisplay(int setting);

    private:
        bool m_settings[DISPLAY_SETTINGS_COUNT];
 };

#endif