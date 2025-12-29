# ImageProcessor

一個基於 Qt 的影像處理應用程式，支援矩形選取、2倍放大、繪圖編輯和儲存功能。

## 功能特色

### 🖼️ 基本功能
- 開啟影像檔案（支援 BMP、PNG、JPEG 格式）
- 影像縮放（放大/縮小）
- 多視窗顯示

### ✨ 新增功能

#### 1. 矩形選取 (Rectangular Selection)
- 使用滑鼠拖曳選取矩形區域
- 即時顯示紅色虛線選取框
- 最小選取尺寸：5x5 像素

![Selection Demo](docs/selection-demo.gif)

#### 2. 2倍放大視窗 (2x Zoom Window)
- 選取區域自動放大 2 倍
- 在新視窗中開啟
- 保持影像品質（使用平滑轉換）

#### 3. 繪圖工具 (Drawing Tools)
- 即時繪圖功能
- 可自訂筆刷顏色
- 可調整筆刷寬度（1-50 像素）
- 支援連續繪製

#### 4. 另存新檔 (Save As)
- 匯出編輯後的影像
- 支援多種格式：PNG、JPEG、BMP
- 提供儲存狀態回饋

## 快速開始

### 系統需求
- Qt 5.x 或更新版本
- C++17 編譯器
- qmake 建置工具

### 建置專案

```bash
# 產生 Makefile
qmake ImageProcessor.pro

# 編譯專案
make

# 執行程式
./ImageProcessor  # Linux/macOS
# 或
ImageProcessor.exe  # Windows
```

### Qt Creator
1. 開啟 `ImageProcessor.pro` 檔案
2. 設定建置套件（Kit）
3. 點擊「建置」按鈕
4. 點擊「執行」按鈕

## 使用說明

### 開啟影像
1. 點擊工具列的「開啟檔案」按鈕，或使用快捷鍵 `Ctrl+O`
2. 選擇影像檔案（支援 .bmp、.png、.jpg）

### 選取並編輯區域
1. 在影像上按住滑鼠左鍵並拖曳，畫出要選取的矩形
2. 放開滑鼠，選取區域會自動放大 2 倍並在新視窗開啟
3. 在新視窗中：
   - 按住滑鼠左鍵移動即可繪圖
   - 點擊「筆刷顏色」選擇顏色
   - 調整「筆刷寬度」改變粗細
4. 編輯完成後點擊「另存新檔」或按 `Ctrl+S` 儲存

### 鍵盤快捷鍵
- `Ctrl+O`: 開啟檔案
- `Ctrl+Q`: 結束程式
- `Ctrl+]`: 放大影像
- `Ctrl+[`: 縮小影像
- `Ctrl+S`: 另存新檔（在編輯視窗中）

## 專案結構

```
ImageProcessor/
├── main.cpp                    # 程式進入點
├── imageprocessor.h/cpp        # 主視窗類別
├── selectablelabel.h/cpp       # 可選取標籤類別
├── editorwindow.h/cpp          # 編輯視窗類別
├── ImageProcessor.pro          # Qt 專案檔
├── README.md                   # 本檔案
├── FEATURE_SUMMARY.md          # 功能詳細說明
└── ARCHITECTURE.md             # 架構文件
```

## 技術細節

### 核心類別

#### ImageProcessor
主視窗，負責：
- 檔案管理
- 選單和工具列
- 處理選取事件
- 座標映射

#### SelectableLabel
自訂 QLabel，提供：
- 滑鼠拖曳選取
- 視覺化選取框
- 選取完成信號

#### EditorWindow
編輯視窗，包含：
- 繪圖工具列
- 可繪圖畫布
- 儲存功能

#### DrawableLabel
自訂 QLabel，支援：
- 即時繪圖
- 筆刷自訂
- 畫布更新

### 關鍵技術
- **座標映射**: 將視窗座標映射到圖片座標
- **邊界檢查**: 確保所有操作在有效範圍內
- **信號槽機制**: Qt 的事件驅動架構
- **QPainter**: 高品質的 2D 繪圖

## 開發資訊

### 程式碼品質
- ✅ 通過 CodeQL 安全掃描
- ✅ 完整的邊界檢查
- ✅ 防止除以零錯誤
- ✅ 正確的記憶體管理

### 最佳實踐
- 使用命名常數而非魔術數字
- 適當的註解和文件
- 模組化設計
- 遵循 Qt 程式設計慣例

## 授權

本專案為教育用途開發。

## 作者

開發者：41343206

## 更新日誌

### v2.0 (2024)
- ✨ 新增矩形選取功能
- ✨ 新增 2 倍放大視窗
- ✨ 新增繪圖工具
- ✨ 新增另存新檔功能
- 🐛 修正座標映射問題
- 🐛 加強邊界檢查
- 📝 完整的中文文件

### v1.0
- 基本影像載入和顯示
- 縮放功能
- 多視窗支援

## 相關連結

- [Qt 官方文件](https://doc.qt.io/)
- [Qt QImage 文件](https://doc.qt.io/qt-5/qimage.html)
- [Qt QPainter 文件](https://doc.qt.io/qt-5/qpainter.html)

## 貢獻

歡迎提交問題回報和功能建議！

---

**注意**: 本專案需要 Qt 開發環境。請確保已安裝 Qt 5.x 或更新版本。
