# ESP32-C3 WiFi Display 移植開發文件

## 專案概述
將 ESP8266 的 wifi_spi_display 專案移植到 ESP32-C3 Super Mini 開發板。

## 硬件對比

### ESP8266 (WeMos D1 Mini)
原始專案使用以下腳位：
- **CS (Chip Select)**: GPIO15 (D8) - 需要 3.3k 下拉電阻到 GND
- **DC (Data/Command)**: GPIO0 (D3)
- **RST (Reset)**: GPIO5 (D1) - 可能需要 1k 上拉電阻到 3.3V
- **BUSY**: GPIO4 (D2)
- **SPI 腳位**:
  - SCLK: GPIO14 (D5)
  - MOSI: GPIO13 (D7)
  - MISO: GPIO12 (D6) - 不使用

### ESP32-C3 Super Mini
可用的 GPIO 腳位與建議配置：

#### SPI 默認腳位 (HSPI)
ESP32-C3 的 SPI 可以使用任意 GPIO，但建議使用以下腳位：
- **SCLK**: GPIO6 或 GPIO2
- **MOSI**: GPIO7 或 GPIO3
- **MISO**: GPIO5 或 GPIO4 (不使用，但列出供參考)

#### E-Paper 控制腳位建議
根據 ESP32-C3 Super Mini 的可用腳位：

**方案 A (推薦 - 避免 Strapping Pins)**
```
CS   -> GPIO10
DC   -> GPIO4
RST  -> GPIO5
BUSY -> GPIO6
SCLK -> GPIO2
MOSI -> GPIO3
```

**方案 B (替代方案)**
```
CS   -> GPIO7
DC   -> GPIO6
RST  -> GPIO5
BUSY -> GPIO4
SCLK -> GPIO2
MOSI -> GPIO3
```

**方案 C (使用標準 SPI 腳位)**
```
CS   -> GPIO7
DC   -> GPIO10
RST  -> GPIO4
BUSY -> GPIO5
SCLK -> GPIO6  (標準 SCLK)
MOSI -> GPIO7  (標準 MOSI)
```

### ESP32-C3 腳位注意事項

#### ⚠️ 避免使用的腳位
1. **GPIO8**: 內建藍色 LED (已用於 LED 指示)
2. **GPIO9**: Strapping Pin (影響開機模式)
3. **GPIO18, GPIO19**: USB D-/D+ (已被 USB-JTAG 使用)
4. **GPIO20, GPIO21**: UART RX/TX (Serial Monitor 使用)

#### ✅ 推薦使用的腳位
- **GPIO2-7, GPIO10**: 通用 GPIO，適合 SPI 和控制信號
- **GPIO0, GPIO1**: 可用但要注意是否會影響下載模式

## 硬件連接表

### 推薦接線 (方案 A)

| E-Paper 模組 | ESP32-C3 Super Mini | 說明 |
|-------------|---------------------|------|
| VCC | 3.3V | 電源 |
| GND | GND | 接地 |
| DIN (MOSI) | GPIO3 | SPI 資料輸出 |
| CLK (SCLK) | GPIO2 | SPI 時鐘 |
| CS | GPIO10 | 片選信號 (需要 3.3k 下拉) |
| DC | GPIO4 | 資料/命令選擇 |
| RST | GPIO5 | 復位信號 (可能需要 1k 上拉) |
| BUSY | GPIO6 | 忙碌狀態檢測 |

### 電阻需求
根據原始 ESP8266 專案：
- **CS 腳位**: 建議加 3.3kΩ 下拉電阻到 GND
- **RST 腳位**: 可能需要 1kΩ 上拉電阻到 3.3V

## 軟件移植要點

### 1. WiFi 庫差異
```cpp
// ESP8266
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

// ESP32-C3 (需要修改)
#include <WiFi.h>
#include <WebSocketsClient.h>  // 使用相同的 WebSocket 庫
```

