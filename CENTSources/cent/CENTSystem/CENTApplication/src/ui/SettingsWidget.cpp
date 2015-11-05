#include "SettingsWidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QStyleOption>
#include <QStylePainter>
#include <QVBoxLayout>

#include "CentConstants.h"
#include "CentFileHelpers.h"
#include "Connect.h"
#include "ErrorHandler.h"

namespace
{
	const QString BOLD_FORMATER = "<b>%1</b>";
}

SettingsWidget::SettingsWidget(QSettings& settings, QWidget *parent)
	: QWidget(parent)
	, m_settings(settings)
{
	setupLayout();
}

void SettingsWidget::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
	m_backupLabel->setText(BOLD_FORMATER.arg(m_settings.value(CENT::Settings::DATA_REPLICATION_PATH).toString()));
}

void SettingsWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.init(this);
	QStylePainter p(this);
	p.drawPrimitive(QStyle::PE_Widget, opt);
	QWidget::paintEvent(event);
}

void SettingsWidget::onChangeClicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"),
					"/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dir.isEmpty())
	{
		return;
	}
	//Test if we have write rights in that folder
	if(!CentFileHelpers::verifyWriteRightsInFolder(dir))
	{
		qCritical() << __FUNCTION__ << "No write rights in: " << dir;
		//TODO: Show error to the user
		ErrorHandler errHandler(*this);
		errHandler.onError(tr("You do not have write rights to that folder, choose a different one"));
		return;
	}

	//Folder looks ok, let's save it's location
	m_settings.setValue(CENT::Settings::DATA_REPLICATION_PATH, dir);
	m_settings.sync();
	m_backupLabel->setText(BOLD_FORMATER.arg(dir));
	emit backupPathChanged();
}

void SettingsWidget::setupLayout()
{
	QHBoxLayout* backupLayout = new QHBoxLayout;
	m_backupLabel = new QLabel(BOLD_FORMATER.arg(m_settings.value(CENT::Settings::DATA_REPLICATION_PATH).toString()));
	QPushButton* button = new QPushButton(tr("Change"));
	CENT::connect(button, SIGNAL(clicked()), this, SLOT(onChangeClicked()));

	backupLayout->addWidget(m_backupLabel);
	backupLayout->addStretch();
	backupLayout->addWidget(button);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addStretch();
	layout->addWidget(new QLabel(tr("Current data backup path:")));
	layout->addLayout(backupLayout);
	layout->addStretch();

	setLayout(layout);
}
