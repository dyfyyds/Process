#ifndef COLORGENERATOR_H
#define COLORGENERATOR_H

#include <QColor>
#include <vector>

class ColorGenerator {
public:
    static QColor next() {
        static const std::vector<QColor> palette = {
            QColor(0, 255, 255),    // cyan
            QColor(255, 0, 255),    // magenta
            QColor(0, 255, 136),    // neon green
            QColor(255, 136, 0),    // orange
            QColor(30, 144, 255),   // electric blue
            QColor(255, 51, 102),   // neon red
            QColor(180, 120, 255),  // purple
            QColor(255, 255, 0),    // yellow
            QColor(0, 200, 200),    // teal
            QColor(255, 150, 200),  // pink
            QColor(100, 255, 100),  // lime
            QColor(200, 200, 255),  // lavender
        };
        static int index = 0;
        QColor c = palette[index % palette.size()];
        index++;
        return c;
    }

    static void reset() {
        // 通过调用next()的static index无法重置，
        // 改用实例方式或接受循环
    }
};

#endif
