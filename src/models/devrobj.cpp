#include "models/devrobj.hpp"

using namespace std;
using namespace SKSE;
using namespace RE;

namespace Devr {
	/**
	 * Convience function to get the name of a pred`
	 */
	std::string DevrObj::Name() {
		return object->GetDisplayFullName();
	}

	/**
	 * Get length based on bbox
	 */
	float DevrObj::GetLength() {
		auto max = object->GetBoundMax();
		auto min = object->GetBoundMin();
		auto deltaY = max.y - min.y;

		return deltaY * GetScale();
	}

	/**
	 * Get the scale of the pred this include all effects
	 * such as those made by Giantess Mod or by RaceMenu
	 * or by SetScale
	 */
	float DevrObj::GetScale() {
		auto refScale = static_cast<float>(object->refScale) / 100.0F; // This is the scale used by SetScale papyrus

		// This is the model scale which includes aspects like giantess mod and racemenus height
		auto modelScale = 1.0;
		auto model = object->GetCurrent3D();
		if (model) {
			auto rootNode = model->GetObjectByName("NPC Root [Root]");
			if (rootNode) {
				modelScale = rootNode->world.scale;
			}
		}

		return refScale * modelScale;
	}

	/**
	 * Get position, used for distance and in front calcs
	 */
	const NiPoint3 DevrObj::GetPosition() {
		return object->GetPosition();
	}


	/**
	 * This is the base volume not including nesting
	 */
	float DevrObj::GetBaseVolume() {
		auto max = object->GetBoundMax();
		auto min = object->GetBoundMin();
		auto deltaX = max.x - min.x;
		auto deltaY = max.y - min.y;
		auto deltaZ = max.z - min.z;

		return (deltaX * deltaY * deltaZ)*GetScale();
	}

	float DevrObj::GetVolume() {
		return GetBaseVolume(); // No stomach at base level see Pred for + stomach volume
	}

	/**
	 * Keywords are used to determine if
	 * they have certain resistances such
	 * as arising from being a deadra
	 */
	bool DevrObj::HasKeywordString(std::string_view keyword) {
		return object->HasKeywordString(keyword);
	}

	/**
	 * Certain magic effects will cause the
	 * vore to fail these are checked with this
	 */
	bool DevrObj::HasMagicEffectWithKeyword(std::string_view keyword) {
		auto magicTarget = object->GetMagicTarget();
		if (magicTarget) {
			for (auto meff: magicTarget->GetActiveEffectList()) {
				auto spell = meff->GetBaseObject();
				if (spell->HasKeywordString(keyword)) {
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * Used to determine if necrovore is required
	 */
	bool DevrObj::IsDead() {
		return this->IsDead();
	}

	DevrObj::DevrObj(TESObjectREFR* inobject) : object(NiPointer<TESObjectREFR>(inobject)) {

	}

}
