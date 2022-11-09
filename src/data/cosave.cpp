#include "data/cosave.hpp"

using namespace SKSE;
using namespace RE;

namespace Devr {
	CoSave& CoSave::GetSingleton() noexcept {
		static CoSave instance;
		return instance;
	}

	void Persistent::OnRevert(SerializationInterface*) {
	}

	static RegisterForSerde(std::uint32_t tag, Serde* callback) {
		this->serde_registry.try_emplace(tag, callback);
	}

	void Persistent::OnGameLoaded(SerializationInterface* serde) {
		std::unique_lock lock(GetSingleton()._lock);

		std::uint32_t type;
		std::uint32_t size;
		std::uint32_t version;

		SizeManager::GetSingleton().Reset();

		while (serde->GetNextRecordInfo(type, version, size)) {
			try {
				auto& serde = this->serde_registry.at(type);
				serde.De(serde, version)
			} catch (const std::out_of_range& oor) {
				log::warn("Unknown record type in cosave ({}).", type);
			}
		}
	}

	void Persistent::OnGameSaved(SerializationInterface* serde) {
		std::unique_lock lock(GetSingleton()._lock);

		for (auto const& [key, record]: this->serde_registry) {
			auto version = record->SerVersion();
			if (!serde->OpenRecord(key, version)) {
				log::error("Unable to open {} record to write cosave data.", record->SerdeName());
				continue;
			} else {
				record.Ser(serde, version);
			}
		}
	}
}
