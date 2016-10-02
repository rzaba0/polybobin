#include "displaysettings.hpp"

DisplaySettings::DisplaySettings()
{
    for (unsigned int i = 0; i < DISPLAY_SETTINGS_COUNT; ++i)
    {
        m_settings[i] = true;
    }
}

void DisplaySettings::SetDisplaySetting(int setting, bool display)
{
    m_settings[setting] = display;
}

bool DisplaySettings::ShouldDisplay(int setting)
{
    return m_settings[setting];
}