#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class Tool
{
public:
    virtual void OnSelect() = 0;
    virtual void OnUnselect() = 0;
    virtual void OnCanvasLeftMouseButtonClick(const wxMouseEvent &event) = 0;
    virtual void OnCanvasMouseMotion(const wxMouseEvent &event) = 0;
    virtual void OnCanvasRightMouseButtonRelease(const wxMouseEvent &event) = 0;
    virtual void OnTimerTick() = 0;
    virtual ~Tool() = default;
};


