// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common.h"
#include "imagescanner.h"
#include "preferencesdialog.h"
#include "settings.h"
#include "ui_preferencesdialog.h"
#include <QDebug>
#include <QImageWriter>

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::PreferencesDialog)
{
    m_ui->setupUi(this);
    int idx = 0;
    int chosenIdx = 0;
    QString chosenType = Settings::instance()->stringOrDefault(SETTING_GROUP_SESSION, "media-type", "");
    for (const QString &mt : Settings::instance()->allMediaTypes()) {
        m_ui->scanSizes->addItem(mt);
        if (chosenType == mt)
            chosenIdx = idx;
        ++idx;
    }
    m_ui->scanSizes->setCurrentIndex(chosenIdx);
    const QString selectedFormat = Settings::instance()->stringOrDefault(SETTING_GROUP_MAIN, "format", "");
    const QByteArray selectedFormatBA = selectedFormat.toLocal8Bit();
    bool selectedSupported = false;
    for (const auto &f : QImageWriter::supportedImageFormats()) {
        m_ui->saveFormatBox->addItem(QString::fromLocal8Bit(f));
        if (f == selectedFormatBA)
            selectedSupported = true;
    }
    if (selectedSupported)
        m_ui->saveFormatBox->setCurrentText(selectedFormat);
}

PreferencesDialog::~PreferencesDialog() { delete m_ui; }

void PreferencesDialog::changeEvent(QEvent *e)
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

void PreferencesDialog::showEvent(QShowEvent * /*event*/)
{
    /// @todo thread this
    for (const QString &scanner : ImageScanner::instance()->allScannerNames())
        m_ui->scannerBox->addItem(scanner);
}

void PreferencesDialog::on_scannerBox_currentIndexChanged(int index)
{
    Settings::instance()->chosenScanner(ImageScanner::instance()->scannerDev(index));
}

void PreferencesDialog::on_tl_x_edit_editingFinished()
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

void PreferencesDialog::on_tl_y_edit_editingFinished()
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

void PreferencesDialog::on_br_x_edit_editingFinished()
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

void PreferencesDialog::on_br_y_edit_editingFinished()
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

void PreferencesDialog::on_resolutionEdit_editingFinished()
{
    bool ok = false;
    int val = m_ui->resolutionEdit->text().toInt(&ok);
    if (ok)
        Settings::instance()->setInt(SETTING_GROUP_MAIN, "resolution", val);
}

void PreferencesDialog::on_scanSizes_currentTextChanged(const QString &cur)
{
    QRectF rect = Settings::instance()->scanGeometry(cur);
    m_ui->tl_x_edit->setText(QString::number(rect.left()));
    m_ui->tl_y_edit->setText(QString::number(rect.top()));
    m_ui->br_x_edit->setText(QString::number(rect.right()));
    m_ui->br_y_edit->setText(QString::number(rect.bottom()));
}

void PreferencesDialog::on_saveFormatBox_currentTextChanged(const QString &f)
{
    Settings::instance()->setString(SETTING_GROUP_MAIN, "format", f);
}
