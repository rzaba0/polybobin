#pragma once
#include <memory>
#include "gl/glcanvas.hpp"
#include <wx/glcanvas.h>
#include "eventdispatcher.hpp"
#include "selectionmanager.hpp"

class MainFrame;

std::unique_ptr<EventDispatcher> CreateEventDispatcher(GLCanvas& canvas, MainFrame& mainFrame, SelectionManager& selectionManager);