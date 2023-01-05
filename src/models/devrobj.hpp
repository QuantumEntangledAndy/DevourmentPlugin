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
	 * A collections of common functions
	 */
	class DevrObj {
		public:
			virtual std::string Name();
			virtual float GetLength();
			virtual float GetScale();
			const NiPoint3 GetPosition();

			virtual float GetBaseVolume(); // Volume of actor not including stomach contents
			virtual float GetVolume(); // Volume of actor including stomach contents

			bool HasKeywordString(std::string_view keyword);
			bool HasMagicEffectWithKeyword(std::string_view keyword);
			bool IsDead();
		protected:
			NiPointer<TESObjectREFR> object;

			Edible(TESObjectREFR* object);
	};
}
