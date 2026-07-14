#include "BtnUUID.h"
#include<random>

namespace BtnSqd {
	static std::random_device rDev;
	static std::mt19937_64 rEngine(rDev());
	static std::uniform_int_distribution uDistro;

	BtnUUID::BtnUUID():uuid(uDistro(rEngine)) {
	
	}
	BtnUUID::BtnUUID(uint64_t id):uuid(id) {
	
	}
}