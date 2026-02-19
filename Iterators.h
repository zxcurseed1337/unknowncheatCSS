#pragma once
namespace Utility::Iterators {
 
	struct EnemyTeam_ {
		C_BasePlayer* begin();
		C_BasePlayer* end();
		int idx = 1;
	};

	struct AllPlayers {
		C_BasePlayer* begin();
		C_BasePlayer* end();
	};
}


