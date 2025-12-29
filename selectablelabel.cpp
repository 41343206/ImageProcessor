#include "selectablelabel.h"

SelectableLabel::SelectableLabel(QWidget *parent)
    : QLabel(parent), selecting(false)
{
    setMouseTracking(false);
}

void SelectableLabel::setImage(const QImage &image)
{
    currentImage = image;
    setPixmap(QPixmap::fromImage(image));
}

void SelectableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !currentImage.isNull()) {
        // Ensure mouse position is within widget bounds
        QPoint pos = event->pos();
        if (rect().contains(pos)) {
            startPoint = pos;
            endPoint = pos;
            selecting = true;
        }
    }
    QLabel::mousePressEvent(event);
}

void SelectableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting) {
        endPoint = event->pos();
        update();
    }
    QLabel::mouseMoveEvent(event);
}

void SelectableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && selecting) {
        selecting = false;
        endPoint = event->pos();
        
        // Create normalized rectangle
        QRect selection = QRect(startPoint, endPoint).normalized();
        
        // Emit signal with the selection
        if (selection.width() > MIN_SELECTION_SIZE && selection.height() > MIN_SELECTION_SIZE) {
            emit selectionMade(selection);
        }
        
        update();
    }
    QLabel::mouseReleaseEvent(event);
}

void SelectableLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    
    if (selecting) {
        QPainter painter(this);
        painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
        painter.drawRect(QRect(startPoint, endPoint).normalized());
    }
}
