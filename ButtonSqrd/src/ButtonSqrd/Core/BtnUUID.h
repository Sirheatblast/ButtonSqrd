#pragma once
#include<xhash>
namespace BtnSqd {
	class BtnUUID {
	public:
		BtnUUID();
		BtnUUID(uint64_t id);
		operator uint64_t()const{
			return uuid;
		}

	private:
		uint64_t uuid;
	};
}

namespace std {
	template<>
	struct hash<BtnSqd::BtnUUID> {
		std::size_t operator()(const BtnSqd::BtnUUID& uuid) const {
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}