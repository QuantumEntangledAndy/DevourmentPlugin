#include "models/personas/devractor.hpp"
#include "data/time.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace {
	const float COOLDOWN = 4.0;
	const float BASE_SWALLOW_RANGE = 225.0;

	bool AllowMalePreds() {
		return true; // TODO
	}

	bool AllowFemalePreds() {
		return true; // TODO
	}

	bool IsGenderless(Actor* actor) {
		return false; // TODO
	}
}
namespace Devr {
	bool DevrActor::OnCooldown() {
		return lastSucessfulVoreTime + COOLDOWN <= Time::WorldTimeElapsed();
	}

	float DevrActor::SwallowRange() {
		return (BASE_SWALLOW_RANGE + GetLength()) * GetScale();
	}

	bool DevrActor::ValidPred() {
		if (!Pred::ValidPred(actor)) {
			return false;
		} else if (IsInWhitelist(actor)) {
			return true;
		} else if (IsChild()) {
			return false;
		} else if (IsGenderless(actor)) {
			return true;
		} else if (GetSex() == SEX::kNone) {
			return true;
		} else if (AllowMalePreds() && GetSex() == SEX::kMale) {
			return true;
		} else if (AllowFemalePreds() && GetSex() == SEX::kFemale) {
			return true;
		} else {
			return false;
		}
	}

	bool DevrActor::IsValidPrey() {
		bool doBleedoutVore = (GetPluginInfo("SexLabDefeat.esp") == nullptr);
		if (!doBleedoutVore && actor->IsBleedingOut()) {
			return false;
		}
		if (actor->IsChild()) {
			return false;
		}
		if (IsInStomach()) {
			return false;
		}
		if (HasMagicEffectWithKeyword("BeingSwallowed")) {
			return false;
		}
		return true;
	}

	/**
	 * Check if this pred can devrour this prey
	 */
	bool DevrActor::CanDevrPrey(Prey* prey, std::string_view stomach_name) {
		if (OnCooldown()) {
			return false;
		}
		if (!prey) {
			return false;
		}
		if (!Pred::CanDevrPrey(prey, CanDevrPrey)) {
			return false;
		}
		if ((GetPosition() - prey->GetPosition()).Length() > SwallowRange()) {
			return false;
		}
		if (prey->IsDead() && !IsInFaction("CorpseVore")) {
			return false;
		}
		if (prey->HasKeywordString("ActorTypeUndead") && !HasPerk("DigestionUndead")) {
			return false;
		}
		if (prey->HasKeywordString("ActorTypeDaedra") && !HasPerk("DigestionDaedric")) {
			return false;
		}
		if (prey->HasKeywordString("ActorTypeDwarven") && !HasPerk("DigestionDwemer")) {
			return false;
		}
		return true;
	}

	bool DevrActor::IsInCombat() {
		return actor->IsInCombat();
	}

	Actor* DevrActor::GetCombatTarget() {
		auto target = actor->currentCombatTarget;
		if (target) {
			return target.get();
		} else {
			return nullptr;
		}
	}

	ATTACK_STATE_ENUM DevrActor::GetAttackState() {
		return actor->GetAttackState();
	}

	SEX DevrActor::GetSex() {
		return actor->GetActorBase()->GetSex();
	}

	bool DevrObj::IsBleedingOut() {
		auto actor = skyrim_cast<Actor*>(object.get());
		if (actor) {
			return actor->IsBleedingOut();
		} else {
			return false;
		}
	}

	/**
	 * Vore the target.
	 *
	 * This makes no checks as it assume that they have Already
	 * been made by calling IsValidPrey preior to calling this
	 */
	void DevrActor::Vore(Pred* prey) {
		lastSucessfulVoreTime = Time::WorldTimeElapsed();
		Forms::CastSpell(actor->get(), prey->GetActor(), "VoreSpell");
	}


	DevrActor::DevrActor(Actor* actor) : Pred(actor), actor(NiObject<Actor>(actor)) {

	}

}
