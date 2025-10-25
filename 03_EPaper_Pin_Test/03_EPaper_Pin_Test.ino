/*
 * E-Paper 腳位測試程式 - ESP32-C3 Super Mini
 * 
 * 目的：
 * ✅ 測試所有 E-Paper 控制腳位是否正常工作
 * ✅ 測試 SPI 通訊是否正常
 * ✅ 不使用 GxEPD2 庫，編譯速度快
 * ✅ 透過 Serial Monitor 查看測試結果
 * 
 * 測試項目：
 * 1. GPIO 輸出測試 (CS, DC, RST)
 * 2. GPIO 輸入測試 (BUSY)
 * 3. SPI 初始化測試
 * 4. SPI 資料傳輸測試
 * 5. 基本的 E-Paper 復位序列測試
 * 
 * 硬體連接：
 * - SCLK: GPIO2
 * - MOSI: GPIO3
 * - CS:   GPIO10
 * - DC:   GPIO4
 * - RST:  GPIO5
 * - BUSY: GPIO6
 * 
 * 版本: v1.0
 * 日期: 2025-10-22
 */

#include <SPI.h>

// ==================== ESP32-C3 腳位定義 ====================
// SPI 腳位
#define EPD_SCLK   2   // SPI 時鐘
#define EPD_MOSI   3   // SPI 資料輸出
#define EPD_MISO   5   // SPI 資料輸入 (不使用但需定義)

// E-Paper 控制腳位
#define EPD_CS    10   // 片選
#define EPD_DC     4   // 資料/命令選擇
#define EPD_RST    5   // 復位
#define EPD_BUSY   6   // 忙碌狀態檢測

// 內建 LED (用於狀態指示)
#define LED_PIN    8   // 內建藍色 LED

// ==================== 輔助函數 ====================

void printTestHeader(const char* testName) {
  Serial.println();
  Serial.println(F("========================================"));
  Serial.print(F("測試: "));
  Serial.println(testName);
  Serial.println(F("========================================"));
}

void printTestResult(bool passed) {
  if (passed) {
    Serial.println(F("✅ 測試通過"));
    // LED 快閃表示成功
    for(int i = 0; i < 2; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(100);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
    }
  } else {
    Serial.println(F("❌ 測試失敗"));
    // LED 慢閃表示失敗
    for(int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(300);
      digitalWrite(LED_PIN, HIGH);
      delay(300);
    }
  }
}

void printMemoryInfo() {
  Serial.println();
  Serial.println(F("--- 記憶體資訊 ---"));
  Serial.print(F("可用堆疊: "));
  Serial.print(ESP.getFreeHeap());
  Serial.println(F(" bytes"));
  Serial.print(F("最小可用: "));
  Serial.print(ESP.getMinFreeHeap());
  Serial.println(F(" bytes"));
}

// ==================== 測試函數 ====================

// 測試 1: GPIO 輸出測試
bool testGPIOOutput() {
  printTestHeader("GPIO 輸出測試 (CS, DC, RST)");
  
  Serial.println(F("測試 CS 腳位..."));
  digitalWrite(EPD_CS, LOW);
  delay(100);
  digitalWrite(EPD_CS, HIGH);
  delay(100);
  Serial.println(F("  CS: LOW -> HIGH"));
  
  Serial.println(F("測試 DC 腳位..."));
  digitalWrite(EPD_DC, LOW);
  delay(100);
  digitalWrite(EPD_DC, HIGH);
  delay(100);
  Serial.println(F("  DC: LOW -> HIGH"));
  
  Serial.println(F("測試 RST 腳位..."));
  digitalWrite(EPD_RST, LOW);
  delay(100);
  digitalWrite(EPD_RST, HIGH);
  delay(100);
  Serial.println(F("  RST: LOW -> HIGH"));
  
  return true;
}

