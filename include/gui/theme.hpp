
#ifndef COLORS_HPP
#define COLORS_HPP

#include <SFML/Graphics.hpp>

namespace GUI {

struct Theme {
  sf::Color backgroundColor;
  sf::Color antColor;

  enum Type { polar, polarDark };
};

// Forward declaration
namespace Polar {
Theme makeTheme();
Theme makeThemeDark();
}  // namespace Polar

class ThemeManager {
 private:
  Theme m_theme{};

 public:
  ThemeManager() = default;

  // Getters
  [[nodiscard]] sf::Color backgroundColor() const {
    return m_theme.backgroundColor;
  }

  [[nodiscard]] sf::Color antColor() const { return m_theme.antColor; };

  // Helpers
  void applyTheme(Theme::Type type) {
    switch (type) {
      case Theme::Type::polar:
        m_theme = Polar::makeTheme();
        break;

      case Theme::Type::polarDark:
        m_theme = Polar::makeThemeDark();

      default:;
    }
  }
};

// Themes
namespace Polar {
struct Color {
  static inline sf::Color nord0{46, 52, 64};  // Base component of "Polar Night"
  static inline sf::Color nord1{59, 66, 82};  // Lighter shade
  static inline sf::Color nord2{67, 76, 94};  // Lighter shade
  static inline sf::Color nord3{76, 86, 106};     // Lighter shade
  static inline sf::Color nord4{216, 222, 233};   // "Snow Storm"
  static inline sf::Color nord5{229, 233, 240};   // Lighter shade
  static inline sf::Color nord6{236, 239, 244};   // Lighter shade
  static inline sf::Color nord7{143, 188, 187};   // Bluish core color
  static inline sf::Color nord8{136, 192, 208};   // Bluish core accent color
  static inline sf::Color nord9{129, 161, 193};   // Bluish core color
  static inline sf::Color nord10{94, 129, 172};   // Bluish core color
  static inline sf::Color nord11{191, 97, 106};   // Red color
  static inline sf::Color nord12{208, 134, 112};  // Orange color
  static inline sf::Color nord13{235, 203, 139};  // Yellowish color
  static inline sf::Color nord14{163, 190, 140};  // Greenish color
  static inline sf::Color nord15{180, 142, 173};  // Purple color
};

inline Theme makeTheme() {
  Theme t;
  t.backgroundColor = Polar::Color::nord4;
  t.antColor = Polar::Color::nord0;
  return t;
}

inline Theme makeThemeDark() {
  Theme t;
  t.backgroundColor = Polar::Color::nord0;
  t.antColor = Polar::Color::nord4;
  return t;
}

}  // namespace Polar
}  // namespace GUI

#endif  // COLORS_HPP
