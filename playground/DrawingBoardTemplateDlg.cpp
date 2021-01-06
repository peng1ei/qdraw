#include "DrawingBoardTemplateDlg.h"
#include "ui_DrawingBoardTemplateDlg.h"
#include <QModelIndex>

DrawingBoardTemplateDlg::DrawingBoardTemplateDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawingBoardTemplateDlg)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Drawing Board Template"));

    this->setFixedSize(680,420);
    //设置QListWidget的显示模式
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    ui->listWidget->setIconSize(QSize(200,200));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(10);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);

    QVector<QString> fileNames = { "../assets/black.jpg",
                                   "../assets/gray.jpg",
                                   "../assets/green.jpg",
                                 "../assets/pure_black.png",
                                 "../assets/pure_gray.png",
                                 "../assets/pure_white.png"};
    QStringList ls{"blackboard-black",
                  "blackboard-gray",
                  "blackboard-green",
                  "pure-black",
                  "pure-gray",
                  "pure-white"};
    int i = 0;
    for(auto tmp : fileNames)
    {
        //定义QListWidgetItem对象
        QListWidgetItem *imageItem = new QListWidgetItem;
        //为单元项设置属性
        imageItem->setIcon(QIcon(tmp));
        imageItem->setText(ls[i++]);
        //重新设置单元项图片的宽度和高度
        imageItem->setSizeHint(QSize(200,160));
        imageItem->setTextAlignment(Qt::AlignCenter);
        //将单元项添加到QListWidget中
        ui->listWidget->addItem(imageItem);
    }

    ui->listWidget->setCurrentRow(0);
}

DrawingBoardTemplateDlg::~DrawingBoardTemplateDlg()
{
    delete ui;
}

QString DrawingBoardTemplateDlg::getTemplateFilePath() const
{
    if (ui->listWidget->currentRow() < 0)
        return QString();

    QVector<QString> fileNames = { "../assets/black.jpg",
                                   "../assets/gray.jpg",
                                   "../assets/green.jpg",
                                 "../assets/pure_black.png",
                                 "../assets/pure_gray.png",
                                 "../assets/pure_white.png"};



    return fileNames[ui->listWidget->currentRow()];
}
