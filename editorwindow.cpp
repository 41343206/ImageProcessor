#include "editorwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>

// DrawableLabel Implementation
DrawableLabel::DrawableLabel(QWidget *parent)
    : QLabel(parent), drawing(false), penColor(Qt::red), penWidth(3)
{
    setMouseTracking(false);
}

void DrawableLabel::setImage(const QImage &image)
{
    canvasImage = image.copy();
    setPixmap(QPixmap::fromImage(canvasImage));
}

void DrawableLabel::setPenColor(const QColor &color)
{
    penColor = color;
}

void DrawableLabel::setPenWidth(int width)
{
    penWidth = width;
}

void DrawableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        drawing = true;
    }
}

void DrawableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        drawLineTo(event->pos());
    }
}

void DrawableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        drawLineTo(event->pos());
        drawing = false;
    }
}

void DrawableLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}

void DrawableLabel::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&canvasImage);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    lastPoint = endPoint;
    
    setPixmap(QPixmap::fromImage(canvasImage));
}

// EditorWindow Implementation
EditorWindow::EditorWindow(const QImage &image, QWidget *parent)
    : QMainWindow(parent), currentPenColor(Qt::red), currentPenWidth(3)
{
    setWindowTitle(tr("影像編輯器"));
    
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    drawableLabel = new DrawableLabel(this);
    drawableLabel->setImage(image);
    drawableLabel->setPenColor(currentPenColor);
    drawableLabel->setPenWidth(currentPenWidth);
    
    layout->addWidget(drawableLabel);
    setCentralWidget(centralWidget);
    
    createActions();
    createToolbar();
    
    resize(image.width() + 50, image.height() + 100);
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::createActions()
{
    saveAsAction = new QAction(tr("另存新檔"), this);
    saveAsAction->setShortcut(tr("Ctrl+S"));
    saveAsAction->setStatusTip(tr("將影像另存新檔"));
    connect(saveAsAction, &QAction::triggered, this, &EditorWindow::saveAs);
    
    penColorAction = new QAction(tr("筆刷顏色"), this);
    penColorAction->setStatusTip(tr("選擇筆刷顏色"));
    connect(penColorAction, &QAction::triggered, this, &EditorWindow::choosePenColor);
}

void EditorWindow::createToolbar()
{
    toolbar = addToolBar(tr("工具列"));
    
    toolbar->addAction(saveAsAction);
    toolbar->addSeparator();
    toolbar->addAction(penColorAction);
    
    toolbar->addWidget(new QLabel(tr(" 筆刷寬度: ")));
    penWidthSpinBox = new QSpinBox(this);
    penWidthSpinBox->setMinimum(1);
    penWidthSpinBox->setMaximum(50);
    penWidthSpinBox->setValue(currentPenWidth);
    connect(penWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &EditorWindow::changePenWidth);
    toolbar->addWidget(penWidthSpinBox);
}

void EditorWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("另存新檔"), ".",
        tr("PNG 檔案 (*.png);;JPEG 檔案 (*.jpg);;BMP 檔案 (*.bmp)"));
    
    if (!fileName.isEmpty()) {
        QImage image = drawableLabel->getImage();
        if (image.save(fileName)) {
            QMessageBox::information(this, tr("儲存成功"), 
                tr("影像已成功儲存！"));
        } else {
            QMessageBox::warning(this, tr("儲存失敗"), 
                tr("無法儲存影像檔案。"));
        }
    }
}

void EditorWindow::choosePenColor()
{
    QColor color = QColorDialog::getColor(currentPenColor, this, tr("選擇筆刷顏色"));
    if (color.isValid()) {
        currentPenColor = color;
        drawableLabel->setPenColor(currentPenColor);
    }
}

void EditorWindow::changePenWidth(int width)
{
    currentPenWidth = width;
    drawableLabel->setPenWidth(currentPenWidth);
}
