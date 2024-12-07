#ifndef LAYER_LOADING_HPP
#define LAYER_LOADING_HPP

#include <layer/layer_base.hpp>
#include <widgets/widget_spinner.hpp>

namespace layer {
	class LoadingScreen : Layer {
	public:
		LoadingScreen(layer::LayerStack* parent);
		void main();
	};
}
#endif
