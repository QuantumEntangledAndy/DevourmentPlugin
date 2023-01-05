#include "models/pred.hpp"
#include "managers/predators.hpp"

namespace Devr {
	Predators& Predators::GetSingleton() {
		static Predators instance;
		return instance;
	}

	Pred* Predators::GetPred(Actor* actor) {
		// Lock this to avoid concurrent access to predData
		auto& predators = Predators::GetSingleton();
		std::unique_lock lock(predators.predDataLock);
		auto& predData = predators.predData;
		if (predData.find(actor) == predData.end()) {
			// Insert it
			predData.try_emplace(actor);
		}

		// If the pred is not valid then we return nullptr
		auto pred = predData.at(actor);
		if (pred.IsValidPred()) {
			return &pred;
		} else {
			return nullptr;
		}
	}

	void Predators::Update() {

	}
}
