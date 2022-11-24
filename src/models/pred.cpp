#include "models/pred.hpp"
#include "models/prey.hpp"
#include "data/forms.hpp"
#include "models/remapper.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace {
	bool IsInBlacklist(Actor* actor) {
		return false; // TODO: Add actual blacklist logic
	}

	bool IsInWhitelist(Actor* actor) {
		return false; // TODO: Add actual whitelist logic
	}

	bool AllowCreaturePreds() {
		return true; // TODO: Add creature pred logic
	}

	bool IsValidCreaturePred(actor* actor) {
		// TESRace* baseRace = Remapper::Race(actor->getRace());
		// if (baseRace) {
		//
		// }
		// int index = CreaturePredatorRaces.Find(baseRace)
		// return index >= 0 && CreaturePredatorToggles[index]
		return true; // TODO: Add creature allowed logic
	}

	bool Pred::AllowMalePreds() {
		return true; // TODO: Add male pred logic
	}

	bool Pred::AllowFemalePreds() {
		return true; // TODO: Add female pred logic
	}

	bool Pred::IsHumanoidPred(Actor* actor) {
		// int sex = target.getLeveledActorBase().getSex()	;We only care for Sex where humanoids are concerned.
		// Race baseRace = Remapper.RemapRace(target.GetLeveledActorBase().getRace())
		// int index = HumanoidPredatorRaces.Find(baseRace)
		return true; // TODO: Add actual logic
	}
}

namespace Devr {
	/**
	 * Get the pred data for an actor
	 * @param  actor               [description]
	 * @return       [description]
	 */
	Pred* Pred::AsPred(Actor* actor) {
		static std::mutex predDataLock;

		// Lock this to avoid concurrent access to predData
		std::unique_lock lock(predDataLock);
		static std::unordered_map<Actor*, Pred> predData;
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

	float Pred::GetCooldown() {
		if (HasKeywordString("ActorTypeNPC")) {
			return 4.0;
		} else {
			return 6.0;
		}
	}

	bool Pred::ValidPred() {
		if (actor.IsChild() || IsInBlacklist(actor)) {
			return false;
		} else if (IsInWhitelist(actor)) {
			return true;
		} else if (HasKeywordString("ActorTypeCreature") && AllowCreaturePreds() && IsValidCreaturePred(actor)) {
			return true;
		} else if (HasKeywordString("ActorTypeNPC") && !IsHumanoidPred(actor)) {
			return true;
		} else if (HasKeywordString("ActorTypeNPC") && AllowMalePreds() && GetSex() == SEX::kMale) {
			return true;
		} else if (HasKeywordString("ActorTypeNPC") && AllowFemalePreds() && GetSex() == SEX::kFemale) {
			return true;
		} else if (HasKeywordString("ActorTypeNPC") && GetSex() == SEX::kNone) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Check if this pred can vore this prey
	 * @param  actor               [description]
	 * @return       [description]
	 */
	bool Pred::IsValidPrey(Prey* prey) {
		if (!prey.IsValidPrey()) {
			return false;
		}
		if (pred->IsDead() && !IsInFaction("CorpseVore")) {
			return false;
		}
		if (pred->HasKeywordString("ActorTypeUndead") && !HasPerk("DigestionUndead")) {
			return false;
		}
		if (pred->HasKeywordString("ActorTypeDaedra") && !HasPerk("DigestionDaedric")) {
			return false;
		}
		if (pred->HasKeywordString("ActorTypeDwarven") && !HasPerk("DigestionDwemer")) {
			return false;
		}
		if (stomach.Room() < prey->GetVolume()) {
			return false;
		}
		return true;
	}

	Pred::Pred(Actor* actor) : Edible(actor), actor(NiObject<Actor>(actor)) {

	}

}
