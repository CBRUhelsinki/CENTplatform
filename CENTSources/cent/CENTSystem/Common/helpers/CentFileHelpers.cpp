#include "CentFileHelpers.h"

#include <QDebug>
#include <QDir>
#include <QTime>

#include "PatientDataCenterConfig.h"

//static
bool CentFileHelpers::copyFiles(const QString& sourceDirectory, const QString& destinationDirectory, const QStringList& extensions)
{
	QDir sourceDir(sourceDirectory);
	QDir destDir(destinationDirectory);
	QString destFile;

	if (sourceDir.exists() && destDir.exists())
	{
		QFileInfoList fileInfoList = sourceDir.entryInfoList(QDir::Files);
		foreach (const QFileInfo& fileInfo, fileInfoList)
		{
			if (extensions.isEmpty() || extensions.contains(fileInfo.suffix()))
			{
				destFile = destinationDirectory + QDir::separator() + fileInfo.fileName();
				if(QFile::exists(destFile) && !QFile::remove(destFile)) //TODO: when we need a conditional copy it can be necessary to add flags/enum to parameter list
				{
					qCritical() << "Could overwrite file: " << destFile;
					return false;
				}
				if (! QFile::copy(fileInfo.absoluteFilePath(), destFile))
				{
					qCritical() << "Could not copy file: " << fileInfo.absoluteFilePath() << " to patient IEP folder.";
					return false;
				}
			}
		}
	}

	return true;
}

bool CentFileHelpers::copyFolder(QString sourceFolder, QString destFolder)
{
	QDir sourceDir(sourceFolder);
	QDir destDir(destFolder);
	if(!sourceDir.exists())
	{
		return false;
	}
	if(!destDir.exists())
	{
		destDir.mkdir(destFolder);
	}

	if(!copyFiles(sourceFolder, destFolder, QStringList()))
	{
		return false;
	}

	QStringList dirs = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	foreach(QString dirName, dirs)
	{
		if(!copyFolder(sourceFolder + QDir::separator() + dirName, destFolder + QDir::separator() + dirName))
		{
			return false;
		}
	}
	return true;
}

//static
bool CentFileHelpers::createPath(QString& path, const QStringList& dirList)
{
	if(dirList.length() < 1)
	{
		qCritical() << __FUNCTION__ << "bad parameter count while creating: " << path;
		return false;
	}

	foreach(QString dirName, dirList)
	{
		path.append(QDir::separator() + dirName);

		QDir dir = QDir(path);
		if(! dir.exists(path))
		{
			if(! dir.mkdir(path))
			{
				qCritical() << __FUNCTION__ << "Could not create directory: " << path;
				return false;
			}
		}
	}

	return true;
}

//static
bool CentFileHelpers::writeConfigFile(const QString& filePath, const QStringList& valueList)
{
	QFile configFile(filePath);
	if(! configFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		qCritical() << __FUNCTION__ << "Could not create OpenVibe configuration file: " << filePath;
		return false;
	}

	QTextStream stream(&configFile);
	stream << PatientDataCenterConstants::OV_CONFIG_OPENING_TAG << endl;
	foreach(const QString& str, valueList)
	{
		stream << "\t" << PatientDataCenterConstants::OV_SETTING_FORMATTER.arg(str) << endl;
	}
	stream << PatientDataCenterConstants::OV_CONFIG_CLOSING_TAG;

	configFile.close();
	return true;
}

