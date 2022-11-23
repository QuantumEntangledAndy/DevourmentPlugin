#include "data/forms.hpp"
#include <articuno/archives/ryml/ryml.h>
#include <articuno/types/auto.h>

using namespace articuno;
using namespace articuno::ryml;
using namespace SKSE;
using namespace RE;


namespace {
	template <class T>
	T* find_form(std::string_view lookup_id) {
		// From https://github.com/Exit-9B/MCM-Helper/blob/a39b292909923a75dbe79dc02eeda161763b312e/src/FormUtil.cpp
		std::string lookup_id_str(lookup_id);
		std::istringstream ss{ lookup_id_str };
		std::string plugin, id;

		std::getline(ss, plugin, '|');
		std::getline(ss, id);
		RE::FormID relativeID;
		std::istringstream{ id } >> std::hex >> relativeID;
		const auto dataHandler = RE::TESDataHandler::GetSingleton();
		return dataHandler ? dataHandler->LookupForm<T>(relativeID, plugin) : nullptr;
	}

	struct FormsConfig {
		std::unordered_map<std::string, std::string> sounds;
		std::unordered_map<std::string, std::string> spellEffects;
		std::unordered_map<std::string, std::string> spells;
		std::unordered_map<std::string, std::string> perks;
		std::unordered_map<std::string, std::string> explosions;
		std::unordered_map<std::string, std::string> globals;
		std::unordered_map<std::string, std::string> quests;
		std::unordered_map<std::string, std::string> factions;
		std::unordered_map<std::string, std::string> impacts;
		std::unordered_map<std::string, std::string> races;

		articuno_serde(ar) {
			ar <=> kv(sounds, "sounds");
			ar <=> kv(spellEffects, "spellEffects");
			ar <=> kv(spells, "spells");
			ar <=> kv(perks, "perks");
			ar <=> kv(explosions, "explosions");
			ar <=> kv(globals, "globals");
			ar <=> kv(quests, "quests");
			ar <=> kv(factions, "factions");
			ar <=> kv(impacts, "impacts");
			ar <=> kv(races, "races");
		}
	};
}

namespace Devr {
	Forms& Forms::GetSingleton() noexcept {
		static Forms instance;
		return instance;
	}

	std::string Forms::DebugName() {
		return "Forms";
	}


	// Sound
	BSISoundDescriptor* Forms::GetSound(const std::string_view& tag) {
		BSISoundDescriptor* data = nullptr;
		try {
			data = Forms::GetSingleton().sounds.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
			if (!Forms::Logged("sond", tag)) {
				log::warn("Sound: {} not found", tag);
			}
		}
		return data;
	}
	void Forms::PlaySound(const std::string_view& tag, Actor* actor, const float& volume, const float& frequency) {
		auto soundDescriptor = Forms::GetSound(tag);
		if (!soundDescriptor) {
			log::error("Sound invalid");
			return;
		}
		auto audioManager = BSAudioManager::GetSingleton();
		if (!audioManager) {
			log::error("Audio Manager invalid");
			return;
		}
		BSSoundHandle soundHandle;
		bool success = audioManager->BuildSoundDataFromDescriptor(soundHandle, soundDescriptor);
		if (success) {
			//soundHandle.SetFrequency(frequency);
			soundHandle.SetVolume(volume);
			NiAVObject* follow = nullptr;
			if (actor) {
				NiAVObject* current_3d = actor->GetCurrent3D();
				if (current_3d) {
					follow = current_3d;
				}
			}
			soundHandle.SetObjectToFollow(follow);
			soundHandle.Play();
		} else {
			log::error("Could not build sound");
		}
	}

	// Spell Effects
	EffectSetting* Forms::GetMagicEffect(const std::string_view& tag) {
		EffectSetting* data = nullptr;
		try {
			data = Forms::GetSingleton().spellEffects.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			if (!Forms::Logged("mgef", tag)) {
				log::warn("MagicEffect: {} not found", tag);
			}
			data = nullptr;
		}
		return data;
	}

	bool Forms::HasMagicEffect(Actor* actor, const std::string_view& tag) {
		return Forms::HasMagicEffectOr(actor, tag, false);
	}

	bool Forms::HasMagicEffectOr(Actor* actor, const std::string_view& tag, const bool& default_value) {
		auto data = Forms::GetMagicEffect(tag);
		if (data) {
			return actor->HasMagicEffect(data);
		} else {
			return default_value;
		}
	}

