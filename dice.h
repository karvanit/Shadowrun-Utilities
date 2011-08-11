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
#ifndef SHADOWRUN_H_
#define SHADOWRUN_H_
#include <limits>

namespace Shadowrun {

/** If something annoying or bad should happen.
 *  Shadowrun: To have a glitch at least half the dice should come up aces.
 */
enum Glitch {
	NO_GLITCH,      //!< Not enough aces, ordinary results.
	GLITCH,         //!< At least half dice are aces.
	CRITICAL_GLITCH //!< A glitch with no hits at all.
};

/** Results of a simple test in Shadowrun.
 *  \internal No constructor, so we are considered a POD struct for unions.
 */
struct SimpleTestResult {
	/** Number of net hits rolled.
	 * If -1, then the roll was a failure (not enough hits).
	 */
	int hits;
	/** If a glitch happened. */
	enum Glitch glitch;

	/** Create a test result.
	 *  \internal We don't define a normal constructor to make this a POD type.
	 *  \param hits The number of net hits scored. Default is -1.
	 *  \param glitch The kind of glitch that happened. Default is no glitch.
	 *  \return The created SimpleTestResult.
	 */
inline static SimpleTestResult create(int hits = -1, enum Glitch glitch = NO_GLITCH) { SimpleTestResult res = { hits, glitch}; return res; }
};

/** Results of an extended test in Shadowrun.
 *  \internal No constructor, so we are considered a POD struct for unions.
 */
struct ExtendedTestResult {
	/** Number of intervals until success or failure.
	 * If <0, this is the number of intervals until the test failed. Otherwise it's the number of intervals until the test succeeded.
	 */
	int intervals;
	/** If a glitch happened.
	 *  Shadowrun: Only critical glitches are reported (and the fail the test).
	 *  Normal glitches just remove 1 - 6 hits from the accumulated amount.
	 */
	enum Glitch glitch;

	/** Create an extended test result.
	 *  \internal We don't define a normal constructor to make this a POD type.
	 *  \param intervals The number of net hits scored. Default is 1.
	 *  \param glitch The kind of glitch that happened. Default is no glitch.
	 *  \return The created ExtendedTestResult.
	 */
inline static ExtendedTestResult create(int intervals = 1, enum Glitch glitch = NO_GLITCH) { ExtendedTestResult res = { intervals, glitch}; return res; }
};

/** Results of an opposed test in Shadowrun.
 *  \internal No constructor, so we are considered a POD struct for unions.
 */
struct OpposedTestResult {
	/** Number of net hits scored.
	 * If <0, this is the number of net hits scored by the defender. Otherwise the number of net hits scored by the acting entity.
	 */
	int hits;
	/** If a glitch happened to the attacker.
	 */
	enum Glitch attacker_glitch;
	/** If a glitch happened to the defender.
	 */
	enum Glitch defender_glitch;

	/** Create an opposed test result.
	 *  \internal We don't define a normal constructor to make this a POD type.
	 *  \param hits The number of net hits scored. Default is 0.
	 *  \param att The kind of glitch that happened to the attacker. Default is no glitch.
	 *  \param def The kind of glitch that happened to the defender. Default is no glitch.
	 *  \return The created OpposedTestResult.
	 */
inline static OpposedTestResult create(int hits = 0, enum Glitch att = NO_GLITCH, enum Glitch def = NO_GLITCH) { OpposedTestResult res = { hits, att, def }; return res;
}
};

/** A pool of dice.
 *  A dice pool in Shadowrun has a number of dice and a possible limit on the number of hits allowed per roll.
 *  We also need the die type, to understand the rolls.
 */
class DicePool {
public:
	enum DieType { NORMAL, EDGE, RUSHED, RUSHED_EDGE };
	/** Create a pool of dice.
	 *  \param count The number of dice.
	 *  \param type The type of dice.
	 *  \param limit The maximum number of hits accepted.
	 */
	explicit DicePool(int count, enum DieType type, int limit = std::numeric_limits<int>::max()) : count(count), type(type), limit(limit) {}

	/** Create a pool of dice.
	 *  \param count The number of dice.
	 *  \param limit The maximum number of hits accepted.
	 *  \param type The type of dice.
	 */
	explicit DicePool(int count, int limit = std::numeric_limits<int>::max(), enum DieType type = NORMAL) : count(count), type(type), limit(limit) {}

	/** Make a simple test and return the result.
	 *  \param threshold To be a success, we have to get at least this many hits.
	 *  @see SimpleTestResult
	 */
	SimpleTestResult roll(int threshold = 1) const;

	/** Make an extended test and return the result.
	 *  \param threshold To be a success, we have to get at least this many hits.
	 *  \param limited If set, we deduct one dice per reroll, until we stop at 0 dice.
	 *  @see ExtendedTestResult
	 */
	ExtendedTestResult extendedRoll(int threshold = 2, bool limited = false) const;

	/** Return the result of rolling opposed by the \a other dice pool.
	 *  \param other The dice pool to used as a defender.
	 *  @see OpposedTestResult
	 */
	OpposedTestResult opposedRoll(const DicePool &other) const;
private:
	int count;	//<! Number of dice.
	DieType type;	//<! The type of dice in the pool.
	int limit;	//<! The number of hits allowed by this pool.
};

}	//Shadowrun namespace

#endif /* SHADOWRUN_H_ */
