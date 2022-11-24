#include "models/stomach.hpp"
#include "models/edible.hpp"
#include "models/pred.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {

	Digestion::Digestion(Edible* edible) : prey(edible) {
	}
	float Digestion::GetVolume() {
		return prey->GetVolume() - digestionProgress;
	}

	Stomach::Stomach(Pred* owner) : owner(owner) {
	}

	float Stomach::Room() {
		return TotalRoom() - GetVolume();
	}
	float Stomach::TotalRoom() {
		return owner->GetBaseVolume() * owner->GetScale() * stretch;
	}
	float Stomach::RoomPercent() {
		return RoomPerunit() * 100.0;
	}
	float Stomach::RoomPerunit() {
		return Room() / TotalRoom();
	}

	float Stomach::GetVolume() {
		float volume = 0.0;
		for (auto digestion: digestions) {
			volume += digestion.Volume();
		}
		return volume;
	}
}
