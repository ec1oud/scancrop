// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMAGESCANNER_H
#define IMAGESCANNER_H
#include <sane/sane.h>

#include <QObject>
#include <QImage>
#include <QThread>
#include <QFileInfo>

class ImageScanner : public QThread
{
	Q_OBJECT
public:
	static ImageScanner* instance();
	void scan(QString mediaType);
	QStringList allScannerNames();
	QString scannerDev(int idx);

	/**
		@param path	prefix part of the file path, to which the index number
					and extension will be appended
	*/
//	void setPath(QFileInfo path) { m_outputImagePath = path; }

	/**
		@param start inclusive starting index
		@param end  exclusive ending index, e.g. if it's 1 then
					foo0000.jpg will be created but foo0001.jpg will not.
	*/
	void setSequence(int start = 0, int end = 1);

	QFileInfo nextImageOutput();

public slots:

signals:
	void progress(int val);
	void progressRange(int zero, int max);
	void done(QImage img);

protected:
    void run() override;

private:
	ImageScanner();
	void setOptions(SANE_Handle dev, QString mediaType);
	void getOptions(SANE_Handle dev);

	QList<const SANE_Device*> m_allDevices;
	SANE_Handle m_scanner;
	QString m_mediaType;
	int m_sequenceStart;
	int m_sequenceEnd;
	int m_sequence;
//	QImage m_scannedImage;
//	QFileInfo m_outputImagePath;
};

#endif // IMAGESCANNER_H
