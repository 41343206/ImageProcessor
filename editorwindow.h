#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QToolBar>
#include <QAction>
#include <QColorDialog>
#include <QSpinBox>

class DrawableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit DrawableLabel(QWidget *parent = nullptr);
    void setImage(const QImage &image);
    void setPenColor(const QColor &color);
    void setPenWidth(int width);
    QImage getImage() const { return canvasImage; }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);

    QImage canvasImage;
    bool drawing;
    QPoint lastPoint;
    QColor penColor;
    int penWidth;
};

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(const QImage &image, QWidget *parent = nullptr);
    ~EditorWindow();

private slots:
    void saveAs();
    void choosePenColor();
    void changePenWidth(int width);

private:
    void createActions();
    void createToolbar();

    DrawableLabel *drawableLabel;
    QToolBar *toolbar;
    QAction *saveAsAction;
    QAction *penColorAction;
    QSpinBox *penWidthSpinBox;
    QColor currentPenColor;
    int currentPenWidth;
};

#endif // EDITORWINDOW_H
