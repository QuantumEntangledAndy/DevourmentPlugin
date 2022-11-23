#include "models/pred.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	/**
	 * Get the pred data for an actor
	 * @param  actor               [description]
	 * @return       [description]
	 */
	Pred* Pred::GetPred(Actor* actor) {
		static std::mutex predDataLock;

		// Lock this to avoid concurrent access to predData
		std::unique_lock lock(predDataLock);
		static std::unordered_map<Actor*, Pred> predData;
		if (predData.find(actor) == predData.end()) {
			// Insert it
			predData.try_emplace(actor);
		}

		return predData.at(actor);
	}

	/**
	 * Convience function to get the name of a pred`
	 * @return [description]
	 */
	std::string Pred::Name() {
		return actor->GetDisplayFullName();
	}

	/**
	 * Get the scale of the pred this include all effects
	 * such as those made by Giantess Mod or by RaceMenu
	 * or by SetScale
	 * @return [description]
	 */
	float Pred::GetScale() {
		auto refScale = static_cast<float>(actor->refScale) / 100.0F; // This is the scale used by SetScale papyrus

		// This is the model scale which includes aspects like giantess mod and racemenus height
		auto modelScale = 1.0;
		auto model = actor->GetCurrent3D();
		if (model) {
			auto rootNode = model->GetObjectByName("NPC Root [Root]");
			if (rootNode) {
				modelScale = rootNode->world.scale;
			}
		}

		return refScale * modelScale;
	}

	float Pred::GetLength() {
		auto max = actor->GetBoundMax();
		auto min = actor->GetBoundMin();
		auto deltaY = max.y - min.y;

		return deltaY * GetScale();
	}

	Pred::Pred(Actor* actor) : actor(NiPointer<Actor>(actor)) {

	}

}
