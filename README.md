# Process 

## 项目简介

这是一个基于 Qt 开发的进程调度算法可视化模拟器，用于演示和理解操作系统中的进程调度机制。项目使用 C++17 和 Qt 框架实现，提供了图形化界面来展示进程的生命周期和调度过程。

## 技术栈

- **编程语言**: C++17
- **框架**: Qt (支持 Qt5 和 Qt6)
- **构建系统**: CMake (最低版本 3.16)
- **UI 设计**: Qt Designer (.ui 文件)

## 项目结构

```
Process/
├── icon/                       # 图标资源目录
│   ├── close.png              # 关闭按钮图标
│   ├── delete.png             # 删除按钮图标
│   ├── mini.png               # 最小化按钮图标
│   └── process.png            # 进程图标
├── .qrc                       # Qt 资源配置文件
├── CMakeLists.txt             # CMake 构建配置
├── main.cpp                   # 程序入口
├── mainwindow.h/.cpp/.ui      # 主窗口
├── List.h                     # 模板链表实现
├── Process.h/.cpp             # 进程类定义
├── ScheDuler.h/.cpp           # 调度器实现
├── ListSorter.h               # 链表排序工具
├── NodeItem.h/.cpp/.ui        # 进程列表项组件
├── TitleItem.h/.cpp/.ui       # 标题栏组件
└── AddProcessWidget.h/.cpp/.ui # 添加进程对话框
```

## 核心模块

### 1. 进程管理 (Process)

[Process](file://d:\qtcode\Process\Process.h#L10-L75) 类负责模拟操作系统中的进程实体，包含以下属性：

- **进程名** (`m_name`): 进程的唯一标识符
- **状态** (`m_state`): W(就绪)、R(运行)、F(完成)
- **优先级** (`m_priority`): 数值越大优先级越高
- **总时间** (`m_ntime`): 进程需要的总运行时间
- **已运行时间** (`m_rtime`): 进程已经执行的时间

主要方法：

- `setRunning()`: 将进程设置为运行状态
- `execute()`: 执行一次进程 (运行时间 +1，优先级 -1)
- `isFinish()`: 判断进程是否完成
- `updateState()`: 更新进程状态

### 2. 调度器 (Scheduler)

[Scheduler](file://d:\qtcode\Process\ScheDuler.h#L8-L27) 类是进程调度的核心，负责管理进程队列和执行调度逻辑：

- **进程链表** (`m_process`): 存储所有待调度的进程
- **调度次数** (`m_count`): 记录调度执行的次数

主要方法：

- `stepRun()`: 单步执行调度
- `getList()`: 获取进程链表

### 3. 数据结构 (List)

[List](file://d:\qtcode\Process\List.h#L12-L153) 是一个模板链表类，提供了基础的链表操作：

- 支持任意类型的模板参数
- 头插法插入节点
- 任意位置插入节点
- 删除头结点
- 清空链表
- 获取链表大小和头结点

### 4. 链表排序器 (ListSorter)

[ListSorter](file://d:\qtcode\Process\ListSorter.h#L7-L49) 提供静态方法用于按优先级插入进程：

- `insertByPriority()`: 根据优先级将进程插入到链表的合适位置
- 返回插入位置的索引

### 5. 用户界面组件

#### MainWindow (主窗口)

- 显示进程列表表格
- 提供调度控制按钮 (单步执行、自动执行、重置、关闭、最小化)
- 支持窗口拖拽移动
- 管理进程添加和删除

#### NodeItem (进程项)

- 显示单个进程的信息
- 提供删除按钮
- 显示进程名、优先级、运行时间等信息

#### AddProcessWidget (添加进程对话框)

- 输入进程信息 (名称、优先级、运行时间)
- 输入验证
- 发送确认/取消信号

#### TitleItem (标题项)

- 自定义标题栏组件

## 功能特性

1. **进程管理**
   - 添加新进程
   - 删除进程
   - 显示进程详细信息
2. **调度控制**
   - 单步执行：一次调度一个进程
   - 自动执行：连续执行调度过程
   - 重置：恢复初始状态
3. **可视化展示**
   - 进程状态实时显示
   - 优先级动态变化
   - 运行进度跟踪
4. **自定义窗口**
   - 无边框窗口设计
   - 支持拖拽移动
   - 自定义最小化/关闭按钮

## 构建说明

### 系统要求

- CMake >= 3.16
- C++17 兼容的编译器
- Qt5 或 Qt6 (Widgets 模块)

### 构建步骤

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 构建
cmake --build .

# 安装 (可选)
cmake --install .
```

### Qt 版本配置

项目自动检测 Qt 版本并适配：

- Qt6: 使用 `qt_add_executable` 和 `qt_finalize_executable`
- Qt5: 使用传统的 `add_executable`

## 使用说明

1. **启动程序**: 运行编译后的可执行文件
2. **添加进程**: 点击"添加进程"按钮，输入进程信息
3. **开始调度**:
   - 点击"单步执行"按钮逐步查看调度过程
   - 或点击"自动执行"按钮自动运行
4. **查看状态**: 在表格中查看各进程的状态变化
5. **重置**: 点击"重置"按钮重新开始

## 设计亮点

1. **手写链表**: 为了教学目的，手动实现了模板链表而非使用 STL
2. **优先级调度**: 实现了基于优先级的进程调度算法
3. **动态优先级**: 进程执行后优先级会降低，避免饥饿现象
4. **模块化设计**: UI 与业务逻辑分离，易于维护和扩展
5. **跨平台**: 支持 Windows、Linux、macOS

## 文件说明

### 头文件

- [List.h](file://d:\qtcode\Process\List.h): 模板链表实现
- [Process.h](file://d:\qtcode\Process\Process.h): 进程类定义
- [ScheDuler.h](file://d:\qtcode\Process\ScheDuler.h): 调度器定义
- [ListSorter.h](file://d:\qtcode\Process\ListSorter.h): 排序工具类
- [mainwindow.h](file://d:\qtcode\Process\mainwindow.h): 主窗口类
- [NodeItem.h](file://d:\qtcode\Process\NodeItem.h): 进程项组件
- [TitleItem.h](file://d:\qtcode\Process\TitleItem.h): 标题组件
- [AddProcessWidget.h](file://d:\qtcode\Process\AddProcessWidget.h): 添加进程对话框

### 实现文件

- [main.cpp](file://d:\qtcode\Process\main.cpp): 程序入口
- [Process.cpp](file://d:\qtcode\Process\Process.cpp): 进程类实现
- [ScheDuler.cpp](file://d:\qtcode\Process\ScheDuler.cpp): 调度器实现
- [mainwindow.cpp](file://d:\qtcode\Process\mainwindow.cpp): 主窗口实现
- [NodeItem.cpp](file://d:\qtcode\Process\NodeItem.cpp): 进程项实现
- [TitleItem.cpp](file://d:\qtcode\Process\TitleItem.cpp): 标题实现
- [AddProcessWidget.cpp](file://d:\qtcode\Process\AddProcessWidget.cpp): 添加对话框实现

### UI 文件

- [mainwindow.ui](file://d:\qtcode\Process\mainwindow.ui): 主窗口界面
- [NodeItem.ui](file://d:\qtcode\Process\NodeItem.ui): 进程项界面
- [TitleItem.ui](file://d:\qtcode\Process\TitleItem.ui): 标题界面
- [AddProcessWidget.ui](file://d:\qtcode\Process\AddProcessWidget.ui): 添加对话框界面

## 许可证

本项目为教学演示项目。
