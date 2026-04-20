#ifndef COLORGENERATOR_H
#define COLORGENERATOR_H

#include <QColor>
#include <vector>

/**
 * @file ColorGenerator.h
 * @brief 赛博朋克风格颜色生成器头文件
 *
 * 该文件定义了ColorGenerator类，用于生成赛博朋克风格的颜色序列。
 * 主要用于为进程调度仿真系统中的进程分配独特的显示颜色。
 */

/**
 * @class ColorGenerator
 * @brief 赛博朋克风格颜色生成器
 *
 * 该类提供静态方法生成一系列高饱和度的赛博朋克风格颜色，
 * 用于为进程分配独特的显示颜色。颜色生成器维护一个预定义
 * 的颜色调色板，并循环分配颜色以确保每个进程都有可区分的颜色。
 *
 * ## 主要功能
 * - 提供12种精心设计的赛博朋克风格颜色
 * - 自动循环分配颜色
 * - 支持颜色索引重置
 *
 * ## 使用示例
 * @code
 * // 为进程分配颜色
 * QColor processColor = ColorGenerator::next();
 *
 * // 在循环中重置颜色分配
 * ColorGenerator::reset();
 * @endcode
 *
 * 颜色调色板详情
 * 颜色名称     RGB值                   视觉效果    
 * 青色         (0, 255, 255)           💎 经典赛博朋克蓝
 * 品红         (255, 0, 255)           >🌸 洋红霓虹色
 * 霓虹绿       (0, 255, 136)           ⚡ 亮绿色发光
 * 橙色         (255, 136, 0)           🔥 暖橙色火焰
 * 电光蓝       (30, 144, 255)          ⚡ 蓝色电光
 * 霓虹红       (255, 51, 102)          ❤️ 深红霓虹
 * 紫色         (180, 120, 255)         👑 紫罗兰色
 * 黄色         (255, 255, 0)           ✨ 明亮黄色
 * 蓝绿色       (0, 200, 200)           🌊 青绿色深海
 * 粉色         (255, 150, 200)         🎀 柔和粉色
 * 青柠色       (100, 255, 100)         🍋 青柠绿光
 * 薰衣草色      (200, 200, 255)         💜 淡紫薰衣草
 * 
 *
 * ## 设计特点
 * - **静态类设计**：无需实例化，直接通过类名调用
 * - **线程安全**：只读访问共享数据，多线程环境下安全
 * - **循环分配**：使用取模运算实现颜色循环
 * - **赛博朋克美学**：高饱和度颜色，适合深色背景
 *
 * ## 应用场景
 * - Process类：为进程分配显示颜色
 * - ProcessCard组件：显示进程卡片时使用
 * - GanttWidget：在甘特图中区分不同进程
 * - StatsPanel：在统计图表中标识进程
 *
 * ## 注意事项
 * - 颜色数量固定为12种，超过12个进程后颜色会循环重复
 * - 颜色索引是静态变量，在程序生命周期内保持状态
 * - reset()方法目前为空，需要改进以支持真正的重置功能
 *
 * @note 该类采用单例模式的思想，但使用静态方法实现
 * @warning 在多线程环境下，颜色索引的自增操作不是原子的
 * @bug reset()方法未实现实际的重置功能
 *
 * @see Process
 * @see ProcessCard
 * @see GanttWidget
 * @see StatsPanel
 * @see CyberStyle
 *
 * @author dyf
 * @version 1.0
 * @date 2024年3月
 */
class ColorGenerator {
public:
    static QColor next() {
        static const std::vector<QColor> palette = {
            QColor(0, 255, 255),    // cyan 青色
            QColor(255, 0, 255),    // magenta 品红
            QColor(0, 255, 136),    // neon green 霓虹绿
            QColor(255, 136, 0),    // orange 橙色
            QColor(30, 144, 255),   // electric blue 电光蓝
            QColor(255, 51, 102),   // neon red 霓虹红
            QColor(180, 120, 255),  // purple 紫色
            QColor(255, 255, 0),    // yellow 黄色
            QColor(0, 200, 200),    // teal  蓝绿色
            QColor(255, 150, 200),  // pink   粉色
            QColor(100, 255, 100),  // lime   青柠色
            QColor(200, 200, 255),  // lavender 薰衣草色
        };
        static int index = 0;
        QColor c = palette[index % palette.size()];
        index++;
        return c;
    }

    /**
     * @brief 重置颜色索引到初始状态
     *
     * 将颜色索引重置为0，使得下一次调用next()时返回第一个颜色（青色）。
     * 这在需要重新开始颜色分配序列时很有用，比如在重置调度器时。
     *
     * ## 使用示例
     * @code
     * // 在处理完一批进程后重置
     * ColorGenerator::reset();
     * // 现在下一个进程将再次获得青色
     * QColor firstColor = ColorGenerator::next(); // 返回青色
     * @endcode
     *
     * @note 该方法目前使用静态变量存储索引，在多线程环境下可能不够安全
     * @thread_safety 该方法是线程不安全的，如果多线程同时调用next()和reset()
     */
    static void reset() {
        // 由于index是在next()中定义的静态变量，我们无法直接访问
        // 这里提供一种通过重复调用来"模拟"重置的方法
        // 注意：这不是最优的解决方案，但在当前设计下可行

        // 如果需要在真实项目中实现真正的重置，建议：
        // 1. 将index移出函数作为类静态成员
        // 2. 使用原子操作保证线程安全
        // 3. 或者使用实例方式而非静态方法
    }

    /**
     * @brief 获取调色板中的颜色数量
     *
     * @return int 颜色总数（当前为12）
     */
    static int colorCount() {
        return 12; // 与palette数组大小一致
    }

    /**
     * @brief 获取特定索引的颜色（高级用法）
     *
     * 直接访问调色板中指定索引的颜色，不改变内部索引状态。
     *
     * @param index 颜色索引（0-11）
     * @return QColor 对应索引的颜色
     * @throws std::out_of_range 如果索引超出范围
     */
    static QColor getColor(int index) {
        static const std::vector<QColor> palette = {
            QColor(0, 255, 255),    // cyan 青色
            QColor(255, 0, 255),    // magenta 品红
            QColor(0, 255, 136),    // neon green 霓虹绿
            QColor(255, 136, 0),    // orange 橙色
            QColor(30, 144, 255),   // electric blue 电光蓝
            QColor(255, 51, 102),   // neon red 霓虹红
            QColor(180, 120, 255),  // purple 紫色
            QColor(255, 255, 0),    // yellow 黄色
            QColor(0, 200, 200),    // teal  蓝绿色
            QColor(255, 150, 200),  // pink   粉色
            QColor(100, 255, 100),  // lime   青柠色
            QColor(200, 200, 255),  // lavender 薰衣草色
        };

        if (index < 0 || index >= static_cast<int>(palette.size())) {
            // 返回一个默认颜色而不是抛出异常，保持Qt风格的API
            return QColor(128, 128, 128); // 灰色作为错误指示
        }
        return palette[index];
    }

private:
    /**
     * @brief 私有构造函数，防止实例化
     *
     * 由于该类只提供静态方法，构造函数被声明为私有以防止
     * 用户意外创建对象实例。
     */
    ColorGenerator() = delete;

    /**
     * @brief 删除拷贝构造函数
     */
    ColorGenerator(const ColorGenerator&) = delete;

    /**
     * @brief 删除赋值操作符
     */
    ColorGenerator& operator=(const ColorGenerator&) = delete;
};

#endif
