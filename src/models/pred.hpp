#pragma once
/**
 * Handles predator related functions
 */

#include "moduels/edible.hpp"
#include "models/stomach.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;


namespace Devr {
	class Prey;

	class Pred : public PredPrey {
		public:
			static Pred* FromActor(Actor* actor);

			virtual Pred* AsPred() {
				return this;
			}

			bool IsValidPred();
			bool IsValidPrey(Pred* pred);
			float GetCooldown();

			bool CanNecroVore();

		protected:
			Pred(Actor* actor);
			NiPointer<Actor> actor;
			Stomach stomach;
	};
}
