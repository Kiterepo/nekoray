#include "ThemeManager.hpp"

#include <QStyleFactory>
#include <QFile>
#include <QTextStream>

ThemeManager *themeManager = new ThemeManager;

void ThemeManager::ApplyTheme(const QString &theme) {
    if (this->system_style_name.isEmpty()) {
        this->system_style_name = qApp->style()->objectName();
    }
    if (this->current_theme == theme) {
        return;
    }

    bool ok;
    auto themeId = theme.toInt(&ok);

    if (ok) {
        // System & Built-in
        QString qss;

        if (themeId != 0) {
            QString path;
            std::map<QString, QString> replace;
            switch (themeId) {
                case 1:
                    path = ":/themes/feiyangqingyun/qss/flatgray.css";
                    replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                    break;
                case 2:
                    path = ":/themes/feiyangqingyun/qss/lightblue.css";
                    replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                    break;
                case 3:
                    path = ":/themes/feiyangqingyun/qss/blacksoft.css";
                    replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                    break;
                default:
                    return;
            }

            QFile file(path);
            file.open(QFile::ReadOnly | QFile::Text);
            QTextStream stream(&file);
            qss = stream.readAll();
            for (auto const &[a, b]: replace) {
                qss = qss.replace(a, b);
            }
        }

        auto system_style = QStyleFactory::create(this->system_style_name);

        if (themeId == 0) {
            // system theme
            qApp->setPalette(system_style->standardPalette());
            qApp->setStyle(system_style);
            qApp->setStyleSheet("");
        } else {
            if (themeId == 1 || themeId == 2 || themeId == 3) {
                // feiyangqingyun theme
                QString paletteColor = qss.mid(20, 7);
                qApp->setPalette(QPalette(paletteColor));
            } else {
                // other theme
                qApp->setPalette(system_style->standardPalette());
            }
            qApp->setStyleSheet(qss);
        }
    } else {
        // QStyleFactory
        const auto &_style = QStyleFactory::create(theme);
        if (_style != nullptr) {
            qApp->setPalette(_style->standardPalette());
            qApp->setStyle(_style);
            qApp->setStyleSheet("");
        }
    }

    current_theme = theme;
}
