// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common.h"
#include "scandialog.h"
#include "settings.h"
#include "ui_scandialog.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

ScanDialog::ScanDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ScanDialog),
	m_scanner(ImageScanner::instance())
{
    m_ui->setupUi(this);
    int idx = 0;
    int chosenIdx = 0;
    QString chosenType = Settings::instance()->stringOrDefault(SETTING_GROUP_MAIN, "geometry", "");
    for (const QString &mt : Settings::instance()->allMediaTypes()) {
        m_ui->scanSizes->addItem(mt);
        if (chosenType == mt)
            chosenIdx = idx;
        ++idx;
    }
    m_ui->scanSizes->setCurrentIndex(chosenIdx);
	connect(m_scanner, SIGNAL(progressRange(int,int)),
			m_ui->scanProgressBar, SLOT(setRange(int,int)));
	connect(m_scanner, SIGNAL(progress(int)),
			m_ui->scanProgressBar, SLOT(setValue(int)));
	connect(m_scanner, SIGNAL(done(QImage)),
			this, SLOT(imageScanned(QImage)));
}

ScanDialog::~ScanDialog()
{
    delete m_ui;
}

void ScanDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ScanDialog::imageScanned(QImage img)
{
	m_scannedImage = img;
    m_ui->scanSequenceFrom->setValue(m_ui->scanSequenceFrom->value() + 1);
    update();
}

void ScanDialog::showEvent(QShowEvent* /*event*/)
{
    qDebug() << "---- getting all scanners";
    /// @todo thread this
    for (const QString &scanner : ImageScanner::instance()->allScannerNames())
        m_ui->scannerBox->addItem(scanner);
    m_ui->scanProgressBar->setValue(0);
    m_ui->scanProgressBar->setValue(0);
    m_ui->resolutionEdit->setText(Settings::instance()->stringOrDefault(SETTING_GROUP_MAIN, "resolution", "300"));
    // m_ui->captureSequenceNumber->setValue(CopyCat::instance()->nextSequenceNumber());
    int next = m_ui->scanSequenceFrom->value();
    // TODO check for existing images, or store next-number in settings
    m_scanner->setSequence(next, next + 10000);
    update();
}


void ScanDialog::on_scannerBox_currentIndexChanged(int index)
{
    Settings::instance()->chosenScanner(ImageScanner::instance()->scannerDev(index));
    qDebug() << "max size" << ImageScanner::instance()->maxSize();
}

void ScanDialog::on_tl_x_edit_editingFinished()
{
    Settings *settings = Settings::instance();
    QString mediaType = m_ui->scanSizes->currentText();
    QRectF rect = settings->scanGeometry(mediaType);
    bool ok = false;
    rect.setLeft(m_ui->tl_x_edit->text().toDouble(&ok));
    if (ok) {
        settings->setScanGeometry(mediaType, rect);
    }
}

void ScanDialog::on_tl_y_edit_editingFinished()
{
    Settings *settings = Settings::instance();
    QString mediaType = m_ui->scanSizes->currentText();
    QRectF rect = settings->scanGeometry(mediaType);
    bool ok = false;
    rect.setTop(m_ui->tl_y_edit->text().toDouble(&ok));
    if (ok) {
        settings->setScanGeometry(mediaType, rect);
    }
}

void ScanDialog::on_br_x_edit_editingFinished()
{
    Settings *settings = Settings::instance();
    QString mediaType = m_ui->scanSizes->currentText();
    QRectF rect = settings->scanGeometry(mediaType);
    bool ok = false;
    rect.setRight(m_ui->br_x_edit->text().toDouble(&ok));
    if (ok) {
        settings->setScanGeometry(mediaType, rect);
    }
}

void ScanDialog::on_br_y_edit_editingFinished()
{
    Settings *settings = Settings::instance();
    QString mediaType = m_ui->scanSizes->currentText();
    QRectF rect = settings->scanGeometry(mediaType);
    bool ok = false;
    rect.setBottom(m_ui->br_y_edit->text().toDouble(&ok));
    if (ok) {
        settings->setScanGeometry(mediaType, rect);
    }
}

void ScanDialog::on_resolutionEdit_editingFinished()
{
    bool ok = false;
    int val = m_ui->resolutionEdit->text().toInt(&ok);
    if (ok)
        Settings::instance()->setInt(SETTING_GROUP_MAIN, "resolution", val);
}

void ScanDialog::on_scanSizes_currentTextChanged(const QString &cur)
{
    const QRectF rect = Settings::instance()->scanGeometry(cur);
    const bool flip = m_ui->flipCB->isChecked();
    m_ui->tl_x_edit->setText(QString::number(flip ? rect.top() : rect.left()));
    m_ui->tl_y_edit->setText(QString::number(flip ? rect.left() : rect.top()));
    m_ui->br_x_edit->setText(QString::number(flip ? rect.bottom() : rect.right()));
    m_ui->br_y_edit->setText(QString::number(flip ? rect.right() : rect.bottom()));
    Settings::instance()->setString(SETTING_GROUP_MAIN, "geometry", cur);
}

void ScanDialog::on_flipCB_toggled(bool)
{
    on_scanSizes_currentTextChanged(m_ui->scanSizes->currentText());
}

void ScanDialog::on_scanButton_clicked()
{
    int from = m_ui->scanSequenceFrom->value();
    m_scanner->setSequence(m_ui->scanSequenceFrom->value(), from + 10000);
    double x1, y1, x2, y2;
    auto parse = [this](double &v, QLineEdit *le) -> bool {
        bool ok = false;
        v = le->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::critical(this, tr("Failed to parse number"),
                                  tr("Failed to parse '%1'").arg(le->text()));
        }
        return ok;
    };
    if (!parse(x1, m_ui->tl_x_edit)) return;
    if (!parse(y1, m_ui->tl_y_edit)) return;
    if (!parse(x2, m_ui->br_x_edit)) return;
    if (!parse(y2, m_ui->br_y_edit)) return;
    QRectF bounds(x1, y1, x2 - x1, y2 - y1);
    qDebug() << "scan area: " << bounds;
    ImageScanner::instance()->scan(bounds);
}

void ScanDialog::update()
{
    m_ui->scanPathLabel->setText(m_scanner->nextImageOutput().absoluteFilePath());
}

void ScanDialog::on_scanSequenceFrom_valueChanged(int val)
{
    m_scanner->setSequence(val, val + 10000);
    update();
}

void ScanDialog::on_outputChooseButton_clicked()
{
    auto dir = QFileDialog::getExistingDirectory(this, tr("Save (temporary) scans to"));
    if (!dir.isEmpty())
        m_scanner->setScanDir(dir);
    update();
}

