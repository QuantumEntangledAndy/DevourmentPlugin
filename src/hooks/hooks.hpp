#pragma once
// Hooks into skyrim engine
//
using namespace RE;
using namespace SKSE;

#define RELOCATION_OFFSET(SE, AE) REL::VariantOffset(SE, AE, 0).offset()

namespace Hooks
{
	void Install();
}
