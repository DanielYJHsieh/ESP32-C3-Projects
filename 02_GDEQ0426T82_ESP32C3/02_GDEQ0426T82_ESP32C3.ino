/*
 * GDEQ0426T82 + ESP32-C3 Super Mini 測試程式
 * 
 * 📊 移植重點：
 * ✅ 從 ESP8266 (WeMos D1 Mini) 移植到 ESP32-C3 Super Mini
 * ✅ 修改腳位定義以符合 ESP32-C3 可用 GPIO
 * ✅ 使用 WiFi.h (ESP32) 取代 ESP8266WiFi.h
 * ✅ 明確初始化 SPI 腳位
 * ✅ 記憶體優化：使用分頁模式 (800 bytes 緩衝)
 * ✅ ESP32-C3 擁有 400KB RAM (vs ESP8266 的 80KB)
 * 
 * 測試項目：
 * - 系統資訊顯示與記憶體統計
 * - 白屏清除測試
 * - 部分更新測試  
 * - 中央區域部分更新測試
 * 
 * 硬體連接：
 * - SCLK: GPIO2
 * - MOSI: GPIO3
 * - CS:   GPIO10
 * - DC:   GPIO4
 * - RST:  GPIO5
 * - BUSY: GPIO6
 * 
 * 版本: v1.0 (ESP32-C3 移植版)
 * 日期: 2025-10-20
 * 基於: GDEQ0426T82_WeMos_D1_Mini v2.1
 */

#include <GxEPD2_BW.h>
#include <gdeq/GxEPD2_426_GDEQ0426T82.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include <SPI.h>

// ==================== ESP32-C3 腳位定義 ====================
// SPI 腳位
#define EPD_SCLK   2   // SPI 時鐘
#define EPD_MOSI   3   // SPI 資料輸出
#define EPD_MISO   5   // SPI 資料輸入 (不使用但需定義)

// E-Paper 控制腳位
#define EPD_CS    10   // 片選 (建議加 3.3k 下拉電阻到 GND)
#define EPD_DC     4   // 資料/命令選擇
#define EPD_RST    5   // 復位 (建議加 1k 上拉電阻到 3.3V)  
#define EPD_BUSY   6   // 忙碌狀態檢測

// ==================== 記憶體最佳化顯示器設定 ====================
// 使用分頁模式：僅緩衝 8 行而非完整螢幕
// 優勢：減少 98% 的 RAM 使用 (從 48KB 降到 800 bytes)
// 功能：仍可顯示完整 800×480 像素 (透過自動分頁)
#define MAX_DISPLAY_BUFFER_SIZE 800  // 每次僅緩衝 8 行

#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? \
                         EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))