	// Spells
	SpellItem* Forms::GetSpell(const std::string_view& tag) {
		SpellItem* data = nullptr;
		try {
			data = Forms::GetSingleton().spells.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			if (!Forms::Logged("spel", tag)) {
				log::warn("Spell: {} not found", tag);
			}
			data = nullptr;
		}
		return data;
	}

	void Forms::AddSpell(Actor* actor, const std::string_view& tag) {
		auto data = Forms::GetSpell(tag);
		if (data) {
			if (!Forms::HasSpell(actor, tag)) {
				actor->AddSpell(data);
			}
		}
	}
	void Forms::RemoveSpell(Actor* actor, const std::string_view& tag) {
		auto data = Forms::GetSpell(tag);
		if (data) {
			if (Forms::HasSpell(actor, tag)) {
				actor->RemoveSpell(data);
			}
		}
	}

	bool Forms::HasSpell(Actor* actor, const std::string_view& tag) {
		return Forms::HasSpellOr(actor, tag, false);
	}

	bool Forms::HasSpellOr(Actor* actor, const std::string_view& tag, const bool& default_value) {
		auto data = Forms::GetSpell(tag);
		if (data) {
			return actor->HasSpell(data);
		} else {
			return default_value;
		}
	}

	void Forms::CastSpell(Actor* caster, Actor* target, const std::string_view& tag) {
		auto data = GetSpell(tag);
		if (data) {
			caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(data, false, target, 1.00f, false, 0.0f, caster);
		}
	}

	// Perks
	BGSPerk* Forms::GetPerk(const std::string_view& tag) {
		BGSPerk* data = nullptr;
		try {
			data = Forms::GetSingleton().perks.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
			if (!Forms::Logged("perk", tag)) {
				log::warn("Perk: {} not found", tag);
			}
		}
		return data;
	}

	void Forms::AddPerk(Actor* actor, const std::string_view& tag) {
		auto data = Forms::GetPerk(tag);
		if (data) {
			if (!Forms::HasPerk(actor, tag)) {
				actor->AddPerk(data);
			}
		}
	}
	void Forms::RemovePerk(Actor* actor, const std::string_view& tag) {
		auto data = Forms::GetPerk(tag);
		if (data) {
			if (Forms::HasPerk(actor, tag)) {
				actor->RemovePerk(data);
			}
		}
	}

	bool Forms::HasPerk(Actor* actor, const std::string_view& tag) {
		return Forms::HasPerkOr(actor, tag, false);
	}

	bool Forms::HasPerkOr(Actor* actor, const std::string_view& tag, const bool& default_value) {
		auto data = Forms::GetPerk(tag);
		if (data) {
			return actor->HasPerk(data);
		} else {
			return default_value;
		}
	}

	// Explosion
	BGSExplosion* Forms::GetExplosion(const std::string_view& tag) {
		BGSExplosion* data = nullptr;
		try {
			data = Forms::GetSingleton().explosions.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
			if (!Forms::Logged("expl", tag)) {
				log::warn("Explosion: {} not found", tag);
			}
		}
		return data;
	}

	void Forms::CreateExplosion(Actor* actor, const float& scale, const std::string_view& tag) {
		if (actor) {
			CreateExplosionAtPos(actor, actor->GetPosition(), scale, tag);
		}
	}

	void Forms::CreateExplosionAtNode(Actor* actor, const std::string_view& node_name, const float& scale, const std::string_view& tag) {
		if (actor) {
			if (actor->Is3DLoaded()) {
				auto model = actor->GetCurrent3D();
				if (model) {
					auto node = model->GetObjectByName(std::string(node_name));
					if (node) {
						CreateExplosionAtPos(actor, node->world.translate, scale, tag);
					}
				}
			}
		}
	}

	void Forms::CreateExplosionAtPos(Actor* actor, NiPoint3 pos, const float& scale, const std::string_view& tag) {
		auto data = GetExplosion(tag);
		if (data) {
			NiPointer<TESObjectREFR> instance_ptr = actor->PlaceObjectAtMe(data, false);
			if (!instance_ptr) {
				return;
			}
			TESObjectREFR* instance = instance_ptr.get();
			if (!instance) {
				return;
			}
			instance->SetPosition(pos);
			Explosion* explosion = instance->AsExplosion();
			explosion->radius *= scale;
			explosion->imodRadius *= scale;
		}
	}

	// Globals
	TESGlobal* Forms::GetGlobal(const std::string_view& tag) {
		TESGlobal* data = nullptr;
		try {
			data = Forms::GetSingleton().globals.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
			if (!Forms::Logged("glob", tag)) {
				log::warn("Global: {} not found", tag);
			}
		}
		return data;
	}

