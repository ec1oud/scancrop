// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common.h"
#include "imagescanner.h"
#include "settings.h"
#include <sane/saneopts.h>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

/**
	This function borrowed from xscanimage.c
static SANE_Word
get_resolution (SANE_Handle dev)
{
	SANE_Status status;
	SANE_Word resolution = 300;
	SANE_Int num_options, i;
	const SANE_Option_Descriptor *option_desc;

	status =
		sane_control_option (dev, 0, SANE_ACTION_GET_VALUE, &num_options, 0);
	if (status != SANE_STATUS_GOOD)
	{
		qDebug("failed to get the number of options");
		return 0;
	}

	for (i = 1; i < num_options; i++)
	{
		option_desc = sane_get_option_descriptor (dev, i);
		if (option_desc)
			if (option_desc->name)
		{
			qDebug() << "see option " << option_desc->name;
			if (strncmp (option_desc->name, SANE_NAME_SCAN_RESOLUTION,
				sizeof (SANE_NAME_SCAN_RESOLUTION)) == 0)
			{
				status = sane_control_option (dev, i, SANE_ACTION_GET_VALUE,
					&resolution, 0);
				if (status == SANE_STATUS_GOOD)
				{
					if (option_desc->type == SANE_TYPE_INT)
						return resolution;
					else if (option_desc->type == SANE_TYPE_FIXED)
						return (SANE_Word) SANE_UNFIX (resolution);
				}
				return 0;
			}
		}
	}
	return 0;
}
*/


ImageScanner* ImageScanner::instance()
{
	static ImageScanner* self = new ImageScanner();
	return self;
}


ImageScanner::ImageScanner()
{
}


QStringList ImageScanner::allScannerNames()
{
	QStringList ret;
	m_allDevices.clear();
	//	if (m_allDevices.isEmpty())
	//	{
	const SANE_Device** deviceList = NULL;
	SANE_Status sts = sane_init (0, 0);
	if (sts != SANE_STATUS_GOOD)
		return ret;
	sts = sane_get_devices(&deviceList, SANE_FALSE);
	qDebug() << "get devices:" << sane_strstatus(sts);

	const SANE_Device** deviceListRem = deviceList;
	const SANE_Device* dev = *deviceListRem;
	while (dev)
	{
		qDebug() << dev->name;
		m_allDevices.append(dev);
		dev = *(++deviceListRem);
	}
	//	}
	foreach (const SANE_Device* dev, m_allDevices)
		ret << QString("%1 %2 %3").arg(dev->vendor).arg(dev->model).arg(dev->type);
	return ret;
}

void ImageScanner::setSequence(int start, int end)
{
	m_sequenceStart = start;
	m_sequence = start;
	m_sequenceEnd = end;
}

QFileInfo ImageScanner::nextImageOutput()
{
	QFileInfo ret(QString("%1/image%2.jpg").arg(
		Settings::instance()->stringOrDefault(SETTING_GROUP_SESSION, "sequenceDir", ""))
//				m_outputImagePath.fileName()
		.arg(m_sequence, 4, 10, QLatin1Char('0')));
	return ret;
}

void ImageScanner::scan(QString mediaType)
{
	m_mediaType = mediaType;
	start();
}

void ImageScanner::run()
{
	QString scannerDev = Settings::instance()->chosenScanner();
	if (scannerDev.isEmpty())
	{
		QMessageBox::warning(QApplication::activeWindow(), tr(STR_PRODUCT),
			tr("Please choose a scanner first (in preferences)."));
		emit done(QImage());
	}
    SANE_Status status = sane_open(scannerDev.toLatin1().constData(), &m_scanner);
	if (status != SANE_STATUS_GOOD)
	{
		QMessageBox::critical(QApplication::activeWindow(), tr(STR_PRODUCT),
			tr("Failed to open scanner: '%1'").arg(scannerDev));
		emit done(QImage());
	}
	setOptions(m_scanner, m_mediaType);
	//	qDebug("resolution: %d\n", get_resolution(m_scanner));
	getOptions(m_scanner);
	status = sane_start (m_scanner);
	if (status != SANE_STATUS_GOOD)
	{
		QMessageBox::critical(QApplication::activeWindow(), tr(STR_PRODUCT),
			tr("sane_start failed for scanner: '%1'").arg(scannerDev));
		emit done(QImage());
	}
	SANE_Parameters params;
	//	params.pixels_per_line = area.width();
	//	params.lines = area.height();
	status = sane_get_parameters(m_scanner, &params);
	if (status != SANE_STATUS_GOOD)
	{
		QMessageBox::critical(QApplication::activeWindow(), tr(STR_PRODUCT),
			tr("sane_get_parameters failed for scanner: '%1'").arg(scannerDev));
		emit done(QImage());
	}
	qDebug("image type is %d; RGB (which we hope for) is %d", params.format, SANE_FRAME_RGB);
	qDebug("Pixels per line: %d lines: %d depth: %d\n",
		params.pixels_per_line, params.lines, params.depth);
	if (params.format != SANE_FRAME_RGB)
	{
		QMessageBox::critical(QApplication::activeWindow(), tr(STR_PRODUCT),
			tr("This application supports only RGB images, and\n"
			"your scanner apparently doesn't (or at least not by default)."));
		emit done(QImage());
	}

	QImage all(params.pixels_per_line, params.lines, QImage::Format_RGB888);
	int line = 0;
	emit progressRange(0, params.lines);
    while (status == SANE_STATUS_GOOD)
    {
		//		SANE_Byte data[params.bytes_per_line];
		SANE_Int len = 0;
		status = sane_read(m_scanner, all.scanLine(line++), params.bytes_per_line, &len);
		emit progress(line);
	}

	// Break up and save images according to matrix
	QImage ret;
	QSize matrixDims = Settings::instance()->matrixDims(m_mediaType);
	int cellW = all.width() / matrixDims.width();
	int cellH = all.height() / matrixDims.height();
	for (int row = 0; row < matrixDims.height(); ++row)
		for (int col = 0; col < matrixDims.width() && m_sequence < m_sequenceEnd; ++col)
		{
			QImage cell = all.copy(col * cellW, row * cellH, cellW, cellH);
			if (ret.isNull())
				ret = cell;
			cell.save(nextImageOutput().absoluteFilePath());
			++m_sequence;
		}

	emit done(ret);
}

