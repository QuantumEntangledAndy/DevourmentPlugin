#include "models/pred.hpp"
#include "models/prey.hpp"
#include "data/forms.hpp"
#include "models/remapper.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace {
	const float BASE_SWALLOW_RANGE = 225.0;

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

	bool Pred::OnCooldown() {
		return lastSucessfulVoreTime + GetCooldown() <= Time::WorldTimeElapsed();
	}

	float Pred::SwallowRange() {
		return (BASE_SWALLOW_RANGE + GetLength()) * GetScale();
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
		if (!prey) {
			return false;
		}
		if (prey->IsChild()) {
			return false;
		}
		if (prey->IsPlayer() && !CanVorePlayer()) {
			return false;
		}
		if (prey->IsInStomach()) {
			return false;
		}
		if ((pred->GetPosition() - prey->GetPosition()).Length() > SwallowRange()) {
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
		if (prey->HasMagicEffectWithKeyword("BeingSwallowed")) {
			return false;
		}
		bool doBleedoutVore = (GetPluginInfo("SexLabDefeat.esp") == nullptr);
		if (!doBleedoutVore && prey->IsBleedingOut()) {
			return false;
		}

		if (stomach.Room() < prey->GetVolume()) {
			return false;
		}
		return true;
	}

	/**
	 * Vore the target.
	 *
	 * This makes no checks as it assume that they have Already
	 * been made by calling IsValidPrey preior to calling this
	 */
	void Pred::Vore(Pred* prey) {
		Forms::CastSpell(actor->get(), prey->GetActor(), "VoreSpell");
	}

	const Actor* Pred::GetActor() {
		return actor->get();
	}

	Pred::Pred(Actor* actor) : Edible(actor), actor(NiObject<Actor>(actor)) {

	}

}
