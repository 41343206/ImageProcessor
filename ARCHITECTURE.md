# ImageProcessor - 架構說明

## 系統架構

```
┌─────────────────────────────────────────────────────────────┐
│                      ImageProcessor                          │
│                    (Main Window)                             │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         SelectableLabel                               │   │
│  │  - 顯示圖片                                           │   │
│  │  - 處理滑鼠拖曳                                       │   │
│  │  - 繪製選取框                                         │   │
│  │  - 發送 selectionMade 信號                           │   │
│  └──────────────────────────────────────────────────────┘   │
│          │                                                    │
│          │ selectionMade(QRect)                              │
│          ▼                                                    │
│  handleSelection()                                           │
│   - 計算縮放比例                                             │
│   - 映射座標到圖片                                           │
│   - 擷取選取區域                                             │
│   - 放大 2 倍                                                │
│   - 建立編輯視窗                                             │
└──────────────────────────────────────────────────────────────┘
                      │
                      │ new EditorWindow(zoomedImage)
                      ▼
┌─────────────────────────────────────────────────────────────┐
│                     EditorWindow                             │
│                   (Editor Window)                            │
│  ┌──────────────────────────────────────────────────────┐   │
│  │   Toolbar                                             │   │
│  │   - 另存新檔 (Save As)                               │   │
│  │   - 筆刷顏色 (Pen Color)                             │   │
│  │   - 筆刷寬度 (Pen Width)                             │   │
│  └──────────────────────────────────────────────────────┘   │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         DrawableLabel                                 │   │
│  │  - 顯示放大的圖片                                     │   │
│  │  - 處理繪圖                                           │   │
│  │  - 即時更新畫布                                       │   │
│  └──────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

## 類別說明

### ImageProcessor (主視窗)
- **繼承**: QMainWindow
- **職責**: 
  - 應用程式主視窗
  - 管理選單和工具列
  - 載入和顯示圖片
  - 處理選取事件
- **成員變數**:
  - `SelectableLabel *imgWin`: 可選取的圖片標籤
  - `QImage img`: 目前載入的圖片
- **重要方法**:
  - `loadFile()`: 載入圖片檔案
  - `handleSelection()`: 處理矩形選取

### SelectableLabel (可選取標籤)
- **繼承**: QLabel
- **職責**:
  - 顯示圖片
  - 追蹤滑鼠拖曳動作
  - 繪製選取矩形
  - 發送選取完成信號
- **成員變數**:
  - `QPoint startPoint, endPoint`: 選取起點和終點
  - `bool selecting`: 是否正在選取
  - `QImage currentImage`: 目前的圖片
- **信號**:
  - `selectionMade(QRect)`: 選取完成時發送

### EditorWindow (編輯視窗)
- **繼承**: QMainWindow
- **職責**:
  - 提供圖片編輯介面
  - 管理繪圖工具
  - 處理儲存功能
- **成員變數**:
  - `DrawableLabel *drawableLabel`: 可繪圖的標籤
  - `QColor currentPenColor`: 目前筆刷顏色
  - `int currentPenWidth`: 目前筆刷寬度
- **重要方法**:
  - `saveAs()`: 另存新檔
  - `choosePenColor()`: 選擇筆刷顏色
  - `changePenWidth()`: 改變筆刷寬度

### DrawableLabel (可繪圖標籤)
- **繼承**: QLabel
- **職責**:
  - 顯示圖片
  - 處理繪圖動作
  - 更新畫布
- **成員變數**:
  - `QImage canvasImage`: 畫布圖片
  - `QPoint lastPoint`: 上一個繪圖點
  - `bool drawing`: 是否正在繪圖
- **重要方法**:
  - `drawLineTo()`: 繪製線條到指定點

## 事件流程

### 1. 圖片載入流程
```
User clicks "Open File"
        ↓
showOpenFile()
        ↓
loadFile(filename)
        ↓
img.load(filename)
        ↓
imgWin->setImage(img)
        ↓
Image displayed
```

### 2. 選取流程
```
User clicks and drags mouse
        ↓
mousePressEvent() → startPoint 設定
        ↓
mouseMoveEvent() → endPoint 更新, 重繪選取框
        ↓
mouseReleaseEvent() → 發送 selectionMade 信號
        ↓
handleSelection() 接收信號
        ↓
計算座標映射
        ↓
擷取圖片區域
        ↓
放大 2 倍
        ↓
建立 EditorWindow
        ↓
New window displayed
```

### 3. 繪圖流程
```
User clicks and drags in editor
        ↓
mousePressEvent() → 設定 lastPoint, drawing = true
        ↓
mouseMoveEvent() → drawLineTo(currentPoint)
        ↓
drawLineTo() → 在 canvasImage 上繪製線條
        ↓
更新 pixmap 顯示
        ↓
mouseReleaseEvent() → drawing = false
```

### 4. 儲存流程
```
User clicks "Save As"
        ↓
saveAs()
        ↓
QFileDialog::getSaveFileName()
        ↓
drawableLabel->getImage()
        ↓
image.save(fileName)
        ↓
顯示成功/失敗訊息
```

## 座標映射

選取框的座標需要從視窗座標映射到圖片座標：

```
Widget Coordinates → Image Coordinates

scaleX = imageWidth / pixmapWidth
scaleY = imageHeight / pixmapHeight

imageX = widgetX * scaleX
imageY = widgetY * scaleY
```

這確保了即使圖片在視窗中縮放顯示，選取的區域仍對應到正確的圖片位置。

## 邊界處理

### SelectableLabel
- 檢查滑鼠位置是否在視窗範圍內
- 確保選取框不超出視窗邊界

### DrawableLabel  
- 使用 `qBound()` 限制繪圖座標
- 確保所有繪圖操作在畫布範圍內

### handleSelection
- 檢查除數不為零
- 使用 `intersected()` 確保選取區域在圖片範圍內
