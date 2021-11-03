#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <gui/theme.hpp>

TEST_CASE("Apply Polar Theme") {
  GUI::ThemeManager themeManager{};

  themeManager.applyTheme(GUI::Theme::polar);
  auto bg = themeManager.backgroundColor();
  auto ant = themeManager.antColor();

  CHECK((bg.r == 216 && bg.g == 222 && bg.b == 233));
  CHECK((ant.r == 46 && ant.g == 52 && ant.b == 64));
}

TEST_CASE("Apply Polar Dark Theme") {
  GUI::ThemeManager themeManager{};

  themeManager.applyTheme(GUI::Theme::polarDark);
  auto bg = themeManager.backgroundColor();
  auto ant = themeManager.antColor();

  CHECK((ant.r == 216 && ant.g == 222 && ant.b == 233));
  CHECK((bg.r == 46 && bg.g == 52 && bg.b == 64));
}