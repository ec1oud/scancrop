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

void PreferencesDialog::on_saveFormatBox_currentTextChanged(const QString &f)
{
    Settings::instance()->setString(SETTING_GROUP_MAIN, "format", f);
}

void PreferencesDialog::on_scanQualityBox_valueChanged(int q)
{
    Settings::instance()->setInt(SETTING_GROUP_MAIN, "scanQuality", q);
}

void PreferencesDialog::on_cropQualityBox_valueChanged(int q)
{
    Settings::instance()->setInt(SETTING_GROUP_MAIN, "cropQuality", q);
}
