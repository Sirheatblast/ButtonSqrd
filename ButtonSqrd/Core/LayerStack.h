#pragma once
#include"core.h"
#include"layer.h"

#include<vector>

namespace BtnSqd {
	class LayerStack {
	public:
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return layers.begin(); }
		std::vector<Layer*>::const_iterator end() const{ return layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const{ return layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const{ return layers.rend(); }

	private:
		std::vector<Layer*> layers;
		unsigned int layerInsertOffset=0;
	};
}