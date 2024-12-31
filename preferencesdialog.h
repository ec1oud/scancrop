// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui
{
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PreferencesDialog *m_ui;

private slots:
    void on_saveFormatBox_currentTextChanged(const QString &f);
};

#endif // PREFERENCESDIALOG_H
