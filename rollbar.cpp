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
#include "rollbar.h"
#include "rollinfo.h"
#include <QKeyEvent>
using namespace Roller;
using namespace Shadowrun;

QPalette *RollBar::error_colours(0);

RollBar::RollBar(QWidget *parent)
  : QWidget(parent), history(), history_position(-1), results(), stored_current()
{
	if (!error_colours) {
		error_colours = new QPalette(QApplication::palette());
		error_colours->setBrush(QPalette::Base, error_colours->alternateBase());
	}
	ui.setupUi(this);
	ui.results->setMaximumHeight(ui.results->fontMetrics().height() * 6);
}

RollBar::~RollBar()
{

}

/** Handle the line input.
 *  This will calculate the result and prepend it in the results label.
 *  It will also add the line in the history, if it's different from the previous one.
 */
void
RollBar::on_roller_editingFinished()
{
	QString val = ui.roller->text();
	if (val.isEmpty()) {
		restoreHistory();
		return;
	}
	RollInfo result;
	if (!parseAndRoll(val, result)) {
		restoreHistory();
		ui.roller->setPalette(*error_colours);
	} else {
		addHistory(val);
		if (results.size() == MAX_RESULT_ENTRIES)
			results.removeLast();
		/* Now decode the result from the roll. */
		QString res("Not implemented yet");
		switch (result.roll) {
		case RollInfo::SR_SIMPLE:
			if (result.simple.glitch == CRITICAL_GLITCH) {
				res = tr("Critical glitch!");
			} else if (result.simple.hits == -1) {
				res = result.simple.glitch == GLITCH ? tr("Failed with a glitch!") : tr("Failed");
			} else if (result.simple.hits == 0) {
				res = result.simple.glitch == GLITCH ? tr("Success with a glitch!") : tr("Bare Success");
			} else if (result.simple.glitch == GLITCH) {
				res = tr("%1 net hits and a glitch!").arg(result.simple.hits);
			} else {
				res = tr("%1 net hits").arg(result.simple.hits);
			}
			break;
		case RollInfo::SR_EXTENDED:
			if (result.extended.intervals < 0) {
				//We never fail with simple glitch.
				if (result.extended.glitch == NO_GLITCH) {
					res = tr("Failed after %1 intervals").arg(-result.extended.intervals);
				} else {
					res = tr("Failed after %1 intervals, with critical glitch!").arg(-result.extended.intervals);
				}
			} else if (result.extended.intervals > 0) {
				//We never have glitch if we succeed.
				res = tr("Success after %1 intervals").arg(result.extended.intervals);
			} else /* No intervals == not enough dice to try */ {
				res = tr("Not even started!");
			}
			break;
		case RollInfo::SR_OPPOSED:
			if (result.opposed.hits < 0) {
				res = tr("%1 net hits for the defender").arg(-result.opposed.hits);
			} else if (result.opposed.hits > 0) {
				res = tr("%1 net hits for the attacker").arg(result.opposed.hits);
			} else {
				res = tr("Tie");
			}
			switch (result.opposed.attacker_glitch) {
			case NO_GLITCH:
				// Nothing more to add.
				break;
			case GLITCH:
				res += tr(", Attacker glitch");
				break;
			case CRITICAL_GLITCH:
				res += tr(", Critical attacker glitch");
				break;
			}
			switch (result.opposed.defender_glitch) {
			case NO_GLITCH:
				// Nothing more to add.
				break;
			case GLITCH:
				res += tr(", Defender glitch");
				break;
			case CRITICAL_GLITCH:
				res += tr(", Critical defender glitch");
				break;
			}
			break;
		}
		results.prepend(res);
		ui.results->setPlainText(results.join("\n"));
		ui.roller->setText("");
		ui.roller->setPalette(QApplication::palette());
	}
}

void
RollBar::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		historyPrev();
		break;
	case Qt::Key_Down:
		historyNext();
		break;
	case Qt::Key_Escape:
		ui.roller->setText("");
		break;
	default:
		QWidget::keyPressEvent(event);
	}
}

/** Show the previous history entry.
 *  The first time this is called we also store the current text.
 */
void
RollBar::historyPrev()
{
	int prev_pos = history_position;
	if (history_position == -1) {
		QString cur = ui.roller->text();
		if (history.isEmpty() || cur != history.last()) {
			history.append(cur);
			stored_current = true;
		} else {	//Skip if same as last history entry
			stored_current = false;
		}
		history_position = history.size() - 2;
	} else if (history_position > 0)
		--history_position;
	if (history_position != prev_pos)
		ui.roller->setText(history[history_position]);
}

/** Show the next history entry.
 * Once we have no more history, display the current text, if we have any.
 */
void
RollBar::historyNext()
{
	if (history_position < history.size() -1) {
		++history_position;
		ui.roller->setText(history[history_position]);
	}
}

/** Restore the history to normal values.
 *  Remove any extra value added temporary for the history walk.
 */
void
RollBar::restoreHistory()
{
	if (stored_current) {
		history.removeLast();
		stored_current = false;
	}
	history_position = -1;
}

/** Append a history entry.
 *  We only add the value if it's not already the last entry in the history list.
 *  \param entry The entry to append to our history list.
 */
void
RollBar::addHistory(const QString &entry)
{
	if (stored_current) {
		if (entry != history.at(history.size() - 2)) {
			history[history.size() - 1] = entry;
		} else {
			history.removeLast();
		}
		stored_current = false;
	} else if (history.isEmpty() || entry != history.last()) {
		history.append(entry);
	}
	history_position = -1;
}