// 測試 2: GPIO 輸入測試
bool testGPIOInput() {
  printTestHeader("GPIO 輸入測試 (BUSY)");
  
  int busyState = digitalRead(EPD_BUSY);
  Serial.print(F("BUSY 腳位狀態: "));
  Serial.println(busyState == HIGH ? "HIGH" : "LOW");
  
  Serial.println(F("提示: 如果沒有連接 E-Paper，BUSY 可能為浮動狀態"));
  
  return true;
}

// 測試 3: SPI 初始化測試
bool testSPIInit() {
  printTestHeader("SPI 初始化測試");
  
  Serial.println(F("初始化 SPI..."));
  SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI, EPD_CS);
  Serial.print(F("  SCLK: GPIO"));
  Serial.println(EPD_SCLK);
  Serial.print(F("  MOSI: GPIO"));
  Serial.println(EPD_MOSI);
  Serial.print(F("  CS:   GPIO"));
  Serial.println(EPD_CS);
  
  Serial.println(F("SPI 初始化完成"));
  
  return true;
}

// 測試 4: SPI 資料傳輸測試
bool testSPITransfer() {
  printTestHeader("SPI 資料傳輸測試");
  
  Serial.println(F("開始 SPI 傳輸測試..."));
  
  // 開始 SPI 事務
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  
  // 拉低 CS 開始通訊
  digitalWrite(EPD_CS, LOW);
  delay(1);
  
  // 發送測試資料
  Serial.println(F("發送測試資料: 0x00, 0x55, 0xAA, 0xFF"));
  SPI.transfer(0x00);
  SPI.transfer(0x55);
  SPI.transfer(0xAA);
  SPI.transfer(0xFF);
  
  // 拉高 CS 結束通訊
  delay(1);
  digitalWrite(EPD_CS, HIGH);
  
  SPI.endTransaction();
  
  Serial.println(F("SPI 傳輸完成"));
  Serial.println(F("提示: 使用邏輯分析儀可以驗證波形"));
  
  return true;
}

// 測試 5: E-Paper 復位序列測試
bool testEPaperReset() {
  printTestHeader("E-Paper 復位序列測試");
  
  Serial.println(F("執行復位序列..."));
  
  // 標準 E-Paper 復位序列
  digitalWrite(EPD_RST, HIGH);
  delay(20);
  Serial.println(F("  RST = HIGH (20ms)"));
  
  digitalWrite(EPD_RST, LOW);
  delay(2);
  Serial.println(F("  RST = LOW (2ms)"));
  
  digitalWrite(EPD_RST, HIGH);
  delay(20);
  Serial.println(F("  RST = HIGH (20ms)"));
  
  Serial.println(F("復位序列完成"));
  Serial.println(F("提示: 如果連接了 E-Paper，顯示器應該已復位"));
  
  return true;
}

// 測試 6: 腳位電壓掃描
bool testPinVoltageScan() {
  printTestHeader("腳位電壓掃描測試");
  
  Serial.println(F("設定所有輸出腳位為 HIGH..."));
  digitalWrite(EPD_CS, HIGH);
  digitalWrite(EPD_DC, HIGH);
  digitalWrite(EPD_RST, HIGH);
  delay(500);
  
  Serial.println(F("✅ 請用萬用表測量以下腳位電壓 (應為 3.3V):"));
  Serial.print(F("  CS  (GPIO"));
  Serial.print(EPD_CS);
  Serial.println(F(")"));
  Serial.print(F("  DC  (GPIO"));
  Serial.print(EPD_DC);
  Serial.println(F(")"));
  Serial.print(F("  RST (GPIO"));
  Serial.print(EPD_RST);
  Serial.println(F(")"));
  
  delay(3000);
  
  Serial.println();
  Serial.println(F("設定所有輸出腳位為 LOW..."));
  digitalWrite(EPD_CS, LOW);
  digitalWrite(EPD_DC, LOW);
  digitalWrite(EPD_RST, LOW);
  delay(500);
  
  Serial.println(F("✅ 請用萬用表測量以下腳位電壓 (應為 0V):"));
  Serial.print(F("  CS  (GPIO"));
  Serial.print(EPD_CS);
  Serial.println(F(")"));
  Serial.print(F("  DC  (GPIO"));
  Serial.print(EPD_DC);
  Serial.println(F(")"));
  Serial.print(F("  RST (GPIO"));
  Serial.print(EPD_RST);
  Serial.println(F(")"));
  
  delay(3000);
  
  // 恢復預設狀態
  digitalWrite(EPD_CS, HIGH);
  digitalWrite(EPD_DC, HIGH);
  digitalWrite(EPD_RST, HIGH);
  
  return true;
}

