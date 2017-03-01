#include "displaysettings.hpp"
#include <algorithm>

DisplaySettings::DisplaySettings()
{
    std::fill(m_settings, m_settings + DISPLAY_SETTINGS_COUNT, true);
}

void DisplaySettings::SetDisplaySetting(int setting, bool display)
{
    m_settings[setting] = display;
}

bool DisplaySettings::ShouldDisplay(int setting) const
{
    return m_settings[setting];
}