	bool Forms::GetBool(const std::string_view& tag) {
		return Forms::GetBoolOr(tag, false);
	}

	bool Forms::GetBoolOr(const std::string_view& tag, const bool& default_value) {
		auto data = GetGlobal(tag);
		if (data) {
			return fabs(data->value - 0.0) > 1e-4;
		} else {
			return default_value;
		}
	}

	void Forms::SetBool(const std::string_view& tag, const bool& value) {
		auto data = GetGlobal(tag);
		if (data) {
			if (value) {
				data->value = 1.0;
			} else {
				data->value = 0.0;
			}
		}
	}

	int Forms::GetInt(const std::string_view& tag) {
		return Forms::GetIntOr(tag, false);
	}

	int Forms::GetIntOr(const std::string_view& tag, const int& default_value) {
		auto data = GetGlobal(tag);
		if (data) {
			return data->value;
		} else {
			return default_value;
		}
	}

	void Forms::SetInt(const std::string_view& tag, const int& value) {
		auto data = GetGlobal(tag);
		if (data) {
			data->value = value;
		}
	}

	float Forms::GetFloat(const std::string_view& tag) {
		return Forms::GetFloatOr(tag, false);
	}

	float Forms::GetFloatOr(const std::string_view& tag, const float& default_value) {
		auto data = GetGlobal(tag);
		if (data) {
			return data->value;
		} else {
			return default_value;
		}
	}

	void Forms::SetFloat(const std::string_view& tag, const float& value) {
		auto data = GetGlobal(tag);
		if (data) {
			data->value = value;
		}
	}

	// Quests
	TESQuest* Forms::GetQuest(const std::string_view& tag) {
		TESQuest* data = nullptr;
		try {
			data = Forms::GetSingleton().quests.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
			if (!Forms::Logged("qust", tag)) {
				log::warn("Quest: {} not found", tag);
			}
		}
		return data;
	}

	std::uint16_t Forms::GetStage(const std::string_view& tag) {
		return Forms::GetStageOr(tag, 0);
	}

	std::uint16_t Forms::GetStageOr(const std::string_view& tag, const std::uint16_t& default_value) {
		auto data = GetQuest(tag);
		if (data) {
			return data->GetCurrentStageID();
		} else {
			return default_value;
		}
	}

	// Factions
	TESFaction* Forms::GetFaction(const std::string_view& tag) {
		TESFaction* data = nullptr;
		try {
			data = Forms::GetSingleton().factions.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
		}
		return data;
	}


	bool Forms::InFaction(Actor* actor, const std::string_view& tag) {
		return Forms::InFactionOr(actor, tag, false);
	}

	bool Forms::InFactionOr(Actor* actor, const std::string_view& tag, const bool& default_value) {
		auto data = GetFaction(tag);
		if (data) {
			return actor->IsInFaction(data);
		} else {
			return default_value;
		}
	}

	// Impacts
	BGSImpactDataSet* Forms::GetImpactEffect(const std::string_view& tag) {
		BGSImpactDataSet* data = nullptr;
		try {
			data = Forms::GetSingleton().impacts.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
			if (!Forms::Logged("impc", tag)) {
				log::warn("ImpactEffect: {} not found", tag);
			}
		}
		return data;
	}
	void Forms::PlayImpactEffect(Actor* actor, const std::string_view& tag, const std::string_view& node, NiPoint3 direction, const float& length, const bool& applyRotation, const bool& useLocalRotation) {
		auto data = GetImpactEffect(tag);
		if (data) {
			auto impact = BGSImpactManager::GetSingleton();
			impact->PlayImpactEffect(actor, data, node, direction, length, applyRotation, useLocalRotation);
		}
	}

	// Races
	TESRace* Forms::GetRace(const std::string_view& tag) {
		TESRace* data = nullptr;
		try {
			data = Forms::GetSingleton().races.at(std::string(tag)).data;
		}  catch (const std::out_of_range& oor) {
			data = nullptr;
			if (!Forms::Logged("impc", tag)) {
				log::warn("ImpactEffect: {} not found", tag);
			}
		}
		return data;
	}
	bool Forms::IsRace(Actor* actor, const std::string_view& tag) {
		auto data = GetRace(tag);
		if (data) {
			return actor->GetRace() == data;
		} else {
			return false;
		}
	}

	// Team Functions
	bool Forms::HasMagicEffectTeam(Actor* actor, const std::string_view& tag) {
		return Forms::HasMagicEffectTeamOr(actor, tag, false);
	}

