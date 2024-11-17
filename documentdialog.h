#ifndef DOCUMENTDIALOG_H
#define DOCUMENTDIALOG_H

#include "ui_documentdialog.h"
#include <QApplication>
#include <QBuffer>
#include <QDialog>
#include <QFileDialog>
#include <QImage>
#include <QObject>
#include <QTextCharFormat>
#include <QTextCursor>

class DocumentDialog : public QDialog, private Ui::Dialog {
  Q_OBJECT
public:
  DocumentDialog(QWidget *parent, QString id, QString title, QString content);

  void insertImageToTextEdit(const QImage &image);

private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_clicked(QAbstractButton *button);
  void on_addImageButton_clicked();
  void on_toggleBoldButton_clicked();

  void on_increaseButton_clicked();

  void on_decreaseButton_clicked();

signals:
  void dialogAccepted(QString id, QString title, QString content);
  void dialogDiscarded(QString id);

private:
  QString id;
};

#endif  // DOCUMENTDIALOG_H
