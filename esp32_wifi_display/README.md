# ESP32-C3 WiFi Display

## 專案簡介
將 ESP8266 的 WiFi E-Paper 顯示專案移植到 ESP32-C3 Super Mini 開發板。

## 硬體需求
- **ESP32-C3 Super Mini** 開發板
- **GDEQ0426T82** 4.26" E-Paper 顯示器 (800x480)
- 杜邦線若干
- (可選) 3.3kΩ 電阻 (CS 下拉)
- (可選) 1kΩ 電阻 (RST 上拉)

## 接線說明

| E-Paper | ESP32-C3 | 說明 |
|---------|----------|------|
| VCC | 3.3V | 電源 |
| GND | GND | 接地 |
| DIN | GPIO3 | MOSI |
| CLK | GPIO2 | SCLK |
| CS | GPIO10 | 片選 |
| DC | GPIO4 | 資料/命令 |
| RST | GPIO5 | 復位 |
| BUSY | GPIO6 | 忙碌檢測 |

## 功能特性
- ✅ WiFi 連接 (AP/STA 模式)
- ✅ WebSocket 影像傳輸
- ✅ UDP 自動發現 Server
- ✅ RLE 壓縮支援
- ✅ 800x480 全彩影像顯示
- ✅ 記憶體優化 (ESP32-C3 有 400KB RAM)

## 開發狀態
🚧 **目前狀態**: 移植規劃中

詳細資訊請參考: [ESP32C3_PORT_GUIDE.md](ESP32C3_PORT_GUIDE.md)

## 相關專案
- 原始 ESP8266 專案: `d:\1_myproject\epaper_display\epaper_display\wifi_spi_display\`
- GitHub: https://github.com/DanielYJHsieh/ESP32-C3-Projects

## 授權
與原始專案相同

---
**更新日期**: 2025-10-20
