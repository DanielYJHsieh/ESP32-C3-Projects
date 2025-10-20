# ESP32-C3 vs ESP8266 腳位對照表

## E-Paper 顯示器接線對照

| 功能 | ESP8266 (WeMos D1 Mini) | ESP32-C3 Super Mini | 說明 |
|------|------------------------|---------------------|------|
| **SPI SCLK** | GPIO14 (D5) | **GPIO2** | SPI 時鐘信號 |
| **SPI MOSI** | GPIO13 (D7) | **GPIO3** | SPI 資料輸出 |
| **SPI MISO** | GPIO12 (D6) | GPIO5 (不使用) | SPI 資料輸入 (E-Paper 不需要) |
| **CS** | GPIO15 (D8) | **GPIO10** | 片選信號 (需 3.3k 下拉) |
| **DC** | GPIO0 (D3) | **GPIO4** | 資料/命令選擇 |
| **RST** | GPIO5 (D1) | **GPIO5** | 復位信號 (可能需 1k 上拉) |
| **BUSY** | GPIO4 (D2) | **GPIO6** | 忙碌狀態檢測 |
| **VCC** | 3.3V | 3.3V | 電源 |
| **GND** | GND | GND | 接地 |

## ESP32-C3 Super Mini 完整腳位圖

```
                    ESP32-C3 Super Mini
                  ┌─────────────────────┐
              GND │ 1               21 │ GND
             3.3V │ 2               20 │ GPIO0 (ADC1_CH0) [BOOT]
           GPIO10 │ 3   [CS 推薦]    19 │ GPIO1 (ADC1_CH1)
           *GPIO8 │ 4   [內建LED]    18 │ GPIO2 (ADC1_CH2) [SCLK 推薦]
            GPIO9 │ 5   [Strapping] 17 │ GPIO3 (ADC1_CH3) [MOSI 推薦]
            GPIO7 │ 6               16 │ GPIO4 (ADC1_CH4) [DC 推薦]
            GPIO6 │ 7   [BUSY推薦]   15 │ GPIO5 (ADC2_CH0) [RST 推薦]
    (USB-) GPIO18 │ 8               14 │ 5V
    (USB+) GPIO19 │ 9               13 │ NC
         (RX) U0T │10               12 │ U0R (TX)
                  └─────────────────────┘
                          [USB-C]
```

\* GPIO8 已用於內建 LED (01_blink_led 專案)

## 推薦配置理由

### 為什麼選擇這些腳位？

1. **GPIO2 (SCLK)**: 
   - 支援高速 SPI
   - 不是 Strapping Pin
   - 可靠的數位輸出

2. **GPIO3 (MOSI)**:
   - 支援高速 SPI
   - 與 SCLK 相鄰，走線方便

3. **GPIO10 (CS)**:
   - 通用 GPIO
   - 遠離 Strapping Pins
   - 穩定可靠

4. **GPIO4 (DC)**:
   - 通用 GPIO
   - 適合快速切換

5. **GPIO5 (RST)**:
   - 通用 GPIO
   - 可安全用於復位控制

6. **GPIO6 (BUSY)**:
   - 通用 GPIO
   - 適合輸入檢測

## ⚠️ 避免使用的腳位

| GPIO | 原因 | 影響 |
|------|------|------|
| GPIO8 | 內建 LED | 已用於狀態指示 |
| GPIO9 | Strapping Pin | 影響開機模式 |
| GPIO18/19 | USB D-/D+ | USB-JTAG 使用中 |
| U0R/U0T | UART0 | Serial Monitor 使用 |

## 硬體連接示意圖

```
ESP32-C3 Super Mini          GDEQ0426T82 (4.26" E-Paper)
┌─────────────────┐          ┌──────────────────────┐
│                 │          │                      │
│  3.3V (Pin 2)  ├──────────┤ VCC                  │
│  GND  (Pin 1)  ├──────────┤ GND                  │
│                 │          │                      │
│  GPIO2 (SCLK)  ├──────────┤ CLK                  │
│  GPIO3 (MOSI)  ├──────────┤ DIN                  │
│                 │          │                      │
│  GPIO10 (CS)   ├─[3.3kΩ]─┤ CS  ──┬── [3.3kΩ]─GND│
│  GPIO4  (DC)   ├──────────┤ DC                   │
│  GPIO5  (RST)  ├─[1kΩ]───┤ RST ──┴── [1kΩ]─3.3V │
│  GPIO6  (BUSY) ├──────────┤ BUSY                 │
│                 │          │                      │
└─────────────────┘          └──────────────────────┘
```

注意: 電阻為可選，建議加上以提高穩定性

## 測試步驟

### 1. 基本連接測試
```cpp
// 測試各腳位是否正常輸出
pinMode(CS_PIN, OUTPUT);
pinMode(DC_PIN, OUTPUT);
pinMode(RST_PIN, OUTPUT);
pinMode(BUSY_PIN, INPUT);

// 切換測試
digitalWrite(CS_PIN, HIGH);
digitalWrite(DC_PIN, HIGH);
digitalWrite(RST_PIN, HIGH);
```

### 2. SPI 通訊測試
```cpp
SPI.begin(SCLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
// 發送測試資料
SPI.transfer(0x00);
SPI.endTransaction();
```

### 3. E-Paper 初始化測試
```cpp
display.init(115200);
display.setRotation(1);
display.fillScreen(GxEPD_WHITE);
display.display();
```

## 除錯建議

### 問題 1: 顯示器無反應
- 檢查電源供電是否穩定 (3.3V)
- 確認接線正確
- 測量 CS, DC, RST 腳位電壓

### 問題 2: SPI 通訊異常
- 降低 SPI 時鐘頻率 (從 4MHz 降到 1MHz)
- 使用邏輯分析儀檢查波形
- 確認 SCLK 和 MOSI 沒有接反

### 問題 3: 顯示異常或花屏
- 檢查 BUSY 信號是否正確讀取
- 確認顯示器型號與驅動匹配
- 增加電源濾波電容 (100nF + 10µF)

---

**版本**: 1.0  
**更新**: 2025-10-20  
**用途**: ESP32-C3 WiFi Display 移植專案
