# GDEQ0426T82 + ESP32-C3 Super Mini 測試專案

## 專案簡介
將 GDEQ0426T82 4.26" E-Paper 顯示器從 ESP8266 (WeMos D1 Mini) 移植到 ESP32-C3 Super Mini，用於驗證硬體接線和基本顯示功能。

## 硬體需求
- **ESP32-C3 Super Mini** 開發板
- **GDEQ0426T82** 4.26" E-Paper 顯示器 (800x480)
- 杜邦線若干
- (建議) 3.3kΩ 電阻 1 個 (CS 下拉)
- (建議) 1kΩ 電阻 1 個 (RST 上拉)

## 接線說明

### ESP32-C3 腳位配置

| E-Paper 模組 | ESP32-C3 GPIO | 說明 |
|-------------|---------------|------|
| VCC | 3.3V | 電源 (重要：必須 3.3V，不可 5V！) |
| GND | GND | 接地 |
| DIN (MOSI) | GPIO3 | SPI 資料輸出 |
| CLK (SCLK) | GPIO2 | SPI 時鐘 |
| CS | GPIO10 | 片選信號 (建議 3.3kΩ 下拉到 GND) |
| DC | GPIO4 | 資料/命令選擇 |
| RST | GPIO5 | 復位信號 (建議 1kΩ 上拉到 3.3V) |
| BUSY | GPIO6 | 忙碌狀態檢測 |

### 與 ESP8266 版本的腳位對照

| 功能 | ESP8266 (D1 Mini) | ESP32-C3 Super Mini |
|------|-------------------|---------------------|
| SPI SCLK | GPIO14 (D5) | GPIO2 |
| SPI MOSI | GPIO13 (D7) | GPIO3 |
| CS | GPIO15 (D8) | GPIO10 |
| DC | GPIO0 (D3) | GPIO4 |
| RST | GPIO5 (D1) | GPIO5 |
| BUSY | GPIO4 (D2) | GPIO6 |

## 編譯與上傳

### 安裝必要的庫
```bash
arduino-cli lib install "GxEPD2"
arduino-cli lib install "Adafruit GFX Library"
```

### 編譯
```bash
arduino-cli compile --fqbn esp32:esp32:esp32c3 02_GDEQ0426T82_ESP32C3
```

### 上傳到 ESP32-C3
```bash
arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32c3 02_GDEQ0426T82_ESP32C3
```

### 查看串口輸出
```bash
arduino-cli monitor -p COM4 -c baudrate=115200
```

## 測試項目

1. ✅ **白屏清除測試** - 全螢幕清除為白色
2. ✅ **部分更新測試** - 左上角小區域更新
3. ✅ **中央部分更新測試** - 中央大區域顯示文字

## 記憶體優化
- 使用分頁模式（Paged Mode）
- 緩衝區大小：800 bytes（僅 8 行）
- ESP32-C3 擁有 400KB RAM（vs ESP8266 的 80KB）

## 參考資源
- **原始專案**: `GDEQ0426T82_WeMos_D1_Mini`
- **GxEPD2 庫**: https://github.com/ZinggJM/GxEPD2
- **移植指南**: `../esp32_wifi_display/PIN_MAPPING.md`

## 版本歷史
- **v1.0** (2025-10-20) - 初始版本，從 ESP8266 移植

---
**作者**: Daniel YJ Hsieh  
**專案**: ESP32-C3-Projects
