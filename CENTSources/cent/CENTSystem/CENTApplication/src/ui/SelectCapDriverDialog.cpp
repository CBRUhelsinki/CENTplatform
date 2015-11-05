#include "SelectCapDriverDialog.h"
#include "CentConstants.h"

#include <QtGui>
#include <QSettings>
#include <QList>
#include <QLabel>
#include <QLineEdit>

namespace
{
	const int BUTTON_WIDTH = 100;
	const QSize INITIAL_SIZE(320, 300);

	const char* CAP_DRV_QUESTION_TEXT("Select cap driver?");
	const char* CAP_DRV_MAC_ADRRESS_TEXT("MAC Address (HH:HH:HH:HH:HH:HH)");
	const char* CAP_DER_INCORRECT_MAC_ADDRESS_TEXT("<font color='red'>Incorrect MAC Address!</font>");

		//Cent version of Acquisition server
		const QString USE_OSCILATOR_AS_SOURCE    = "--use-oscilator-as-source";
		const QString USE_OSCILATOR_AS_SOURCE_8  = "--use-oscilator-as-source-8";
		const QString USE_OSCILATOR_AS_SOURCE_20 = "--use-oscilator-as-source-20"; // not supported yet
		const QString USE_ENOBIO_API_DRIVER      = "--use-enobio-api-driver";
		const QString USE_ENOBIO_3G_API_DRIVER   = "--use-enobio-3g-api-driver -enobio-3g-mac %1"; // 8 channels
		const QString USE_ENOBIO_STARLAB_DRIVER  = "";	//this is the default driver so no cmd switch is necessary

	const char* CAP_DRV_4CH_OSCILATOR_TEXT("4 Channels Oscilator Generator");
	const char* CAP_DRV_8CH_OSCILATOR_TEXT("8 Channels Oscilator Generator");
	const char* CAP_DRV_4CH_ENOBIO_TEXT("4 Channels Enobio Api Cap Driver");
	const char* CAP_DRV_8CH_ENOBIO_TEXT("8 Channels Enobio 3G Api Cap Driver");
	const char* CAP_DRV_4CH_STARLAB_TEXT("4 Channels Starlab GUI Api Cap Driver");

	const char* OK_BUTTON_TEXT("Ok");
	const char* CANCEL_BUTTON_TEXT("Cancel");
	const char* APP_NAME_TEXT("Cent Application");

	const QString REG_EXP_MAC_ADDRESSr = "[0-9a-fA-F]{2}[:-][0-9a-fA-F]{2}[:-][0-9a-fA-F]{2}[:-][0-9a-fA-F]{2}[:-][0-9a-fA-F]{2}[:-][0-9a-fA-F]{2}";
}

