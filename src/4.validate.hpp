#pragma once
#include "wizzardrt.hpp"

// === Validate code structure & types ===
struct Validate {
	// validate everything
	void all() {
		for (const auto& cl : wizclass)
			vclass(cl);
		printf("Validate OK!\n");
	}

	void vclass(const WizClass& cl) {
		// duplicate name
		// duplicate members
		// duplicate functions
	}
};
