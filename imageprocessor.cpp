#include "imageprocessor.h"
#include "editorwindow.h"
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
    imgWin = new SelectableLabel();
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
    
    // Connect selection signal
    connect(imgWin, &SelectableLabel::selectionMade, this, &ImageProcessor::handleSelection);
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
    imgWin->setImage(img);
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

void ImageProcessor::handleSelection(QRect selection)
{
    if (img.isNull()) {
        return;
    }
    
    // Check for zero dimensions to prevent division by zero
    if (imgWin->width() == 0 || imgWin->height() == 0) {
        return;
    }
    
    // Get the pixmap to calculate actual displayed image dimensions
    QPixmap pix = imgWin->pixmap();
    if (pix.isNull()) {
        return;
    }
    
    // Calculate the scaling factor based on actual pixmap size
    qreal scaleX = (qreal)img.width() / pix.width();
    qreal scaleY = (qreal)img.height() / pix.height();
    
    // Map selection from widget coordinates to image coordinates
    QRect imageRect(
        selection.x() * scaleX,
        selection.y() * scaleY,
        selection.width() * scaleX,
        selection.height() * scaleY
    );
    
    // Ensure the rectangle is within image bounds
    imageRect = imageRect.intersected(img.rect());
    
    if (imageRect.width() > 0 && imageRect.height() > 0) {
        // Extract the selected region
        QImage selectedRegion = img.copy(imageRect);
        
        // Scale it by 2x
        QImage zoomedRegion = selectedRegion.scaled(
            selectedRegion.width() * 2,
            selectedRegion.height() * 2,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        
        // Open editor window with zoomed region
        EditorWindow *editor = new EditorWindow(zoomedRegion);
        editor->setAttribute(Qt::WA_DeleteOnClose);
        editor->show();
    }
}
