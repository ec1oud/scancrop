// Copyright (C) 2024 Shawn Rutledge <s@ecloud.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SCANDIALOG_H
#define SCANDIALOG_H

#include <QDialog>
#include "imagescanner.h"

namespace Ui {
    class ScanDialog;
}

class ScanDialog : public QDialog {
    Q_OBJECT

public:
    ScanDialog(QWidget *parent = 0);
    ~ScanDialog();

public slots:
	void imageScanned(QImage img);

protected:
    void changeEvent(QEvent *e);
	void showEvent(QShowEvent* event);
	void update();

private:
    Ui::ScanDialog *m_ui;
	ImageScanner* m_scanner;
	QImage m_scannedImage;

private slots:
    void on_resolutionEdit_editingFinished();
    void on_tl_x_edit_editingFinished();
    void on_tl_y_edit_editingFinished();
    void on_br_x_edit_editingFinished();
    void on_br_y_edit_editingFinished();
    void on_scannerBox_currentIndexChanged(int index);
    void on_scanSizes_currentTextChanged(const QString &cur);
    void on_flipCB_toggled(bool checked);
    void on_scanButton_clicked();
    void on_scanSequenceFrom_valueChanged(int val);
    void on_outputChooseButton_clicked();
};

#endif // SCANDIALOG_H
