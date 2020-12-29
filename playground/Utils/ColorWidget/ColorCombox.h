#ifndef COLORCOMBOX_H
#define COLORCOMBOX_H

#include <QToolButton>
#include <QRgb>

class ColorCombox : public QToolButton
{
    Q_OBJECT

public:
    ColorCombox(const QString &name = QString(), QWidget *parent = 0);
    ~ColorCombox();

    void setButtonIcon(const QString &imageFile, QColor color);

private:
    QMenu* createColorMenu(const char *slot, const char *slotColorBoard);
    QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);
    QIcon createColorIcon(QColor color);

public:
    void setColor(QRgb color);
    QRgb getColor();

signals:
    void sigColorChanged(QColor color);

private slots:
    void OnColorChanged();           // 文本颜色设置
    void OnShowColorBoard();         // 颜色板
    void changeColor(QColor color);

private:
    QString m_name;
    QColor m_color;
};

#endif
