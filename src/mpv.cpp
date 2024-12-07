#include <mpv.hpp>

namespace mpv {
	/*============================================================*/
	Uint32 wakeup_on_mpv_render_update, wakeup_on_mpv_events;
	mpv_handle *mpv;
	mpv_render_context *mpv_gl;
	int redraw_flag;
	/*============================================================*/
	// MPV callbacks
	void die(const char *msg) {
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
	void *get_proc_address_mpv(void *fn_ctx, const char *name) {
		return SDL_GL_GetProcAddress(name);
	}
	void on_mpv_events(void *ctx) {
		SDL_Event event = {.type = wakeup_on_mpv_events};
		SDL_PushEvent(&event);
	}
	void on_mpv_render_update(void *ctx) {
		SDL_Event event = {.type = wakeup_on_mpv_render_update};
		SDL_PushEvent(&event);
	}
	/*============================================================*/
	void init() {
		mpv = mpv_create();
		if (!mpv)
			die("context init failed");

		//mpv_set_option_string(mpv, "log-file", "mpv-log.txt");

		if (mpv_initialize(mpv) < 0)
			die("mpv init failed");

		mpv_request_log_messages(mpv, "debug");

		mpv_set_option_string(mpv, "vo", "opengl-cb");
		mpv_set_option_string(mpv, "hwdec", "vaapi");

		mpv_opengl_init_params opengl_params{
			.get_proc_address = get_proc_address_mpv
		};

		int advanced_control = 1;
	
		if (!isWayland) {
			Display* x11_display = XOpenDisplay(":0");
			mpv_render_param params[] = {
				{MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL},
				{MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &opengl_params},
				{MPV_RENDER_PARAM_X11_DISPLAY, x11_display},
				{MPV_RENDER_PARAM_ADVANCED_CONTROL, &advanced_control},
				{mpv_render_param{}}
			};
			if (mpv_render_context_create(&mpv_gl, mpv, params) < 0)
				die("failed to initialize mpv GL context");

		}
		else {
			struct wl_display *wl_display = wl_display_connect(NULL);
			mpv_render_param params[] = {
				{MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL},
				{MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &opengl_params},
				{MPV_RENDER_PARAM_WL_DISPLAY, wl_display},
				{MPV_RENDER_PARAM_ADVANCED_CONTROL, &advanced_control},
				{mpv_render_param{}}
			};
			if (mpv_render_context_create(&mpv_gl, mpv, params) < 0)
				die("failed to initialize mpv GL context");
		}

		wakeup_on_mpv_render_update = SDL_RegisterEvents(1);
		wakeup_on_mpv_events = SDL_RegisterEvents(1);
		if (wakeup_on_mpv_render_update == (Uint32)-1 ||
			wakeup_on_mpv_events == (Uint32)-1)
			die("could not register events");

		mpv_set_wakeup_callback(mpv, on_mpv_events, NULL);

		mpv_render_context_set_update_callback(mpv_gl, on_mpv_render_update, NULL);

		redraw_flag = 0;

		mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_INT64);
		mpv_observe_property(mpv, 0, "playback-time", MPV_FORMAT_INT64);
		mpv_observe_property(mpv, 0, "media-title", MPV_FORMAT_STRING);
		mpv_observe_property(mpv, 0, "pause", MPV_FORMAT_FLAG);
		mpv_observe_property(mpv, 0, "seeking", MPV_FORMAT_FLAG);
		mpv_observe_property(mpv, 0, "track-list", MPV_FORMAT_NODE);
	}
	void events(SDL_Event* event) {
		if (event->window.event == SDL_WINDOWEVENT_EXPOSED || event->type == SDL_WINDOWEVENT)
			redraw_flag = 1;

		if (event->type == wakeup_on_mpv_render_update) {
			uint64_t flags = mpv_render_context_update(mpv_gl);
			if (flags & MPV_RENDER_UPDATE_FRAME)
				redraw_flag = 1;
		}

		if (event->type == wakeup_on_mpv_events) {
			while (1) {
				mpv_event *mp_event = mpv_wait_event(mpv, 0);
				if (mp_event->event_id == MPV_EVENT_NONE)
					break;
				if (mp_event->event_id == MPV_EVENT_LOG_MESSAGE) {
					mpv_event_log_message *msg = (mpv_event_log_message*) mp_event->data;
					if (strstr(msg->text, "DR image"))
						printf("log: %s", msg->text);
					continue;
				}
				// if (mp_event->event_id == MPV_EVENT_PROPERTY_CHANGE)
				// 	video_events(mp_event);
				printf("event: %s\n", mpv_event_name(mp_event->event_id));
			}
		}

	}
	void redraw(SDL_Window *window) {
		if (redraw_flag) {
			mpv_opengl_fbo opengl_fbo{
				.fbo = 0,
				.w = (int)win_size.x,
				.h = (int)win_size.y,
			};

			int flip_y = 1;

			mpv_render_param params[] = {
				{MPV_RENDER_PARAM_OPENGL_FBO, &opengl_fbo},
				{MPV_RENDER_PARAM_FLIP_Y, &flip_y},
				{mpv_render_param_type{}}
			};

			mpv_render_context_render(mpv_gl, params);

		}
	}
	void destroy() {
		mpv_render_context_free(mpv_gl);
		mpv_detach_destroy(mpv);
	}
	/*============================================================*/
	// MPV commands
	void load_file(std::string file) {
		const char *cmd[] = {"loadfile", file.c_str(), NULL};
		if (mpv_command_async(mpv, 0, cmd))
			die("amougs");
	}
	void toggle_pause() {
		const char *cmd[] = {"cycle", "pause", NULL};
		mpv_command_async(mpv, 0, cmd);
	}
	void seek(std::string val) {
		const char *cmd[] = {"seek", val.c_str(), "relative", NULL};
		mpv_command_async(mpv, 0, cmd);
	}
	void clear_playlist() {
		const char *cmd[] = {"playlist-remove", "0", NULL};
		mpv_command_async(mpv, 0, cmd);
	}
	void debug_info() {
		const char *cmd[] = {"script-binding", "stats/display-stats-toggle", NULL};
		mpv_command_async(mpv, 0, cmd);
	}
	void show_progress() {
		const char *cmd[] = {"cycle-values", "osd-level", "3", "1",NULL};
		mpv_command_async(mpv, 0, cmd);
	}
	/*============================================================*/
}
