#pragma once
/**
 * Handles predator related functions
 */

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	class Stomach;

	/**
	 * A collections of functions common to pred and prey
	 * @return [description]
	 */
	class Edible {
		public:
			std::string Name();
			float GetLength();
			float GetScale();

			bool IsInFaction(std::string_view tag);
			bool HasPrek(std::string_view tag);
			bool HasKeywordString(std::string_view keyword);
			bool IsInCombat();
			Actor* GetCombatTarget();
			ATTACK_STATE_ENUM GetAttackState();
			const NiPoint3 GetPosition();
			bool IsBleedingOut();
			bool hasMagicEffectWithKeyword(std::string_view keywork);
			bool IsChild();
			bool IsPlayer();

			SEX GetSex();

			float GetBaseVolume(); // Volume of actor not including stomach contents
			float GetVolume(); // Volume of actor including stomach contents
		protected:
			NiPointer<Actor> actor;

			Edible(TESObjectREFR* object);

			Stomach stomach; // TODO: Fix this so that it points to same object weather Pred or Prey
	};
}
