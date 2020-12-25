#include "ProgressBarDialog.h"
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

ProgressBarDialog::ProgressBarDialog(ProgressType type, QWidget* parent)
    : QDialog(parent), mType(type) {
    setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    mUiMsg = new QLabel(this);
    mUiProgressBar = new QProgressBar(this);

    mUiProgressBar->setOrientation(Qt::Horizontal);

    // 如果最小值和最大值都设置为0，进度条会显示了一个繁忙指示，而不会显示当前的值
    mUiProgressBar->setMinimum(0);
    if (mType == ProgressType::Busy)
        mUiProgressBar->setMaximum(0);
    else
        mUiProgressBar->setMaximum(100);
    mUiProgressBar->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(mUiMsg);
    layout->addWidget(mUiProgressBar);

    setLayout(layout);
    setFixedSize(400, 100);
}

void ProgressBarDialog::Update(const QString &text, int value)
{
    mUiMsg->setText(text);
    mUiProgressBar->setValue(value);
}

void ProgressBarDialog::SetProgressType(ProgressBarDialog::ProgressType type)
{
    mType = type;
    if (mType == ProgressType::Busy)
        mUiProgressBar->setMaximum(0);
    else
        mUiProgressBar->setMaximum(100);
}

void ProgressBarDialog::SetText(const QString &text)
{
    mUiMsg->setText(text);
}

