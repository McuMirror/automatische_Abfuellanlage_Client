/* ############################################################################
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage
 * Host:		Android System
 * Filename:	mainwindow.h
 *
 * Entwickler:	Wögerbauer Stefan
 * E-Mail:		woegste@hotmail.com
 *
 * ############################################################################
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "bluetoothtransmissionclient.h"
#include "mixture.h"


//Bluetooth
#include <QBluetoothHostInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_pushButton_connect_clicked();
	void commandReceived(QString server, QString command);
	void loadMixesFromServer(void);
	void sendBluetoothCommand(QString command);
	void writeListWidget(void);
	void ButtonSlot(QString name);
	void connected(QString peerName);

signals:
	void sendCommand(const QString &command);

private:
	Ui::MainWindow *ui;

	QList<BluetoothTransmissionClient *> clients;
	QList<QBluetoothHostInfo> localBluetoothAdapters;
	QList<mixture*> mixtures;

	int screenWidth;
	int screenHeight;

	QString localName;
};

#endif // MAINWINDOW_H
