#ifndef LAYER_EXTR_HPP
#define LAYER_EXTR_HPP

#include <extractor/extr_base.hpp>
#include <extractor/extr_jkanime.hpp>
#include <extractor/extr_pelisplushd.hpp>

#include <layer/layer_base.hpp>
#include <layer/layer_search.hpp>

namespace layer {
	/*============================================================*/
	class Explore : Layer {
	public:
		Explore();
		void main();
	};
}

#endif