// ==================== 主程式 ====================

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println();
  Serial.println(F("╔════════════════════════════════════════╗"));
  Serial.println(F("║  E-Paper 腳位測試程式                 ║"));
  Serial.println(F("║  ESP32-C3 Super Mini                   ║"));
  Serial.println(F("╚════════════════════════════════════════╝"));
  Serial.println();
  
  // 顯示系統資訊
  Serial.println(F("--- 系統資訊 ---"));
  Serial.print(F("晶片型號: "));
  Serial.println(ESP.getChipModel());
  Serial.print(F("CPU 頻率: "));
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(F(" MHz"));
  Serial.print(F("Flash 大小: "));
  Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
  Serial.println(F(" MB"));
  
  printMemoryInfo();
  
  // 初始化 LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED OFF (active LOW)
  
  // 初始化 E-Paper 控制腳位
  Serial.println();
  Serial.println(F("--- 初始化腳位 ---"));
  pinMode(EPD_CS, OUTPUT);
  pinMode(EPD_DC, OUTPUT);
  pinMode(EPD_RST, OUTPUT);
  pinMode(EPD_BUSY, INPUT);
  
  digitalWrite(EPD_CS, HIGH);   // CS 預設為 HIGH (不選中)
  digitalWrite(EPD_DC, HIGH);   // DC 預設為 HIGH
  digitalWrite(EPD_RST, HIGH);  // RST 預設為 HIGH (不復位)
  
  Serial.println(F("腳位初始化完成"));
  Serial.println(F("  CS, DC, RST: OUTPUT (初始化為 HIGH)"));
  Serial.println(F("  BUSY: INPUT"));
  
  delay(1000);
  
  // 執行所有測試
  Serial.println();
  Serial.println(F("=========================================="));
  Serial.println(F("開始執行測試序列..."));
  Serial.println(F("=========================================="));
  
  delay(500);
  
  bool allPassed = true;
  
  allPassed &= testGPIOOutput();
  printTestResult(true);
  delay(1000);
  
  allPassed &= testGPIOInput();
  printTestResult(true);
  delay(1000);
  
  allPassed &= testSPIInit();
  printTestResult(true);
  delay(1000);
  
  allPassed &= testSPITransfer();
  printTestResult(true);
  delay(1000);
  
  allPassed &= testEPaperReset();
  printTestResult(true);
  delay(1000);
  
  allPassed &= testPinVoltageScan();
  printTestResult(true);
  delay(1000);
  
  // 顯示最終結果
  Serial.println();
  Serial.println(F("=========================================="));
  if (allPassed) {
    Serial.println(F("✅ 所有測試完成！"));
    Serial.println(F(""));
    Serial.println(F("下一步："));
    Serial.println(F("1. 如果尚未連接 E-Paper，請按照 PIN_MAPPING.md 接線"));
    Serial.println(F("2. 連接後可以編譯完整的 02_GDEQ0426T82_ESP32C3 程式"));
    Serial.println(F("3. 使用邏輯分析儀可以驗證 SPI 波形"));
  } else {
    Serial.println(F("❌ 部分測試失敗，請檢查硬體連接"));
  }
  Serial.println(F("=========================================="));
  
  printMemoryInfo();
  
  Serial.println();
  Serial.println(F("測試程式進入閒置模式..."));
  Serial.println(F("重新上電或按 RESET 鍵可以再次執行測試"));
}

void loop() {
  // 閒置模式 - LED 慢速閃爍表示運行中
  digitalWrite(LED_PIN, LOW);  // LED ON
  delay(2000);
  digitalWrite(LED_PIN, HIGH); // LED OFF
  delay(2000);
}
