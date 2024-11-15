#include "documentdialog.h"

DocumentDialog::DocumentDialog(QWidget *parent, QString id, QString title, QString content) :
    QDialog(parent) {
  setupUi(this);
  this->id = id;
  lineEdit->setText(title);
  textEdit->setText(content);
}

void DocumentDialog::on_buttonBox_accepted() {
  emit dialogAccepted(this->id, lineEdit->text(), textEdit->toPlainText());
  accept();
}

void DocumentDialog::on_buttonBox_clicked(QAbstractButton *button) {
  QDialogButtonBox::StandardButton standardButton = buttonBox->standardButton(button);

  if (standardButton == QDialogButtonBox::Discard) {
    emit dialogDiscarded(this->id);
    close();
  }
}
