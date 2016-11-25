/* ###########################################################################################################
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage
 * Host:		Android System
 * Filename:	micture.cpp
 *
 * Entwickler:	Wögerbauer Stefan
 * E-Mail:		woegste@hotmail.com
 *
 * ##########################################################################################################
 */

#include "mixture.h"

#include <QString>
#include <QDebug>

mixture::mixture(QObject *parent) : QObject(parent)
{

}

QString mixture::getName(void)
{
	return name;
}

void mixture::setName(QString name)
{
	this->name = name;
}

void mixture::getCommandName(void)
{
	emit sendName(name);
}