### 2. SPI 配置差異
```cpp
// ESP8266 - 自動使用硬體 SPI
// SCK=GPIO14, MOSI=GPIO13

// ESP32-C3 - 需要明確初始化
#include <SPI.h>
SPIClass hspi(HSPI);  // 或直接使用 SPI

void setup() {
  // 初始化自定義 SPI 腳位
  SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  // 或使用 hspi.begin(SCLK, MISO, MOSI, CS);
}
```

### 3. GxEPD2 庫兼容性
GxEPD2 庫支援 ESP32 系列，包括 ESP32-C3：
- 相同的顯示器型號: `GxEPD2_426_GDEQ0426T82`
- 相同的控制器: SSD1677
- 解析度: 800x480 像素

### 4. 記憶體差異
ESP32-C3 記憶體優勢：
- **RAM**: 約 400KB (vs ESP8266 的 80KB)
- **Flash**: 4MB
- **優點**: 
  - 可以使用完整畫面緩衝 (48KB)
  - 不需要過度依賴分塊模式
  - 更好的 WebSocket 效能

## 開發步驟

### 第一階段：基礎移植
1. ✅ 建立專案結構
2. ⬜ 修改腳位定義
3. ⬜ 移植 WiFi 連接代碼
4. ⬜ 測試 E-Paper 顯示基本功能

### 第二階段：功能實現
1. ⬜ 移植 WebSocket 通訊
2. ⬜ 移植 UDP 自動發現
3. ⬜ 移植 RLE 解壓縮
4. ⬜ 實現影像顯示

### 第三階段：優化
1. ⬜ 記憶體優化 (利用 ESP32-C3 較大 RAM)
2. ⬜ 效能測試
3. ⬜ 穩定性測試

## 檔案結構

```
esp32_wifi_display/
├── ESP32C3_PORT_GUIDE.md          (本文件)
├── esp32_wifi_display.ino         (主程式)
├── config.h                        (配置檔)
├── protocol.h                      (通訊協議)
├── rle_decoder.h                   (RLE 解壓縮)
├── GxEPD2_display_selection.h     (顯示器配置)
└── README.md                       (使用說明)
```

## 相容性檢查清單

- [ ] WiFi 連接功能
- [ ] WebSocket 通訊
- [ ] UDP 自動發現
- [ ] SPI 通訊正常
- [ ] E-Paper 顯示正常
- [ ] RLE 解壓縮正常
- [ ] 記憶體管理穩定
- [ ] 長時間運行穩定

## 參考資源

### ESP32-C3 官方文件
- [ESP32-C3 技術參考手冊](https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf)
- [ESP32-C3 資料表](https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf)

### 使用的庫
- **GxEPD2**: https://github.com/ZinggJM/GxEPD2
- **WebSocketsClient**: https://github.com/Links2004/arduinoWebSockets
- **WiFi (ESP32)**: Arduino ESP32 內建

### 原始專案
- 位置: `d:\1_myproject\epaper_display\epaper_display\wifi_spi_display\`
- 文件: client_esp8266/

## 測試計劃

### 硬體測試
1. **SPI 通訊測試**: 使用邏輯分析儀檢查 SPI 信號
2. **E-Paper 初始化**: 測試基本的顯示器初始化
3. **WiFi 連接**: 測試 AP/STA 模式

### 軟體測試
1. **記憶體壓力測試**: 確認沒有記憶體洩漏
2. **長時間運行測試**: 24 小時穩定性測試
3. **影像傳輸測試**: 不同大小和格式的影像

## 已知問題與解決方案

### 問題 1: SPI 速度
- **ESP8266**: 最高 40MHz
- **ESP32-C3**: 最高 80MHz
- **解決**: 可能需要調整 SPI 時鐘速度以確保穩定性

### 問題 2: WebSocket 庫版本
- 確保使用最新版本的 WebSocketsClient 庫
- 測試與 ESP32 的兼容性

## 下一步

1. 建立基礎程式檔案
2. 實現硬體腳位配置
3. 測試基本的 SPI 通訊
4. 移植核心功能

---

**建立日期**: 2025-10-20  
**作者**: Daniel YJ Hsieh  
**版本**: 0.1.0  
**狀態**: 規劃中
