#include "models/prey.hpp"
#include "data/forms.hpp"
#include "models/remapper.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace {
	bool CanVorePlayer() {
		return true; // TODO: Add player check
	}
}

namespace Devr {
	/**
	 * Get the prey data for an actor/object
	 * @param  actor               [description]
	 * @return       [description]
	 */
	Prey* Prey::FromActor(Actor* actor) {
		static std::mutex predDataLock;

		// Lock this to avoid concurrent access to preyData
		std::unique_lock lock(predDataLock);
		static std::unordered_map<Actor*, unique_ptr<Prey> > preyData;
		if (preyData.find(actor) == preyData.end()) {
			// Insert it
			preyData.try_emplace(actor);
		}

		// If the prey is not valid then we return nullptr
		auto prey = preyData.at(actor);
		if (prey.IsValidPrey()) {
			return &prey;
		} else {
			return nullptr;
		}
	}

	bool Prey::IsValidPrey() {
		if (actor->IsChild()) {
			return false;
		}
		if (actor->formID == 0x14 && !CanVorePlayer()) {
			return false;
		}
		return true;
	}

	Prey::Prey(Actor* actor) : Edible(actor), actor(NiPointer<Actor>(actor)) {

	}

}
