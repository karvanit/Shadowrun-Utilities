/*
 * Copyright © 2011 Konstantinos Arvanitis <konstantinos.arvanitis@gmail.com>
 *
 * This file is part of Shadowrun Utilities.
 *
 * Shadowrun Utilities program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dice.h"
#include "combatactor.h"
#include <cstdlib>
#include <algorithm>
#include <assert.h>

using namespace Shadowrun;
namespace {
/** The functional values of Shadowrun dice sides.
 *  Only the odds of each value change, the types are fixed.
 */
enum DieFace {
	BAD, //!< This counts towards glitches.
	NOTHING, //!< Nothing special.
	HIT, 	//!< This counts towards hits.
	REROLL 	//!< Count this as a hit, and reroll.
};

/** Normal dice use this side map.
 *  We have 1 as bad, 5 and 6 as hits.
 */
DieFace normal_sides[6] = { BAD, NOTHING, NOTHING, NOTHING, HIT, HIT };

/** Edge dice use this side map.
 *  We have 1 as bad, 5 as hit and 6 as hit and reroll.
 */
DieFace edge_sides[6] = { BAD, NOTHING, NOTHING, NOTHING, HIT, REROLL };

/** Normal dice in rushed extended tests use this side map.
 *  We have 1 and 2 as bad, 5 and 6 as hits.
 */
DieFace rushed_sides[6] = { BAD, BAD, NOTHING, NOTHING, HIT, HIT };

/** Edge dice in rushed extended tests use this side map.
 *  We have 1 and 2 as bad, 5 as hit and 6 as hit and reroll.
 */
DieFace rushed_edge_sides[6] = { BAD, BAD, NOTHING, NOTHING, HIT, REROLL };

/** Roll a single die.
 *  The return is one of the face values, NOT a [1,6] number.
 *  \param type The type of die to roll. This controls the value distribution.
 */
DieFace roll(DicePool::DieType type = DicePool::NORMAL)
{
	DieFace *sides;
	switch (type) {
	case DicePool::NORMAL:
		sides = normal_sides;
		break;
	case DicePool::EDGE:
		sides = edge_sides;
		break;
	case DicePool::RUSHED:
		sides = rushed_sides;
		break;
	case DicePool::RUSHED_EDGE:
		sides = rushed_edge_sides;
		break;
	default:
		assert(0 && "Unreachable code reached!");
	}
	return sides[static_cast<int>(random() / (RAND_MAX + 1.0) * 6)];
}
}	//Local namespace

/** This namespace contains classes that represent game concepts in Shadowrun.
 */
namespace Shadowrun {
SimpleTestResult DicePool::roll(int threshold) const
{
	if (count <= 0)
		return SimpleTestResult::create(-1, NO_GLITCH);
	threshold = std::max(0, threshold);
	int dice = count;
	int hits = 0;
	int bads = 0;
	while (dice) {
		DieFace res = ::roll(type);
		switch (res) {
		case NOTHING: /* Do nothing! */
			break;
		case BAD:
			++bads;
			break;
		case REROLL:
			++dice;
			/* And count as a hit too! */
		case HIT:
			++hits;
			break;
		}
		--dice;
	}
	int net_hits = std::max(-1, std::min(limit, hits) - threshold);
	return SimpleTestResult::create(net_hits, (count <= 2 * bads) ? (hits ? GLITCH : CRITICAL_GLITCH) : NO_GLITCH);
}

ExtendedTestResult DicePool::extendedRoll(int threshold, bool limited) const
{
	if (count <= 0)
		return ExtendedTestResult::create(0, NO_GLITCH);
	int hits = 0;
	int tries = 0;
	threshold = std::max(2, threshold);
	if (!limited) {
		do {
			++tries;
			SimpleTestResult res = roll(0);
			switch (res.glitch) {
			case GLITCH:
				hits -= static_cast<int>(random() / (RAND_MAX + 1.0) * 6 + 1);
				/* Now count any hits */
			case NO_GLITCH:
				hits += res.hits;
				break;
			case CRITICAL_GLITCH:
				return ExtendedTestResult::create(-tries, CRITICAL_GLITCH);
			}
		} while (hits < threshold);
		return ExtendedTestResult::create(tries, NO_GLITCH);
	} else {
		DicePool diminishing(*this);
		do {
			++tries;
			SimpleTestResult res = diminishing.roll(0);
			switch (res.glitch) {
			case GLITCH:
				hits -= static_cast<int>(random() / (RAND_MAX + 1.0) * 6 + 1);
				/* Now count any hits */
			case NO_GLITCH:
				hits += res.hits;
				break;
			case CRITICAL_GLITCH:
				return ExtendedTestResult::create(-tries, CRITICAL_GLITCH);
			}
			--diminishing.count;
		} while (hits < threshold && diminishing.count > 0);
		return ExtendedTestResult::create((diminishing.count != 0) ? tries : -tries, NO_GLITCH);
	}
}

OpposedTestResult DicePool::opposedRoll(const DicePool & other) const
{
	SimpleTestResult att = roll(0);
	SimpleTestResult def = other.roll(0);
	return OpposedTestResult::create(att.hits - def.hits, att.glitch, def.glitch);
}

/** Compare based on initiative.
 *  Higher initiative goes first!
 */
bool
CombatActor::operator <(CombatActor & other)
{
	int ret = other.initiativeScore() - initiativeScore();
	if (!ret) {
		if (other.glitched && !glitched)
			return true;
		if (!other.glitched && glitched)
			return false;
		ret = other.edge - edge;
	}
	if (!ret) {
		ret = other.initiative - initiative;
	}
	// ShadowrunRule: This is not strictly correct by the rules, we should finally compare Reaction attributes
	return ret < 0;
}

/** Calculate the initiative score of an entity.
 * ShadowrunRule: A character with no initiative pool (due to wounds probably) gets no actions.
 */
void
CombatActor::calculateInitiativeScore(bool use_edge)
{
	if (initiative + wound_mod + (use_edge ? edge : 0) <= 0) {
		//TODO: Implement ShadowrunRule: This actor should lose all actions for this turn.
		init_hits = 0;
	} else {
		int init_pool = initiative + wound_mod + use_edge ? edge : 0;
		DicePool dp = DicePool(init_pool, use_edge ? DicePool::EDGE : DicePool::NORMAL);
		SimpleTestResult res = dp.roll(0);
		switch (res.glitch) {
		case CRITICAL_GLITCH:
			//TODO: ShadowrunRule: This actor should lose an action, if it has more than one pass.
			// Plus whatever the regular glitch does.
		case GLITCH:
			glitched = true;
			init_hits = res.hits;
			break;
		case NO_GLITCH:
			glitched = false;
			init_hits = res.hits;
			break;
		}
	}
}

int
CombatActor::initiativeScore()
{
	if (init_hits == -1) {
		calculateInitiativeScore();
	}
	return initiative + wound_mod + init_hits;
}
//TODO Implement ShadowrunRule: A character can spend Edge to gain an extra pass.
}	//Shadowrun namespace