// 使用分頁模式的顯示器物件
GxEPD2_BW<GxEPD2_426_GDEQ0426T82, MAX_HEIGHT(GxEPD2_426_GDEQ0426T82)> display(
    GxEPD2_426_GDEQ0426T82(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

// ==================== 記憶體統計輔助函數 ====================
void printMemoryStats() {
  Serial.println(F("--- 記憶體狀態 ---"));
  Serial.print(F("可用堆疊記憶體: ")); 
  Serial.print(ESP.getFreeHeap()); 
  Serial.println(F(" bytes"));
  
  Serial.print(F("總堆疊記憶體: ")); 
  Serial.print(ESP.getHeapSize()); 
  Serial.println(F(" bytes"));
  
  Serial.print(F("已用記憶體: ")); 
  Serial.print(ESP.getHeapSize() - ESP.getFreeHeap()); 
  Serial.println(F(" bytes"));
  
  Serial.print(F("最小可用記憶體: ")); 
  Serial.print(ESP.getMinFreeHeap()); 
  Serial.println(F(" bytes"));
  
  // 計算實際緩衝大小
  unsigned long bufferSize = (unsigned long)(display.width() / 8) * display.height();
  Serial.print(F("顯示緩衝: ")); 
  Serial.print(bufferSize); 
  Serial.println(F(" bytes"));
  
  Serial.print(F("緩衝行數: ")); 
  Serial.print(display.height()); 
  Serial.println(F(" lines"));
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(3000);  // 延長等待時間，確保 Serial Monitor 連接
  
  Serial.println();
  Serial.println();
  Serial.println(F("=== GDEQ0426T82 + ESP32-C3 Super Mini 測試 ==="));
  Serial.printf("編譯時間: %s %s\r\n", __DATE__, __TIME__);
  Serial.println();
  
  // ==================== 記憶體最佳化：禁用 WiFi ====================
  WiFi.mode(WIFI_OFF);
  Serial.println(F("WiFi 已禁用以最佳化記憶體"));
  Serial.println();
  
  // 顯示系統資訊
  Serial.println(F("--- ESP32-C3 系統資訊 ---"));
  Serial.print(F("晶片型號: "));
  Serial.println(ESP.getChipModel());
  Serial.print(F("晶片版本: "));
  Serial.println(ESP.getChipRevision());
  Serial.print(F("CPU 頻率: "));
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(F(" MHz"));
  Serial.print(F("Flash 大小: "));
  Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
  Serial.println(F(" MB"));
  Serial.println();
  
  printMemoryStats();
  
  // ==================== 初始化 SPI ====================
  Serial.println(F("--- 初始化 SPI ---"));
  SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI, EPD_CS);
  Serial.println(F("SPI 初始化完成"));
  Serial.printf("SCLK: GPIO%d, MOSI: GPIO%d, CS: GPIO%d\r\n", EPD_SCLK, EPD_MOSI, EPD_CS);
  Serial.println();
  
  // 初始化控制腳位
  Serial.println(F("--- 初始化控制腳位 ---"));
  pinMode(EPD_BUSY, INPUT);
  pinMode(EPD_RST, OUTPUT);
  pinMode(EPD_DC, OUTPUT);
  pinMode(EPD_CS, OUTPUT);
  digitalWrite(EPD_CS, HIGH);
  digitalWrite(EPD_DC, HIGH);
  digitalWrite(EPD_RST, HIGH);
  Serial.println(F("控制腳位初始化完成"));
  Serial.printf("DC: GPIO%d, RST: GPIO%d, BUSY: GPIO%d\r\n", EPD_DC, EPD_RST, EPD_BUSY);
  Serial.println();
  
  // 初始化 GxEPD2 顯示器
  Serial.println(F("--- 初始化 GxEPD2 顯示器 ---"));
  display.init(115200, true, 2, false);
  Serial.println(F("[OK] GxEPD2 顯示器初始化成功！"));
  Serial.printf("顯示器尺寸: %d x %d 像素\r\n", display.width(), display.height());
  Serial.println();
  
  printMemoryStats();
  
  // 執行單次測試
  Serial.println(F("=== 開始單次測試 ==="));
  testClearScreen();
  testPartialUpdate();
  testPartialUpdateCenter();
  
  Serial.println();
  Serial.println(F("=== 測試完成 ==="));
  Serial.println(F("✅ 所有測試完成！進入閒置模式..."));
  printMemoryStats();
}

void loop() {
  // 測試已在 setup() 中完成，進入閒置狀態
  delay(10000);
}

void testClearScreen() {
  Serial.println(F("測試 1: 清除螢幕"));
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
  Serial.println(F("[OK] 白屏測試完成"));
  Serial.println();
}

void testPartialUpdate() {
  Serial.println(F("測試 2: 部分更新"));
  
  // 先建立完整背景
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(10, 50);
    display.print(F("ESP32-C3 Background"));
    display.drawRect(5, 5, display.width()-10, display.height()-10, GxEPD_BLACK);
  } while (display.nextPage());
  
  Serial.println(F("背景圖像完成，開始部分更新..."));
  delay(1000);
  
  // 部分更新：在左上角更新較大區域（保留邊距）
  int16_t x = 10;   // 距離左邊 10 像素
  int16_t y = 10;   // 距離上方 10 像素
  int16_t w = 400;  // 增加寬度 200 -> 400
  int16_t h = 120;  // 增加高度 50 -> 120
  
  display.setPartialWindow(x, y, w, h);
  display.setFont(&FreeMonoBold9pt7b);  // 設定字體
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.drawRect(0, 0, w, h, GxEPD_BLACK);
    display.drawRect(2, 2, w-4, h-4, GxEPD_BLACK);
    
    // 在 setPartialWindow 模式下，座標是相對的，從 (0,0) 開始
    display.setCursor(10, 30);   // 相對座標，字體有 baseline 所以調整 Y
    display.print(F("PARTIAL UPDATE"));
    display.setCursor(10, 55);
    display.print(F("ESP32-C3 Super Mini"));
    display.setCursor(10, 80);
    display.print(F("Area: 400x120 px"));
  } while (display.nextPage());
  display.setFont();  // 恢復預設字體
  
  Serial.println(F("[OK] 部分更新完成"));
  Serial.println();
}

