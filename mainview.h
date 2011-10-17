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
#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QtGui/QWidget>
#include "ui_mainview.h"

class MainView : public QWidget {
	Q_OBJECT

public:
	MainView(QWidget *parent = 0);
	~MainView();

	void addActivity(QWidget *activity, const QString &actName);

private slots:
	void on_activity_tabCloseRequested(int);

private:
	Ui::MainViewClass ui;
};

#endif // MAINVIEW_H
