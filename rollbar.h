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
#ifndef ROLLBAR_H
#define ROLLBAR_H

#include <QtGui/QWidget>
#include "ui_rollbar.h"

class RollBar : public QWidget {
	Q_OBJECT

public:
	RollBar(QWidget *parent = 0);
	~RollBar();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void historyPrev();
	void historyNext();

private slots:
	void on_roller_editingFinished();

private:
	QStringList history;
	int history_position;
	QStringList results;
	Ui::RollBar ui;
static QPalette *error_colours;
};

#endif // ROLLBAR_H
