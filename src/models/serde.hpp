#pragma once
// Serde generalisation
//

using namespace SKSE;

namespace Devr {
	class Serde {
		// Name used for debug purposes
		virtual std::string SerdeName();

		// Serialisation (save) method
		virtual void Ser(SerializationInterface* serde, std::uint32_t version);

		// Desearilation (load) method
		virtual void De(SerializationInterface* serde, std::uint32_t version);

		// Serialisation version for new entries
		virtual std::uint32_t SerVersion();
	};
}
