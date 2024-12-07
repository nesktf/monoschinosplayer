#ifndef LAYER_BASE_HPP
#define LAYER_BASE_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <list>
#include <string>
#include <future>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <extractor/extr_base.hpp>

extern bool done;
extern bool fullscreen;
extern ImVec2 win_size;
extern SDL_Window* window;

namespace layer {
	/*============================================================*/
	class LayerStack;
	/*============================================================*/
	enum LayerId {
		EPLIST,
		SVSEL,
		PLAYER
	};
	enum EventID {
		INITPLAYER,
		QUITPLAYER,
		PUSHLAYER,
		POPLAYER
	};
	/*============================================================*/
	class Layer {
	protected:
		std::string id;
	public:
		Layer(const std::string &id);
		virtual void main();
		virtual ~Layer();
		virtual void sendEvent(SDL_Event* event);
		std::string getId();
	};
	/*============================================================*/
	class LayerStack {
	private:
		std::list<Layer*> stack;
		Layer* last;
	public:
		LayerStack();
		LayerStack(Layer* start);
		void push(Layer* layer);
		void pop();
		void render();
		int size();
		void sendEvent(SDL_Event* event);
	};
	/*============================================================*/
}
#endif
