#include "LayerStack.h"

namespace BtnSqd {
	LayerStack::~LayerStack() {
		for (Layer* ptr : layers) {
			delete ptr;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		layers.emplace(layers.begin()+layerInsertOffset,layer);
		layerInsertOffset++;
	}
	void LayerStack::PushOverlay(Layer* layer){
		layers.emplace_back(layer);
	}
	void LayerStack::PopLayer(Layer* layer){
		auto pos = std::find(layers.begin(),layers.begin()+layerInsertOffset,layer);
		if (pos!=layers.begin()+layerInsertOffset) {
			layer->OnDetach();
			layers.erase(pos);
			layerInsertOffset--;
		}
	}
	void LayerStack::PopOverlay(Layer* overlay){
		auto pos = std::find(layers.begin()+layerInsertOffset, layers.end(), overlay);
		if (pos != layers.end()) {
			overlay->OnDetach();
			layers.erase(pos);
		}
	}
}