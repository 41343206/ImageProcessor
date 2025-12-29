#ifndef SELECTABLELABEL_H
#define SELECTABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>

class SelectableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit SelectableLabel(QWidget *parent = nullptr);
    void setImage(const QImage &image);
    
    static const int MIN_SELECTION_SIZE = 5;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void selectionMade(QRect selection);

private:
    QPoint startPoint;
    QPoint endPoint;
    bool selecting;
    QImage currentImage;
};

#endif // SELECTABLELABEL_H
