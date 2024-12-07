#include <gui.hpp>

/*============================================================*/
SDL_Window* window;
SDL_GLContext gl_context;
ImVec2 win_size = ImVec2(1280.0f, 720.0f);
bool done = false;
bool fullscreen = false;
const char* glsl_version = "#version 130";
bool isWayland;
/*============================================================*/
int init() {
	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
	SDL_Init(0);
	if (!SDL_VideoInit("wayland")){
		isWayland = true;
		std::cout << "Wayland" << std::endl;
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
	}
	else {
		isWayland = false;
		std::cout << "X11" << std::endl;
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
		SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"); // keep compositer enabled
		SDL_SetHint(SDL_HINT_VIDEO_X11_FORCE_EGL, "1"); // mpv hwdec X11 (force egl)
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
	//SDL_SetHint(SDL_HINT_VIDEO_X11_FORCE_EGL, "1"); // mpv hwdec X11

	SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("MonosChinosPlayer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); //Vsync;
	return 0;
}
/*============================================================*/
int main(int, char**) {
	// /*
	if (!init()) {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init(glsl_version);

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = NULL;


		mpv::init();
		SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "no");

		gui::renderLoop();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		mpv::destroy();

		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	// */
	return 0;
}
/*============================================================*/
