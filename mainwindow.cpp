/* ####################################################################################
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage
 * Host:		Android System
 * Filename:	mainwindow.cpp
 *
 * Entwickler:	Wögerbauer Stefan
 * E-Mail:		woegste@hotmail.com
 *
 * ####################################################################################
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "remoteselector.h"
#include "bluetoothtransmissionclient.h"
#include "mixture.h"

#include <QDebug>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QScreen>
#include <QString>
#include <QSize>

//Bluetooth
#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//loadMixesFromServerAfterTime= new QTimer(this);
	//loadMixesFromServerAfterTime->setSingleShot(true);
	//connect(loadMixesFromServerAfterTime, SIGNAL(timeout()), this,
	//		SLOT(loadMixesFromServer()));

	connect(ui->pushButton_quit, SIGNAL(clicked(bool)), this, SLOT(close()));

	localBluetoothAdapters = QBluetoothLocalDevice::allDevices();

	//Only one adapter allowed
	QBluetoothLocalDevice adapter(localBluetoothAdapters.at(0).address());
	adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

	QScreen *screen = QApplication::screens().at(0);
	screenWidth  = screen->availableSize().width();
	screenHeight = screen->availableSize().height();

	localName = QBluetoothLocalDevice().name();
}

MainWindow::~MainWindow()
{
	for(int i =0; i < clients.size(); i++)
	{
		clients.at(i)->stopClient();
	}

	delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
	ui->pushButton_connect->setEnabled(false);

	// scan for services
	const QBluetoothAddress adapter = localBluetoothAdapters.isEmpty() ?
				QBluetoothAddress() :
				localBluetoothAdapters.at(0).address();

	RemoteSelector remoteSelector(adapter);
	remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));
	remoteSelector.setGeometry(0, 0, screenWidth, screenHeight/3);

	if (remoteSelector.exec() == QDialog::Accepted)
	{
		QBluetoothServiceInfo service = remoteSelector.service();

		qDebug() << "Connecting to service 2" << service.serviceName() <<
					"on" << service.device().name();

		// Create client
		qDebug() << "Going to create client";
		BluetoothTransmissionClient *client = new BluetoothTransmissionClient(this);
		qDebug() << "Connecting...";

		connect(client, SIGNAL(messageReceived(QString,QString)),   this,
				SLOT(commandReceived(QString,QString)));
		//connect(client, SIGNAL(disconnected()),                     this,
		//		SLOT(clientDisconnected()));
		connect(this,   SIGNAL(sendCommand(QString)),               client,
				SLOT(sendMessage(QString)));
		//connect(client, SIGNAL(connected(QString)),					this,
				//SLOT(connected(QString)));
		connect(client, SIGNAL(connected(QString)),					this,
				SLOT(loadMixesFromServer()));
		qDebug() << "Start client";
		client->startClient(service);

		clients.append(client);
	}
	//loadMixesFromServerAfterTime->start(5000);
	ui->pushButton_connect->setEnabled(true);
}

void MainWindow::commandReceived(QString server, QString command)
{
	qDebug() << "command:" << command;
	qDebug() << "server:" << server;

	if(command.trimmed().at(0) == '_')
	{
		QString commandForSend;
		for(int i=1; i < command.length(); i++)
		{
			commandForSend.append(command.at(i));
		}

		QMessageBox::critical(this, "Automatische Abfüllanlage",
							  commandForSend.trimmed());
	}
	else if(command.trimmed() != "ready")
	{
		mixtures.append(new mixture);
		mixtures.last()->setName(command.trimmed());
	}
	else
	{

	}

	if(command.trimmed() == "ready")
	{
		writeListWidget();
	}
}

void MainWindow::loadMixesFromServer(void)
{
	sendBluetoothCommand("_sendBackMixes");
}

void MainWindow::sendBluetoothCommand(QString command)
{
	emit sendCommand(command);
}

void MainWindow::writeListWidget(void)
{
	for(int i = 0; i < mixtures.size(); i++)
	{
		QPushButton *button = new QPushButton;
		button->setText(mixtures.at(i)->getName());

		QListWidgetItem *item = new QListWidgetItem;
		QSize size;
		size.setHeight(screenHeight/15);
		item->setSizeHint(size);
		ui->listWidget_Mixes->addItem(item);
		ui->listWidget_Mixes->setItemWidget(item, button);
		connect(button,			SIGNAL(clicked(bool)),		mixtures.at(i),
				SLOT(getCommandName()));
		connect(mixtures.at(i), SIGNAL(sendName(QString)),	this,
				SLOT(ButtonSlot(QString)));
	}
}

void MainWindow::ButtonSlot(QString name)
{
	sendBluetoothCommand("/" + name);
}

void MainWindow::connected(QString peerName)
{
	qDebug() << peerName;
}
