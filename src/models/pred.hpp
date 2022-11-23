#pragma once
/**
 * Handles predator related functions
 */

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	class Pred {
		public:
			static Pred& GetPred(Actor* actor);
			std::string Name();
			float GetLength();
			float GetScale();

		protected:
			Pred(Actor* actor);
		private:
			NiPointer<Actor> actor;
	};
}
