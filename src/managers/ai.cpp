#include "managers/ai.hpp"
#include "data/actors.hpp"
#include "models/pred.hpp"

namespace Devr {
	void Ai::Update() {
		for (auto actor: FindActors()) {
			auto pred = Pred::GetPred(actor);
			std::string predName = pre.Name();
			float reach = swallowRange + pred.GetLength();

		}
	}
}
