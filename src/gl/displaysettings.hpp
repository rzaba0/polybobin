#pragma once

#include "../constants.hpp"

/**
 * \brief Used to keep track of preferences regarding what should be displayed in the canvas.
 */
 class DisplaySettings
 {
    public:
        DisplaySettings();
        void SetDisplaySetting(int setting, bool display);
        bool ShouldDisplay(int setting) const;
        bool ShouldDisplayPolygons() const { return ShouldDisplay(DISPLAY_POLYGONS); }
        bool ShouldDisplayScenery()  const { return ShouldDisplay(DISPLAY_SCENERY); }
    private:
        bool m_settings[DISPLAY_SETTINGS_COUNT];
 };