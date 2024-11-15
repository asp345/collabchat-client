#ifndef DOCUMENTDIALOG_H
#define DOCUMENTDIALOG_H

#include "ui_documentdialog.h"
#include <QApplication>
#include <QDialog>
#include <QObject>

class DocumentDialog : public QDialog, private Ui::Dialog {
  Q_OBJECT
public:
  DocumentDialog(QWidget *parent, QString id, QString title, QString content);
private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_clicked(QAbstractButton *button);

signals:
  void dialogAccepted(QString id, QString title, QString content);
  void dialogDiscarded(QString id);

private:
  QString id;
};

#endif  // DOCUMENTDIALOG_H
