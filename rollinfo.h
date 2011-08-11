/*
 * Copyright © 2011 Konstantinos Arvanitis <konstantinos.arvanitis@gmail.com>
 *
 * This file is part of Shadowrun Utilities.
 *
 * Shadowrun Utilities program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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


#ifndef ROLLINFO_H_
#define ROLLINFO_H_
#include "dice.h"
class QString;

namespace Roller {
/** The results of any type of Shadowrun roll, along with the actual roll type.
 */
struct RollInfo {
	enum Roll {
		SR_SIMPLE,
		SR_EXTENDED,
		SR_OPPOSED
	} roll;
	union {
		Shadowrun::SimpleTestResult simple;
		Shadowrun::ExtendedTestResult extended;
		Shadowrun::OpposedTestResult opposed;
	};
};

/** Make the roll and fill the result information.
    @param cmd The string to parse.
    @param result The place to return the result.
    @return Notify if there was a parse error.
 */
bool parseAndRoll(const QString &cmd, RollInfo &result);
}	//end of Roller namespace
#endif /* ROLLINFO_H_ */
