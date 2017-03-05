/* ####################################################################################
 *																					  #
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage								  #
 * Host:		Android System														  #
 * Filename:	main.cpp															  #
 *																					  #
 * Entwickler:	Wögerbauer Stefan													  #
 * E-Mail:		woegste@hotmail.com													  #
 *																					  #
 * ####################################################################################
 */

//********************** INCLUDES *****************************************************
#include "mainwindow.h"
#include <QApplication>

//*************************************************************************************

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();						//show the Window

	return a.exec();
}