void testPartialUpdateCenter() {
  Serial.println(F("測試 3: 中央部分更新"));
  
  // 先建立網格背景
  display.setRotation(0);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    for (int i = 50; i < display.width(); i += 50) {
      display.drawLine(i, 0, i, display.height(), GxEPD_BLACK);
    }
    for (int i = 50; i < display.height(); i += 50) {
      display.drawLine(0, i, display.width(), i, GxEPD_BLACK);
    }
  } while (display.nextPage());
  
  Serial.println(F("網格背景完成，開始中央部分更新..."));
  delay(1000);
  
  // 部分更新：在中央更新更大區域（螢幕中心對齊）
  int16_t w = 500;  // 增加寬度 300 -> 500
  int16_t h = 240;  // 增加高度 160 -> 240
  int16_t x = (display.width() - w) / 2;   // 水平置中：(800 - 500) / 2 = 150
  int16_t y = (display.height() - h) / 2;  // 垂直置中：(480 - 240) / 2 = 120
  
  // 除錯資訊：印出實際計算值
  Serial.println(F("--- 中央更新座標除錯 ---"));
  Serial.printf("display.width() = %d\r\n", display.width());
  Serial.printf("display.height() = %d\r\n", display.height());
  Serial.printf("更新區域: x=%d, y=%d, w=%d, h=%d\r\n", x, y, w, h);
  Serial.printf("計算公式: x=(%d-%d)/2=%d, y=(%d-%d)/2=%d\r\n", 
                display.width(), w, x, display.height(), h, y);
  Serial.println();
  
  display.setPartialWindow(x, y, w, h);
  display.setFont(&FreeMonoBold9pt7b);  // 設定字體
  
  // 更新前再次顯示座標資訊
  Serial.println(F("準備開始更新螢幕..."));
  Serial.printf("setPartialWindow(%d, %d, %d, %d)\r\n", x, y, w, h);
  
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.drawRect(0, 0, w, h, GxEPD_BLACK);
    display.drawRect(2, 2, w-4, h-4, GxEPD_BLACK);
    display.drawRect(10, 10, w-20, h-20, GxEPD_BLACK);
    
    // 在 setPartialWindow 模式下，座標是相對的，從 (0,0) 開始
    display.setCursor(30, 60);
    display.print(F("ESP32-C3 SUPER MINI"));
    display.setCursor(30, 100);
    display.print(F("CENTER UPDATE TEST"));
    display.setCursor(30, 140);
    display.print(F("GDEQ0426T82 4.26\""));
    display.setCursor(30, 180);
    display.print(F("Area: 500x240 pixels"));
  } while (display.nextPage());
  display.setFont();  // 恢復預設字體
  
  Serial.println(F("[OK] 中央部分更新完成"));
  Serial.println();
}