//static
bool CentFileHelpers::parseConfigFile(const QString& filePath, QStringList& values)
{
	//TODO: it would be better to use QDomDocument to parse xml format file
	QFile configFile(filePath);
	if(! configFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qCritical() << __FUNCTION__ << "Could not open OpenVibe configuration file: " << filePath;
		return false;
	}

	QTextStream stream(&configFile);
	QString content = stream.readAll();
	configFile.close();
	values.clear();

	int i = content.indexOf(PatientDataCenterConstants::OV_CONFIG_OPENING_TAG, 0, Qt::CaseInsensitive);
	if(i < 0)
	{
		qCritical() << __FUNCTION__ << "Corrupted OpenVibe configuration file: " << filePath;
		return false;
	}

	content.remove(0, i + PatientDataCenterConstants::OV_CONFIG_OPENING_TAG.length());

	while(true)
	{
		i = content.indexOf(PatientDataCenterConstants::OV_SETTING_OPENING_TAG, 0, Qt::CaseInsensitive);
		if(i < 0)
		{
			break;
		}
		content.remove(0, i + PatientDataCenterConstants::OV_SETTING_OPENING_TAG.length());

		i = content.indexOf(PatientDataCenterConstants::OV_SETTING_CLOSING_TAG, 0, Qt::CaseInsensitive);
		if(i < 0)
		{
			qCritical() << __FUNCTION__ << "Corrupted OpenVibe configuration file: " << filePath;
			return false;
		}
		values.append(content.left(i));
		content.remove(0, i + PatientDataCenterConstants::OV_SETTING_CLOSING_TAG.length());
	}

	i = content.indexOf(PatientDataCenterConstants::OV_CONFIG_CLOSING_TAG, 0, Qt::CaseInsensitive);
	if(i < 0)
	{
		qCritical() << __FUNCTION__ << "Corrupted OpenVibe configuration file: " << filePath;
		return false;
	}

	return values.count();
}

//static
bool CentFileHelpers::findFiles(const QString& dirName, const QString& filename, QStringList& filesFound)
{
	filesFound.clear();

	QDir dir(dirName);
	if(!dir.exists())
	{
		qCritical() << dirName << "Does not exist";
		return false;
	}
	QStringList found;
	//search for subdirs
	QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(const QString& name, subdirs)
	{
		if(findFiles(dir.path() + QDir::separator() + name, filename, found))
		{
			filesFound += found;
		}
		else
		{
			return false;
		}
	}

	QFileInfoList infoList = dir.entryInfoList(QStringList(filename), QDir::Files);
	foreach( QFileInfo info, infoList)
	{
		filesFound += info.absoluteFilePath();
	}
	return true;
}

bool CentFileHelpers::verifyWriteRightsInFolder(QString dir)
{
	if(!QFile::exists(dir))
	{
		qWarning() << "Folder doesn't exist: " << dir;
		return false;
	}
	//Test if we have write rights in that folder
	qsrand(QTime::currentTime().msec());
	QString fName;
	while(1)
	{
		fName = dir + QDir::separator() + QString::number(qrand()) + ".test";
		if(!QFile::exists(fName))
		{
			break;
		}
	}

	QFile file(fName);
	if(!file.open( QIODevice::WriteOnly |  QIODevice::Truncate))
	{
		qCritical() << __FUNCTION__ << "Could not create: " << file.fileName();
		//TODO: Show error to the user
		file.close();
		return false;
	}
	file.close();
	QFile::remove(file.fileName());
	return true;
}

bool CentFileHelpers::verifyDirContainingFiles(const QString& parentPath, const QStringList& filesList)
{
	QDir dir(parentPath);
	if(!dir.exists())
	{
		qWarning() << "Folder doesn't exist: " << dir;
		return false;
	}
	QStringList filesInParent = dir.entryList(QDir::Files);
	foreach(const QString& name, filesList)
	{
		if(!filesInParent.contains(name))
		{
			qWarning() << "Folder " << dir.absolutePath() << " missing file: " << name;
			return false;
		}
	}
	return true;
}

bool CentFileHelpers::findFoldersWithFiles(const QString& parentPath, const QStringList& filesList, QStringList& foundDirs, int maxDepth/* = 1*/)
{
	foundDirs.clear();

	QDir parentDir(parentPath);
	if(! parentDir.exists())
	{
		qCritical() << parentPath << " does not exist!";
		return false;
	}

	if(maxDepth > 0)
	{
		QStringList localFound;

		QStringList subdirs = parentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
		foreach(const QString& name, subdirs)
		{
			if(findFoldersWithFiles(parentDir.path() + QDir::separator() + name, filesList, localFound, maxDepth - 1))
			{
				foundDirs += localFound;
			}
		}
	}

	if(verifyDirContainingFiles(parentPath, filesList))
	{
		foundDirs += parentPath;
	}
	return foundDirs.count() > 0;
}
