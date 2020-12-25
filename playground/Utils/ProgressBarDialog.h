#pragma once
#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QProgressBar;
QT_END_NAMESPACE

class ProgressBarDialog : public QDialog {
    Q_OBJECT
public:
    enum ProgressType {
        Normal,
        Busy
    };

    explicit ProgressBarDialog(ProgressType type, QWidget* parent = nullptr);
    void Update(const QString &text, int value);

    ProgressType GetProgressType() const { return mType; }
    void SetProgressType(ProgressType type);

    void SetText(const QString &text);

private:
    QLabel *mUiMsg;
    QProgressBar *mUiProgressBar;
    ProgressType mType;
};
