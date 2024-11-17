#include "documentdialog.h"
#include <QMessageBox>

DocumentDialog::DocumentDialog(QWidget *parent, QString id, QString title, QString content) :
    QDialog(parent) {
  setupUi(this);
  setWindowTitle("Edit document");
  this->id = id;
  lineEdit->setText(title);
  textEdit->setHtml(content);
}

void DocumentDialog::on_buttonBox_accepted() {
  emit dialogAccepted(this->id, lineEdit->text(), textEdit->toHtml());
  accept();
}

void DocumentDialog::on_buttonBox_clicked(QAbstractButton *button) {
  QDialogButtonBox::StandardButton standardButton = buttonBox->standardButton(button);

  if (standardButton == QDialogButtonBox::Discard) {
    emit dialogDiscarded(this->id);
    close();
  }
}

void DocumentDialog::insertImageToTextEdit(const QImage &image) {
  if (image.isNull())
    return;

  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);
  image.save(&buffer, "PNG");
  QString base64Data = QString::fromLatin1(byteArray.toBase64());

  int width = 800;
  QString imgHtml = QString("<img src='data:image/png;base64,%1' width = '%2' >")
                      .arg(base64Data)
                      .arg(width);
  textEdit->insertHtml("<br></br>");
  textEdit->insertHtml(imgHtml);
}

void DocumentDialog::on_addImageButton_clicked() {
  QString filePath = QFileDialog::getOpenFileName(this, tr("Select Image"), QString(), tr("Images (*.png *.jpg *.jpeg *.bmp)"));

  if (!filePath.isEmpty()) {
    QImage image(filePath);
    if (!image.isNull()) {
      insertImageToTextEdit(image);
    } else {
      QMessageBox::warning(this, tr("Error"), tr("Failed to load the selected image."));
    }
  }
}

void DocumentDialog::on_toggleBoldButton_clicked() {
  QTextCursor cursor = textEdit->textCursor();
  if (!cursor.hasSelection()) {
    QMessageBox::information(this, tr("No Selection"), tr("Please select text to make bold."));
    return;
  }

  QTextCharFormat format = cursor.charFormat();
  format.setFontWeight(format.fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);

  cursor.mergeCharFormat(format);
}

void DocumentDialog::on_increaseButton_clicked() {
  QTextCursor cursor = textEdit->textCursor();
  if (!cursor.hasSelection()) {
    QMessageBox::information(this, tr("No Selection"), tr("Please select text to adjust font size."));
    return;
  }

  QTextCharFormat format = cursor.charFormat();
  int currentSize = format.fontPointSize() > 0 ? format.fontPointSize() : 12;
  format.setFontPointSize(currentSize + 2);
  cursor.mergeCharFormat(format);
}

void DocumentDialog::on_decreaseButton_clicked() {
  QTextCursor cursor = textEdit->textCursor();
  if (!cursor.hasSelection()) {
    QMessageBox::information(this, tr("No Selection"), tr("Please select text to adjust font size."));
    return;
  }

  QTextCharFormat format = cursor.charFormat();
  int currentSize = format.fontPointSize() > 0 ? format.fontPointSize() : 12;
  format.setFontPointSize(currentSize - 2);
  cursor.mergeCharFormat(format);
}
