#pragma once
#include <memory>
#include "gl/glcanvas.hpp"
#include <wx/glcanvas.h>
#include "eventdispatcher.hpp"

class MainFrame;

std::unique_ptr<EventDispatcher> MakeEventDispatcher(GLCanvas& canvas, MainFrame& mainFrame);