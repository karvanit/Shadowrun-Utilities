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
  : QWidget(parent), history(), history_position(-1), results()
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
 *  FIXME: This is a mess, it should be possible to make the flow cleaner.
 *  XXX: When the new text is the same as the previous entry, we should not add it to the history again, and in fact not change the history at all.
 */
void
RollBar::on_roller_editingFinished()
{
	QString val = ui.roller->text();
	if (val.isEmpty()) {
		history_position = -1;
		return;
	}
	RollInfo result;
	if (!parseAndRoll(val, result)) {
		//Make sure the bad line is not part of our history, but that it remains in the editline.
		//Also, change the background of the display area, to signal an error.
		//XXX: Are we actually doing what we promised to the history?
		if (history_position != -1) {
			if (history.at(history.size() - 2) != val) {
				history.removeLast();
			}
		}
		ui.roller->setPalette(*error_colours);
	} else {
		//Make sure the line becomes part of our history and clear the editline.
		//And place the result message in the message log.
		//XXX: Are we actually doing what we promised to the history?
		if (history_position != -1) {
			if (history.at(history.size() - 2) != val) {
				history[history.size() - 1] = val;
			} else {
				history.removeLast();
			}
		} else {
			history.append(val);
		}
		if (history.size() > 100)
			history.removeFirst();
		if (results.size() > 9)
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
			//XXX: Write this
			break;
		case RollInfo::SR_OPPOSED:
			//XXX: Write this
			break;
		}
		results.prepend(res);
		ui.results->setPlainText(results.join("\n"));
		ui.roller->setText("");
		ui.roller->setPalette(QApplication::palette());
	}
	history_position = -1;	//Reset history walk
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
 *  The first time this is called for each entry it will also store the partial
 *  entry at the end of the history.
 */
void
RollBar::historyPrev()
{
	int prev_pos = history_position;
	if (!history.isEmpty() && history_position == -1) {
		if (ui.roller->text() != history.last()) {
			history.append(ui.roller->text());
			history_position = history.size() - 2;
		} else {
			history_position = history.size() - 1;
		}
	} else if (history_position > 0)
		--history_position;
	if (history_position != prev_pos)
		ui.roller->setText(history[history_position]);
}

/** Show the next history entry.
 *  The line been edited when history view started is always the last entry in history.
 */
void
RollBar::historyNext()
{
	int prev_pos = history_position;
	if (history_position < history.size() -1)
		++history_position;
	if (history_position != prev_pos)
		ui.roller->setText(history[history_position]);
}
