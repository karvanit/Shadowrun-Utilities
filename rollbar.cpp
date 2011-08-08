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
#include <QKeyEvent>

RollBar::RollBar(QWidget *parent)
  : QWidget(parent), history(), history_position(-1), results()
{
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
	if (!val.isEmpty()) {
		if (history_position != -1) {	//XXX: This places an extra empty line if the line matches the previous line's text.
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
		history_position = -1;	//Reset history walk
		// TODO: Instead of echoing the line, calculate the results!
		if (results.size() > 9)
			results.removeLast();
		results.prepend(val);
		ui.results->setPlainText(results.join("\n"));
		ui.roller->setText("");
		return;
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
