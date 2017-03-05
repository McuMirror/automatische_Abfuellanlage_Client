/* ####################################################################################
 *																					  #
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage								  #
 * Host:		Android System														  #
 * Filename:	micture.cpp															  #
 *																					  #
 * Entwickler:	Wögerbauer Stefan													  #
 * E-Mail:		woegste@hotmail.com													  #
 *																					  #
 * ####################################################################################
 */

//********************** INCLUDES *****************************************************
#include "mixture.h"
#include <QString>
#include <QDebug>

//*************************************************************************************

mixture::mixture(QObject *parent) : QObject(parent)
{

}

QString mixture::getName(void)
{
	//return the name of the mixture
	return name;
}

void mixture::setName(QString name)
{
	//set the name of the mixture
	this->name = name;
}

void mixture::getCommandName(void)
{
	//emit the signal for sending the name
	emit sendName(name);
}
