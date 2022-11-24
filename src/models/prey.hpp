#pragma once
/**
 * Handles prey related functions
 */

#include "models/edible.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	class Prey : public Edible {
		public:
			static Prey& FromActor(Actor* actor);

			bool IsValidPrey();
		protected:
			Prey(Actor* actor);
			NiPointer<Actor> actor;
	};
}
