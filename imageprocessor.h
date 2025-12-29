#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QMainWindow>

#include<QAction>
#include<QMenu>
#include<QToolBar>
#include<QImage>
#include<QLabel>
#include "selectablelabel.h"

class ImageProcessor : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcessor(QWidget *parent = nullptr);
    ~ImageProcessor();
    void createActions();
    void createMenus();
    void createToolbars();
    void loadFile(QString filename);
private slots:
    void showOpenFile();
    void getZoomOut();
    void getZoomIn();
    void handleSelection(QRect selection);

private:
    QWidget     *central;
    QMenu       *fileMenu;
    QToolBar    *fileTool;
    QImage      img;
    QString     filename;
    SelectableLabel *imgWin;
    QAction     *OpenFileAction;
    QAction     *exitAction;
    QAction     *zoomInAction;
    QAction     *zoomOutAction;

};
#endif // IMAGEPROCESSOR_H
