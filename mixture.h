/* ############################################################################
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage
 * Host:		Android System
 * Filename:	mixture.h
 *
 * Entwickler:	Wögerbauer Stefan
 * E-Mail:		woegste@hotmail.com
 *
 * ############################################################################
 */

#ifndef MIXTURE_H
#define MIXTURE_H

#include <QObject>
#include <QString>

class mixture : public QObject
{
	Q_OBJECT
public:
	explicit mixture(QObject *parent = 0);

signals:
		void sendName(QString name);

public slots:
		void setName(QString name);
		QString getName(void);
		void getCommandName(void);

private:
		QString name;
};

#endif // MIXTURE_H