	bool Forms::HasMagicEffectTeamOr(Actor* actor, const std::string_view& tag, const bool& default_value) {
		if (Forms::HasMagicEffectOr(actor, tag, default_value)) {
			return true;
		}
		if (Forms::InFaction(actor, "FollowerFaction") || actor->IsPlayerTeammate()) {
			auto player = PlayerCharacter::GetSingleton();
			return Forms::HasMagicEffectOr(player, tag, default_value);
		} else {
			return false;
		}
	}

	bool Forms::HasSpellTeam(Actor* actor, const std::string_view& tag) {
		return Forms::HasMagicEffectTeamOr(actor, tag, false);
	}

	bool Forms::HasSpellTeamOr(Actor* actor, const std::string_view& tag, const bool& default_value) {
		if (Forms::HasSpellTeam(actor, tag)) {
			return true;
		}
		if (Forms::InFaction(actor, "FollowerFaction") || actor->IsPlayerTeammate()) {
			auto player = PlayerCharacter::GetSingleton();
			return Forms::HasSpellTeamOr(player, tag, default_value);
		} else {
			return default_value;
		}
	}

	bool Forms::HasPerkTeam(Actor* actor, const std::string_view& tag) {
		return Forms::HasPerkTeamOr(actor, tag, false);
	}

	bool Forms::HasPerkTeamOr(Actor* actor, const std::string_view& tag, const bool& default_value) {
		if (Forms::HasPerk(actor, tag)) {
			return true;
		}
		if (Forms::InFaction(actor, "FollowerFaction") || actor->IsPlayerTeammate()) {
			auto player = PlayerCharacter::GetSingleton();
			return Forms::HasPerkOr(player, tag, default_value);
		} else {
			return default_value;
		}
	}

	bool Forms::Logged(const std::string_view& catagory, const std::string_view& key) {
		auto& m = Forms::GetSingleton().logged;
		std::string logKey = std::format("{}::{}", catagory, key);
		bool shouldLog = !(m.find(logKey) == m.end());
		m.emplace(logKey);
		return shouldLog;
	}

	void Forms::DataReady() {
		FormsConfig config;

		std::ifstream in(R"(Data\SKSE\Plugins\DevrForms.yaml)");
		yaml_source src(in);
		src >> config;

		for (auto &[key, value]: config.sounds) {
			auto form = find_form<BGSSoundDescriptorForm>(value);
			if (form) {
				this->sounds.try_emplace(key, form);
			} else if (!Forms::Logged("sond", key)) {
				log::warn("SoundDescriptorform not found for {}", key);
			}
		}

		for (auto &[key, value]: config.spellEffects) {
			auto form = find_form<EffectSetting>(value);
			if (form) {
				this->spellEffects.try_emplace(key, form);
			} else if (!Forms::Logged("mgef", key)) {
				log::warn("EffectSetting form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.spells) {
			auto form = find_form<SpellItem>(value);
			if (form) {
				this->spells.try_emplace(key, form);
			} else if (!Forms::Logged("spel", key)) {
				log::warn("SpellItem form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.perks) {
			auto form = find_form<BGSPerk>(value);
			if (form) {
				this->perks.try_emplace(key, form);
			} else if (!Forms::Logged("perk", key)) {
				log::warn("Perk form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.explosions) {
			auto form = find_form<BGSExplosion>(value);
			if (form) {
				this->explosions.try_emplace(key, form);
			} else if (!Forms::Logged("expl", key)) {
				log::warn("Explosion form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.globals) {
			auto form = find_form<TESGlobal>(value);
			if (form) {
				this->globals.try_emplace(key, form);
			} else if (!Forms::Logged("glob", key)) {
				log::warn("Global form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.quests) {
			auto form = find_form<TESQuest>(value);
			if (form) {
				this->quests.try_emplace(key, form);
			} else if (!Forms::Logged("qust", key)) {
				log::warn("Quest form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.factions) {
			auto form = find_form<TESFaction>(value);
			if (form) {
				this->factions.try_emplace(key, form);
			} else if (!Forms::Logged("facn", key)) {
				log::warn("FactionData form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.impacts) {
			auto form = find_form<BGSImpactDataSet>(value);
			if (form) {
				this->impacts.try_emplace(key, form);
			} else if (!Forms::Logged("impc", key)) {
				log::warn("ImpactData form not found for {}", key);
			}
		}

		for (auto &[key, value]: config.races) {
			auto form = find_form<TESRace>(value);
			if (form) {
				this->races.try_emplace(key, form);
			} else if (!Forms::Logged("race", key)) {
				log::warn("RaceData form not found for {}", key);
			}
		}

	}
}
