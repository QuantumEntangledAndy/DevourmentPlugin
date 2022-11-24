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
			bool HasKeywordString(std::string_view keyword);
			bool IsInCombat();

			SEX GetSex();

			float GetBaseVolume(); // Volume of actor not including stomach contents
			float GetVolume(); // Volume of actor including stomach contents
		protected:
			NiPointer<Actor> actor;

			Edible(TESObjectREFR* object);

			Stomach stomach;
	};
}
