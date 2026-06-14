# OSC509 Vibe Coding Fix

> 🎛️ 基于 OSC509 示波器套件的 AI 修复版固件，使用 Vibe Coding 方式编程改进。

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![MCU](https://img.shields.io/badge/MCU-STM32H750xx-orange.svg)]()
[![IDE](https://img.shields.io/badge/IDE-Keil%20MDK--ARM-green.svg)]()

---

## 📖 项目简介

本项目是基于 **OSC509 便携式示波器套件** 的固件修改版，采用 **AI（Vibe Coding）** 辅助编程方式进行问题修复与功能改进。固件运行于 **STM32H750xx** (ARM Cortex-M7) 平台，集成了自研的 FOS 实时操作系统、图形用户界面(GUI)、数字信号处理(DSP)、USB 通信、文件系统等核心模块。

## 🏗️ 项目结构
```
H750_OSC509_FIX/ 
├── osc509_V2.4/ # 主固件程序 (V2.4) 
│ ├── Oscilloscope/ 
│ │ ├── osc/ # 示波器核心逻辑（波形、测量、校准、菜单等） │ │ ├── gui/ # GUI 框架 
│ │ ├── fos/ # FOS 实时操作系统 
│ │ ├── dsp/ # 数字信号处理 (CMSIS-DSP) 
│ │ ├── dev/ # 设备驱动 (LCD, 触摸, SD卡, SDRAM 等) 
│ │ ├── layer/ # HAL 抽象层 
│ │ ├── hzs/ # 中文字库 
│ │ └── boot/ # 引导程序 
│ ├── Src/ # 用户源码 (main, 中断, USB 等)
│ ├── Inc/ # 头文件 
│ ├── Drivers/ # STM32H7xx HAL 驱动 & CMSIS 
│ ├── Middlewares/ # 中间件 (FatFs, USB Host/Device) 
│ ├── usb_lib/ # USB 应用层 
│ └── MDK-ARM/ # Keil MDK 工程文件 
├── OSC_BOOTLOADER_V1.2_256k/ # Bootloader (V1.2, 256KB) ├── HARDWARE/ # 硬件资料 (原理图, PCB, 数据手册) 
├── LICENSE # MIT 许可证 
└── README.md
```
## ✨ 主要特性

- **2通道示波器**：支持多通道波形采集、显示与测量
- **FOS 操作系统**：轻量级实时任务调度
- **GUI 框架**：高效图形渲染，支持中文字库
- **触发系统**：支持多种触发模式与校准
- **自动测量**：频率、周期、占空比、峰峰值等参数自动计算
- **DAC 信号发生器**：内置任意波形输出
- **USB 通信**：支持 USB Device (MSC) 与 USB Host (U盘读写)
- **SD 卡存储**：支持波形截图保存与固件升级
- **触摸屏交互**：GT9xx 电容触摸驱动
- **看门狗**：独立看门狗 (IWDG) 保障系统稳定

## 🔧 开发环境

| 项目 | 说明 |
|------|------|
| **硬件平台** | STM32H750xx (Cortex-M7, 480MHz) |
| **IDE** | Keil MDK-ARM V5 |
| **编译器** | ARM Compiler 5/6 (AC5/AC6) |
| **HAL 库** | STM32H7xx HAL Driver |
| **文件系统** | FatFs (Generic FAT) |
| **DSP 库** | CMSIS-DSP |

## 🚀 快速开始

1. 使用 **Keil MDK-ARM** 打开工程文件：

2. 编译工程，生成固件

3. 通过 **STM32CubeProgrammer** 或 **DFU** 方式烧录到 OSC509 硬件

4. 若需烧录 Bootloader，使用：

## 🧠 AI 修复说明 (Vibe Coding Fix)

本版本通过 AI 辅助编程（Vibe Coding）对原固件进行了如下改进：

- 🐛 修复了原版固件中存在的若干 Bug
- ⚡ 优化了部分代码逻辑与性能
- 🔒 增强了系统稳定性与安全性
- 🧹 清理了冗余代码与无用注释

## 📄 许可证

本项目基于 [MIT License](LICENSE) 开源。

Copyright © 2026 Nyawa🍥

## 📚 参考资料

硬件原理图与数据手册请参考 `HARDWARE/` 目录：

- `OSC509_示波器原理图.PDF` — 主板原理图
- `OSC-VGA.PDF` — VGA 扩展板
- `TCP-INCH43.PDF` / `TCP-INCH5.PDF` / `TCP-INCH7_1219.PDF` — 各尺寸屏幕资料

---

> 💡 *此项目由 Vibe Coding 驱动，AI 辅助编程，仅供学习与交流使用。*