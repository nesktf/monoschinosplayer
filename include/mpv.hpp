#ifndef MPV_HPP
#define MPV_HPP

#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <layer/layer_base.hpp>
#include <X11/Xlib.h>
#include <wayland-client.h>

extern bool isWayland;

namespace mpv {
	/*============================================================*/
	void init();
	void events(SDL_Event* event);
	void destroy();
	void redraw(SDL_Window* window);
	/*============================================================*/
	void load_file(std::string file);
	void toggle_pause();
	void seek(std::string val);
	void clear_playlist();
	void debug_info();
	void show_progress();
	/*============================================================*/
}

#endif
