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
    void showEvent(QShowEvent *event);

private:
    Ui::PreferencesDialog *m_ui;

private slots:
    void on_resolutionEdit_editingFinished();
    void on_tl_x_edit_editingFinished();
    void on_tl_y_edit_editingFinished();
    void on_br_x_edit_editingFinished();
    void on_br_y_edit_editingFinished();
    void on_scannerBox_currentIndexChanged(int index);
    void on_scanSizes_currentTextChanged(const QString &cur);
    void on_saveFormatBox_currentTextChanged(const QString &f);
};

#endif // PREFERENCESDIALOG_H
