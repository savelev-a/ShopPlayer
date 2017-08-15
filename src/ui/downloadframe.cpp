#include "downloadframe.h"

#include <QVBoxLayout>

DownloadFrame::DownloadFrame(QWidget *parent) : QFrame(parent)
{
    this->resize(150, 50);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setLineWidth(5);
    this->setAutoFillBackground(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    label = new QLabel(this);
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);

    layout->addWidget(label);
    layout->addWidget(progressBar);
    setLayout(layout);
}

void DownloadFrame::showProgress(qint64 current, qint64 total, const QString &filename)
{
    setVisible(true);
    if(total == 0) total++;
    this->progressBar->setValue((current * 100) / total);
    this->label->setText(filename);
}
