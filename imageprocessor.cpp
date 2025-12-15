#include "imageprocessor.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
ImageProcessor::ImageProcessor(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,400);
    initPixmap->fill(QColor(255,255,128));
    imgWin->resize(300,200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenus();
    createToolbars();
}

ImageProcessor::~ImageProcessor() {}


void ImageProcessor::createActions(){
    OpenFileAction = new QAction(QStringLiteral("開啟檔案&O"),this);
    OpenFileAction->setShortcut(tr("Ctrl+O"));
    OpenFileAction->setStatusTip(QStringLiteral("開啟影像檔案"));
    connect(OpenFileAction,SIGNAL(triggered()),this,SLOT(showOpenFile()));

    exitAction = new QAction(QStringLiteral("結束&Q"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(QStringLiteral("退出程式"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    zoomInAction = new QAction(QStringLiteral("放大"));
    zoomInAction->setShortcut(tr("Ctrl+]"));
    zoomInAction->setStatusTip(tr("放大影像"));
    connect(zoomInAction,SIGNAL(triggered()),this,SLOT(getZoomIn()));

    zoomOutAction = new QAction(QStringLiteral("縮小"));
    zoomOutAction->setShortcut(tr("Ctrl+["));
    zoomOutAction->setStatusTip(tr("縮小影像"));
    connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(getZoomOut()));
}
void ImageProcessor::createMenus(){
    fileMenu = menuBar()->addMenu(QStringLiteral("檔案&F"));
    fileMenu->addAction(OpenFileAction);
    fileMenu->addAction(exitAction);
    fileMenu = menuBar()->addMenu(QStringLiteral("工具&T"));
    fileMenu->addAction(zoomInAction);
    fileMenu->addAction(zoomOutAction);
}
void ImageProcessor::createToolbars(){
    fileTool = addToolBar("file");
    fileTool->addAction(OpenFileAction);
    fileTool = addToolBar("zoom");
    fileTool->addAction(zoomInAction);
    fileTool->addAction(zoomOutAction);

}
void ImageProcessor::loadFile(QString filename){
    qDebug()<<QString("file name:%1").arg(filename);
    QByteArray ba = filename.toLatin1();
    printf("FN:%s\n",(char *) ba.data());
    img.load(filename);
    imgWin->setPixmap(QPixmap::fromImage(img));
}
void ImageProcessor::showOpenFile(){
    filename = QFileDialog::getOpenFileName(this,tr("開啟影像"),
                                            tr("."),
                                            "bmp(*.bmp);;png(*.png)"
                                            ";;Jpeg(*.jpg)");
    if (!filename.isEmpty()) {
        if (img.isNull()) {
            loadFile(filename);
        }else{
            ImageProcessor *newIPWin = new ImageProcessor();
            newIPWin->show();
            newIPWin->loadFile(filename);
        }
    }
}
void ImageProcessor::getZoomIn(){
    QImage zoomedIn;
    zoomedIn = img.scaled(img.width()*2,img.height()*2);
    QLabel *ret = new QLabel();
    ret->setPixmap(QPixmap::fromImage(zoomedIn));
    ret->show();
}
void ImageProcessor::getZoomOut(){
    QImage zoomedOut;
    zoomedOut = img.scaled(img.width()/2,img.height()/2);
    QLabel *ret = new QLabel();
    ret->setPixmap(QPixmap::fromImage(zoomedOut));
    ret->show();
}
