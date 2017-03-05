/* ####################################################################################
 *																					  #
 * Projekt:		Diplomarbeit: Autmatische Abfüllanlage								  #
 * Host:		Android System														  #
 * Filename:	mainwindow.cpp														  #
 *																					  #
 * Entwickler:	Wögerbauer Stefan													  #
 * E-Mail:		woegste@hotmail.com													  #
 *																					  #
 * ####################################################################################
 */

//********************** INCLUDES *****************************************************
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

//************** Service UUID *********************************************************

/*
 * The client must have the same service UUID as the server otherwise the client doesn't
 * find the server
 */

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

//*************************************************************************************

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//close the application if you press the button "Beenden"
	connect(ui->pushButton_quit, SIGNAL(clicked(bool)), this, SLOT(close()));

	//local Bluetooth adapters
	localBluetoothAdapters = QBluetoothLocalDevice::allDevices();

	//Only one adapter allowed
	QBluetoothLocalDevice adapter(localBluetoothAdapters.at(0).address());
	//make the bluetooth discoverable
	adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

	//get the size of the Screen
	QScreen *screen = QApplication::screens().at(0);	//use screen number 0
	screenWidth		= screen->availableSize().width();	//screen width
	screenHeight	= screen->availableSize().height();	//screen height

	localName = QBluetoothLocalDevice().name();			//get the name of the local BT
}

MainWindow::~MainWindow()
{
	//stop all Bluetooth-clients
	for(int i =0; i < clients.size(); i++)
	{
		clients.at(i)->stopClient();	//stop the client
	}

	delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
	ui->pushButton_connect->setEnabled(false);	//disable the Connect Button

	// scan for services
	const QBluetoothAddress adapter = localBluetoothAdapters.isEmpty() ?
				QBluetoothAddress() :
				localBluetoothAdapters.at(0).address();

	//create a Remote selector with the local Bluetooth adapter
	RemoteSelector remoteSelector(adapter);
	//start discovering for the bluetooth server withe the same Service UUID
	remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));

	/*
	 * set the size and the position of the RemoteSelector Window
	 * x = 0 -> screenWidth
	 * y = 0 -> screenHeight/3
	 */

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


		/*
		 * connect the signal 'messageReceived' of the bluetooth client
		 * with the slot send 'commandRecieved' of the MainWindow
		 */

		connect(client, SIGNAL(messageReceived(QString,QString)), this,
						SLOT(commandReceived(QString,QString)));

		/*
		 * connect the signal 'sendCommand' of the MainWindow
		 * with the slot 'send message' of the bluetooth client
		 */

		connect(this,   SIGNAL(sendCommand(QString)), client,
						SLOT(sendMessage(QString)));

		/*
		 * load the availbale mixes from the server when the connectione is established
		 */

		connect(client, SIGNAL(connected(QString)),	this,
						SLOT(loadMixesFromServer()));

		qDebug() << "Start client";
		client->startClient(service);	//start the bluetooth client

		clients.append(client);			//append the bluetooth client to the client list
	}

	ui->pushButton_connect->setEnabled(true);	//enable Connect Button
}

void MainWindow::commandReceived(QString server, QString command)
{
	qDebug() << "command:" << command;
	qDebug() << "server:" << server;

	//commands begins with an '_'
	if(command.trimmed().at(0) == '_')
	{
		QString commandForSend;
		for(int i=1; i < command.length(); i++)
		{
			//edit the command, ignore the '_' at the beginning of the command
			commandForSend.append(command.at(i));
		}

		//open a Message box to warn the user
		QMessageBox::critical(this, "Automatische Abfüllanlage",
									commandForSend.trimmed());
	}
	//while the command isn't 'ready' mixtures will be sended
	else if(command.trimmed() != "ready")
	{
		//append a new mixture Object to the list
		mixtures.append(new mixture);
		//set the name of the mixture of the last object in the list
		mixtures.last()->setName(command.trimmed());
	}
	else
	{
		//nothing
	}

	if(command.trimmed() == "ready")		//all mixtures were sendet
	{
		writeListWidget();					//write the list Widget
	}
}

void MainWindow::loadMixesFromServer(void)
{
	//send the command for sending beck the Mixes to the Server
	sendBluetoothCommand("_sendBackMixes");
}

void MainWindow::sendBluetoothCommand(QString command)
{
	//emit the signal for sending data via bluetooth
	emit sendCommand(command);
}

void MainWindow::writeListWidget(void)
{
	ui->listWidget_Mixes->clear();			//clear the current list

	for(int i = 0; i < mixtures.size(); i++)
	{
		QPushButton *button = new QPushButton;			//create a new button
		button->setText(mixtures.at(i)->getName());		//set the text of the button

		QListWidgetItem *item = new QListWidgetItem;	//create a new ListWidgetItem
		QSize size;
		size.setHeight(screenHeight/15);				//set the heigth of the button
		item->setSizeHint(size);
		ui->listWidget_Mixes->addItem(item); //add the ListWidgetItem to the ListWidget
		ui->listWidget_Mixes->setItemWidget(item, button);	//set the button as widget
															//...of the item

		/*
		 * connect the signal 'clicked' of the Button with the slot 'getCommandName' of
		 * the mixture to get the name of the wished mix from the Object
		 */

		connect(button,	SIGNAL(clicked(bool)), mixtures.at(i),
						SLOT(getCommandName()));

		/*
		 * connect the signal 'sendName' of the mixture with the slot 'ButtonSlot'
		 * of the Mainwindow to send the name of the mixture via bluetooth to the server
		 */

		connect(mixtures.at(i), SIGNAL(sendName(QString)), this,
								SLOT(ButtonSlot(QString)));
	}
}

void MainWindow::ButtonSlot(QString name)
{
	//send the command for the requested mixture
	sendBluetoothCommand("/" + name);
}

void MainWindow::connected(QString peerName)
{
	qDebug() << peerName;
}
