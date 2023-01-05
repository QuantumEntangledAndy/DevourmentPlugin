#pragma once
/**
 * Handles predator related functions
 */

#include "models/prey.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;


namespace Devr {
	class DevrActor : public Pred {
		public:
			// Pred
			virtual bool IsValidPred() override;
			virtual bool CanDevrPrey(Prey* prey)  override;
			virtual void Vore(Prey* prey) override;

			// Prey
			virtual bool IsValidPrey() override;

			// New
			virtual float SwallowRange() override;
			virtual bool OnCooldown();

			// Actor Related
			bool IsChild();
			SEX GetSex();

			bool IsInFaction(std::string_view tag);
			bool HasPerk(std::string_view tag);

			bool IsInCombat();
			Actor* GetCombatTarget();
			ATTACK_STATE_ENUM GetAttackState();
			bool IsBleedingOut();

		protected:
			DevrActor(Actor* actor);
			NiPointer<Actor> actor;
			float lastSucessfulVoreTime = -1e8;
	};
}
