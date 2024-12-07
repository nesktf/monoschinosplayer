#include <layer/layer_base.hpp>

namespace layer {
	/*============================================================*/
	Layer::Layer(const std::string &id) {
		this->id = id;
	}
	std::string Layer::getId() {
		return this->id;
	}
	Layer::~Layer() {}
	void Layer::main() {}
	void Layer::sendEvent(SDL_Event* event) {}
	/*============================================================*/
	LayerStack::LayerStack() {
		this->last = nullptr;
	}
	LayerStack::LayerStack(Layer* start) : LayerStack() {
		this->push(start);
	}
	void LayerStack::push(Layer* layer) {
		last = layer;
		stack.push_back(layer);
		std::cout << last->getId() << std::endl;
	}
	void LayerStack::pop() {
		layer::Layer* prev = stack.back();
		stack.pop_back();
		last = stack.back();
		delete prev;
	}
	void LayerStack::render() {
		this->last->main();
	}
	int LayerStack::size() {
		return this->stack.size();
	}
	void LayerStack::sendEvent(SDL_Event* event) {
		this->last->sendEvent(event);
	}
	/*============================================================*/
}
