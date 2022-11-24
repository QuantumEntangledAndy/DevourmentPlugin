#pragma once
/**
 * Handles stomach related functions
 */

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {

	class Edible;
	class Pred;

	/**
	 * The stomach object
	 */
	class Digestion {
		public:
			Digestion(Edible* edible);
			float GetVolume()
		protected:
			float digestionProgress = 0.0;
			Edible* prey;
	};

	class Stomach {
		public:
			Stomach(Pred* owner)
			float Room();
			float TotalRoom();
			float RoomPercent();
			float RoomPerunit();
			float GetVolume();
		protected:
			Pred* owner;
			std::vector<Digestion> digestions;
			float digestionStrength = 100.0; // How much maximum volume of prey to digest each second
			float strech = 1.5; // The factor of the owners volume we can stretch too
	};
}