QString ImageScanner::scannerDev(int idx)
{
	return m_allDevices.at(idx)->name;
}


void ImageScanner::setOptions(SANE_Handle dev, QString mediaType)
{
	SANE_Status status;
	SANE_Word val;
	SANE_Int num_options, i;
	const SANE_Option_Descriptor *option_desc;
	QRectF individualScanArea = Settings::instance()->scanGeometry(mediaType);
	QSize matrixDims = Settings::instance()->matrixDims(mediaType);
	QRectF scanArea = individualScanArea;
	scanArea.setWidth(individualScanArea.width() * matrixDims.width());
	scanArea.setHeight(individualScanArea.height() * matrixDims.height());

	status = sane_control_option (dev, 0, SANE_ACTION_GET_VALUE, &num_options, 0);
	if (status != SANE_STATUS_GOOD)
	{
		qDebug("failed to get the number of options");
		return;
	}

	for (i = 1; i < num_options; i++)
	{
		option_desc = sane_get_option_descriptor (dev, i);
		if (option_desc)
			if (option_desc->name)
		{
			SANE_String_Const optName = option_desc->name;
			//		  qDebug() << "see option " << optName;
			if (strncmp (optName, SANE_NAME_SCAN_RESOLUTION,
				sizeof (SANE_NAME_SCAN_RESOLUTION)) == 0)
			{
				val = Settings::instance()->intOrDefault(SETTING_GROUP_MAIN, "resolution", 300);
				status = sane_control_option (dev, i, SANE_ACTION_SET_VALUE, &val, 0);
				if (status != SANE_STATUS_GOOD)
					qDebug() << "failed to set resolution";
			}
			else if (strncmp (optName, SANE_NAME_SCAN_TL_X,
				sizeof (SANE_NAME_SCAN_TL_X)) == 0)
			{
				val = SANE_FIX(scanArea.topLeft().x());
				status = sane_control_option (dev, i, SANE_ACTION_SET_VALUE, &val, 0);
				if (status != SANE_STATUS_GOOD)
					qDebug() << "failed to set resolution";
			}
			else if (strncmp (optName, SANE_NAME_SCAN_TL_Y,
							  sizeof (SANE_NAME_SCAN_TL_Y)) == 0)
			{
				val = SANE_FIX(scanArea.topLeft().y());
				status = sane_control_option (dev, i, SANE_ACTION_SET_VALUE, &val, 0);
				if (status != SANE_STATUS_GOOD)
					qDebug() << "failed to set resolution";
			}
			else if (strncmp (optName, SANE_NAME_SCAN_BR_X,
							  sizeof (SANE_NAME_SCAN_BR_X)) == 0)
			{
				val = SANE_FIX(scanArea.bottomRight().x());
				status = sane_control_option (dev, i, SANE_ACTION_SET_VALUE, &val, 0);
				if (status != SANE_STATUS_GOOD)
					qDebug() << "failed to set resolution";
			}
			else if (strncmp (optName, SANE_NAME_SCAN_BR_Y,
							  sizeof (SANE_NAME_SCAN_BR_Y)) == 0)
			{
				val = SANE_FIX(scanArea.bottomRight().y());
				status = sane_control_option (dev, i, SANE_ACTION_SET_VALUE, &val, 0);
				if (status != SANE_STATUS_GOOD)
					qDebug() << "failed to set resolution";
			}
		}
	}
}


void ImageScanner::getOptions(SANE_Handle dev)
{
	SANE_Status status;
	SANE_Word val;
	SANE_Int num_options, i;
	const SANE_Option_Descriptor *option_desc;

	status = sane_control_option (dev, 0, SANE_ACTION_GET_VALUE, &num_options, 0);
	if (status != SANE_STATUS_GOOD)
	{
		qDebug("failed to get the number of options");
		return;
	}

	qDebug("========= option values");
	for (i = 1; i < num_options; i++)
	{
		option_desc = sane_get_option_descriptor (dev, i);
		if (option_desc && option_desc->name)
		{
			//		  qDebug() << "see option " << option_desc->name << " with type " << option_desc->type;
			//		  if (option_desc->type == SANE_TYPE_INT || option_desc->type == SANE_TYPE_FIXED)
			if (!strstr(option_desc->name, SANE_NAME_GAMMA_VECTOR))
			{
				status = sane_control_option (dev, i, SANE_ACTION_GET_VALUE, &val, 0);
				if (status == SANE_STATUS_GOOD)
				{
					if (option_desc->type == SANE_TYPE_INT)
						qDebug() << "see option " << option_desc->name << " with type "
							<< option_desc->type << " and value " << val;
					else if (option_desc->type == SANE_TYPE_FIXED)
						qDebug() << "see option " << option_desc->name << " with type "
								<< option_desc->type << " and FIXED value " << SANE_UNFIX(val);
				}
			}
		}
	}
}
