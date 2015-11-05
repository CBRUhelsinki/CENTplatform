#include "SettingsWidget.h"
#include "Settings.h"

#include <QtGui>

namespace
{
	const char* SETTINGS_TEXT      = "Settings";
	const char* CURRENT_VIDEO_TEXT = "Current video path:";
	const char* OPEN_TEXT          = "Open...";
	const char* OPEN_MOVIE_TEXT    = "Open Movie";
	const char* BLANK_MODE_TEXT    = "Blank Mode";
}

SettingsWidget::SettingsWidget(Settings* settings, QWidget* parent)
	: QDialog(parent)
	, m_settings(settings)
{

	QAbstractButton* openButton = new QPushButton(tr(OPEN_TEXT));
	connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
	
	m_filePathLabel = new QLabel(m_settings->getFileName());
	
	m_checkBox = new QCheckBox(tr(BLANK_MODE_TEXT));
	m_checkBox->setChecked(m_settings->getVideoMode() == Settings::NoVideo);
	connect(m_checkBox, SIGNAL(clicked()), this, SLOT(blankMode()));

	QHBoxLayout* openFileLayout = new QHBoxLayout;
	openFileLayout->addWidget(m_filePathLabel);
	openFileLayout->addWidget(openButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addStretch();
	mainLayout->addWidget(new QLabel(tr(CURRENT_VIDEO_TEXT)));
	mainLayout->addLayout(openFileLayout);
	mainLayout->addStretch();
	mainLayout->addWidget(m_checkBox);
	mainLayout->addStretch();

	setModal(true);
	setLayout(mainLayout);
	setWindowTitle(tr(SETTINGS_TEXT));
	setMinimumSize(sizeHint());
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr(OPEN_MOVIE_TEXT),QDir::homePath());

	if (!fileName.isEmpty())
	{
		m_filePathLabel->setText(fileName);
		m_settings->setVideoFile(fileName);
		m_checkBox->setChecked(false);
	}
}

void SettingsWidget::blankMode()
{
	m_settings->setBlankMode(m_checkBox->isChecked());
}
