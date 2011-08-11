/*
 * Copyright © 2011 Konstantinos Arvanitis <konstantinos.arvanitis@gmail.com>
 *
 * This file is part of Shadowrun Utilities.
 *
 * Shadowrun Utilities program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMBATACTOR_H_
#define COMBATACTOR_H_
#include <QString>

namespace Shadowrun {

/** An entity that can take part in combat.
 */
struct CombatActor {
	//Base values
	QString name;
	int edge;
	//These can change while in combat
	int initiative;
	int passes;
	int wound_mod;
	//Combat values (these only have meaning during combat)
	int init_score;
	int lost_passes;
	CombatActor(): name("Mr. Johnson"), edge(1), initiative(2), passes(1), wound_mod(0), init_score(0), lost_passes(0) {}

	/** Calculate new initiative value. */
	void calculateInitiativeScore(bool use_edge = false);
	/** Compare based on initiative. */
	bool operator<(const CombatActor &other) const;
};

}	//Shadowrun namespace

#endif /* COMBATACTOR_H_ */
