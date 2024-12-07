#ifndef GUI_HPP
#define GUI_HPP

#include <mpv.hpp>

#include <layer/layer_base.hpp>
#include <layer/layer_explore.hpp>
#include <layer/layer_player.hpp>

#include <extractor/extr_base.hpp>
// #include <GL/glew.h>
// extern SDL_Window* window;
// extern SDL_GLContext gl_context;

namespace gui {
	void renderLoop();
	void eventHandler();
	void mainLayer();
}

#endif
