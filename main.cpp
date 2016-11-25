/* ###########################################################################################################
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage
 * Host:		Android System
 * Filename:	main.cpp
 *
 * Entwickler:	Wögerbauer Stefan
 * E-Mail:		woegste@hotmail.com
 *
 * ##########################################################################################################
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
