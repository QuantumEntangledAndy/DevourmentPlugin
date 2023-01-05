#pragma once
/**
 * Handles predator related functions
 */

#include "moduels/prey.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;


namespace Devr {
	class Prey;

	class Pred : public Prey {
		public:
			virtual bool IsValidPred();
			virtual bool CanDevrPrey(Prey* prey, std::string_view stomach);
			virtual void Vore(Prey* prey) = 0;

			Stomach* GetStomach(std::string_view name);

		protected:
			Pred(TESObjectREFR* object);
			std::unordered_map<std::string, std::unique_ptr<Stomach> > stomachs;
	};
}
