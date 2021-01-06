#ifndef DRAWINGBOARDTEMPLATEDLG_H
#define DRAWINGBOARDTEMPLATEDLG_H

#include <QDialog>

namespace Ui {
class DrawingBoardTemplateDlg;
}

class DrawingBoardTemplateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DrawingBoardTemplateDlg(QWidget *parent = nullptr);
    ~DrawingBoardTemplateDlg();

    QString getTemplateFilePath() const;

private:
    Ui::DrawingBoardTemplateDlg *ui;
};

#endif // DRAWINGBOARDTEMPLATEDLG_H
