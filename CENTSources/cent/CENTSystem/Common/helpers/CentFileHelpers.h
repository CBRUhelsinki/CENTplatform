#ifndef CENT_FILE_HELPERS_H
#define CENT_FILE_HELPERS_H

#include <QStringList>

class CentFileHelpers
{
public:
	enum FileOperation {
		OverwriteExisting = 0x01, 
		SkipExisting = 0x02
	};
	Q_DECLARE_FLAGS(FileOperations, FileOperation)

	// Copy files from source directory to destination directory, to filter files you can specify acceptable extensions.
	// Empty extensions means that all files will be copied
	static bool copyFiles(const QString& sourceDirectory, const QString& destinationDirectory, const QStringList& extensions);
	static bool copyFolder(QString sourceFolder, QString destFolder);
	
	static bool createPath(QString& path, const QStringList& dirList);
	static bool findFiles(const QString& dirName, const QString& filename, QStringList& filesFound);

	static bool parseConfigFile(const QString& filePath, QStringList& values);
	static bool writeConfigFile(const QString& filePath, const QStringList& valueList);

	static bool verifyWriteRightsInFolder(QString dirPath);
	static bool verifyDirContainingFiles(const QString& parentPath, const QStringList& filesList);

	static bool findFoldersWithFiles(const QString& parentPath, const QStringList& filesList, QStringList& foundDirs, int maxDepth = 1);
};

#endif // CENT_FILE_HELPERS_H