SelectCapDriverDialog::SelectCapDriverDialog(QWidget* parent)
	: QDialog(parent)
{
	m_settings = new QSettings(QSettings::UserScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);

	initialize();

	resize(INITIAL_SIZE);
	setMinimumSize(INITIAL_SIZE);
	QList<QRadioButton*> buttons;
	foreach(CentData::CapDriverData capDriver, m_mapCapDrivers)
	{
		QRadioButton* radio = new QRadioButton(capDriver.name);
		connect(radio, SIGNAL(clicked()), this, SLOT(onRadioChanged()));

		buttons.append(radio);
	}

	m_okButton = new QPushButton(tr(OK_BUTTON_TEXT));
	m_okButton->setFixedWidth(BUTTON_WIDTH);
	m_okButton->setDisabled(true);
	QPushButton* cancelButton = new QPushButton(tr(CANCEL_BUTTON_TEXT));
	cancelButton->setFixedWidth(BUTTON_WIDTH);

	connect(m_okButton, SIGNAL( clicked() ), this, SLOT( onSetCapDriver() ) );
	connect(cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	QVBoxLayout *mainLayout = new QVBoxLayout;

	buttonLayout->addWidget(m_okButton);
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addStretch();

	mainLayout->addWidget(new QLabel(tr(CAP_DRV_QUESTION_TEXT)));
	mainLayout->addStretch();
	foreach(QRadioButton* button, buttons)
	{
		mainLayout->addWidget(button);
	}
	mainLayout->addStretch();
	
	// MAC address line editor
	QRegExp rx(REG_EXP_MAC_ADDRESSr);
	QValidator *validator = new QRegExpValidator(rx, this);
	m_macLabel = new QLabel(tr(CAP_DRV_MAC_ADRRESS_TEXT));
	m_macLabel->setVisible(false);

	//label as placeholder in layout to maintain height
	QLabel* fakeLabel = new QLabel(" ");
	fakeLabel->setMinimumHeight(15);

	QHBoxLayout* macLabelLayout = new QHBoxLayout;
	macLabelLayout->addWidget(m_macLabel);
	macLabelLayout->addWidget(fakeLabel);

	mainLayout->addLayout(macLabelLayout);

	m_macField = new QLineEdit;
	m_macField->setValidator(validator);
	m_macField->setVisible(false);

	//label as placeholder in layout to maintain height
	QLabel* fakeLabel2 = new QLabel(" ");
	fakeLabel2->setMinimumHeight(25);

	QHBoxLayout* macEditorLayout = new QHBoxLayout;
	macEditorLayout->addWidget(m_macField);
	macEditorLayout->addWidget(fakeLabel2);

	mainLayout->addLayout(macEditorLayout);

	m_macWarrningLabel = new QLabel(tr(CAP_DER_INCORRECT_MAC_ADDRESS_TEXT));
	m_macWarrningLabel->setVisible(false);

	QHBoxLayout* macIncorrectLabelLayout = new QHBoxLayout;
	macIncorrectLabelLayout->addWidget(m_macWarrningLabel);
	macIncorrectLabelLayout->addWidget(fakeLabel);
	mainLayout->addLayout(macIncorrectLabelLayout);

	mainLayout->addStretch();
	mainLayout->addLayout(buttonLayout);
	
	setModal(true);
	setAttribute(Qt::WA_QuitOnClose);

	setLayout(mainLayout);

	setWindowTitle(tr(APP_NAME_TEXT));
}

SelectCapDriverDialog::~SelectCapDriverDialog()
{
	delete m_settings;
}

void SelectCapDriverDialog::initialize()
{
	// Set current cap driver to unknown
	m_currentCapDriver.type = CentData::UnknowCapDriver;

	// Set CapDriverData { type, name, commandOfRun, mac address }
	// 4th channels cap drivers
	CentData::CapDriverData capDrv4ChEnobio    = { CentData::CapDriver4thChannels, tr(CAP_DRV_4CH_ENOBIO_TEXT), USE_ENOBIO_API_DRIVER };

	// 8th channels cap drivers
	CentData::CapDriverData capDrv8Channels    = { CentData::CapDriver8thChannels, tr(CAP_DRV_8CH_ENOBIO_TEXT), USE_ENOBIO_3G_API_DRIVER };

#ifdef _DEBUG
	// 4th channels cap drivers for debug version
	CentData::CapDriverData capDrv4ChOscilator = { CentData::CapDriver4thChannels, tr(CAP_DRV_4CH_OSCILATOR_TEXT), USE_OSCILATOR_AS_SOURCE };
	CentData::CapDriverData capDrv4ChStarlab   = { CentData::CapDriver4thChannels, tr(CAP_DRV_4CH_STARLAB_TEXT),   USE_ENOBIO_STARLAB_DRIVER };
	// 8th channels cap drivers for debug version
	CentData::CapDriverData capDrv8ChOscilator = { CentData::CapDriver8thChannels, tr(CAP_DRV_8CH_OSCILATOR_TEXT), USE_OSCILATOR_AS_SOURCE_8 };
	
	m_mapCapDrivers.insert(capDrv4ChOscilator.name, capDrv4ChOscilator);
	m_mapCapDrivers.insert(capDrv4ChStarlab.name,   capDrv4ChStarlab);
	m_mapCapDrivers.insert(capDrv8ChOscilator.name, capDrv8ChOscilator);
#endif
	m_mapCapDrivers.insert(capDrv4ChEnobio.name,    capDrv4ChEnobio);
	m_mapCapDrivers.insert(capDrv8Channels.name,    capDrv8Channels);
}

void SelectCapDriverDialog::onRadioChanged()
{
	QRadioButton* button = qobject_cast<QRadioButton *>(sender());

	if(button->isChecked())
	{
		m_currentCapDriver = m_mapCapDrivers.value(button->text());
		m_okButton->setDisabled(false);
	}
	else
	{
		m_currentCapDriver.type = CentData::UnknowCapDriver;
	}

	if(m_currentCapDriver.name == CAP_DRV_8CH_ENOBIO_TEXT)
	{
		QString macAddress = m_settings->value(CENT::Settings::CAP_DRIVER_MAC_ADDRESS).toString();
		m_macField->setText(macAddress);
		m_macLabel->setVisible(true);
		m_macField->setVisible(true);
	}
	else
	{
		m_macLabel->setVisible(false);
		m_macField->setVisible(false);
	}
	m_macWarrningLabel->setVisible(false);
}

void SelectCapDriverDialog::onSetCapDriver()
{
	if(m_currentCapDriver.type != CentData::UnknowCapDriver)
	{
		m_settings->setValue(CENT::Settings::CAP_DRIVER_TYPE, m_currentCapDriver.type);
		if(m_currentCapDriver.name == CAP_DRV_8CH_ENOBIO_TEXT)
		{
			QString macAddress = m_macField->text();
			int len = macAddress.length();
			QValidator::State state = m_macField->validator()->validate(macAddress, len);
			if(state == QValidator::Acceptable)
			{
				m_settings->setValue(CENT::Settings::CAP_DRIVER_MAC_ADDRESS, m_macField->text());
				QString cmd = m_currentCapDriver.commandToRun.arg(m_macField->text());
				m_settings->setValue(CENT::Settings::CAP_DRIVER_CMD, cmd);
			}
			else
			{
				m_macWarrningLabel->setVisible(true);
				return;
			}
		}
		else
		{
			m_settings->setValue(CENT::Settings::CAP_DRIVER_CMD, m_currentCapDriver.commandToRun);
		}
		accept();
	}
	else
	{
		reject();
	}
}
