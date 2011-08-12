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
%{
#include <cstdlib>
#include "roller.tab.hpp"
#define yyterminate() return token::END
#define YY_DECL Roller::Parser::token_type rollerlex(Roller::Parser::semantic_type *yylval)
%}
%option noyywrap nounput batch
NUM	[1-9][0-9]*
WS	[ \t\n]+
%%
%{
	typedef Roller::Parser::token token;
%}
{WS}	/* Skip blanks and tabs */
{NUM}|0	{
		yylval->num = atoi(yytext);
		return token::NUMBER;
	}
\+	return token::PLUS;
\-	return token::MINUS;
"/"	return token::LIMIT;
rushed|rush|r	return token::RUSHED;
limited|lim|l	return token::LIMITED;
e|E	return token::EDGE;
vs|opposed|opp	return token::OPPOSED;
"("|"["	return token::OPEN;
")"|"]"	return token::CLOSE;
","	return token::INTERVAL;
<<EOF>>	return token::END;
.	return token::INVALID;
%%

#include "rollbar.h"
#include "rollinfo.h"

/** Make the roll and fill the result information.
    @param cmd The string to parse.
    @param result The place to return the result.
    @return Notify if there was a parse error.
 */
bool
Roller::parseAndRoll(const QString &cmd, Roller::RollInfo &result)
{
	Roller::Parser parser(result);
	QByteArray ba = cmd.toAscii();
	YY_BUFFER_STATE buffer = roller_scan_bytes(ba.constData(), ba.length());
	bool ok = !parser.parse();
	roller_delete_buffer(buffer);
	return ok;
}
