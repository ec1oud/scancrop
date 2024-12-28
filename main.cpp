// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QStringList args;
	int i = 1;
	while (i < argc)
		args.append(argv[i++]);
	MainWindow w(args);
//	if (argc > 1)
//		w.open(QString(argv[1]));
//	if (argc > 2)
//		w.openTemplate(QString(argv[2]));
	w.show();
	return a.exec();
}
