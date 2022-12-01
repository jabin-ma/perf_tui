#include "cpu.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <iostream>

using namespace ftxui;

void createview() {
  auto screen = ScreenInteractive::Fullscreen();

  auto my_graph = [&](int width, int height) {
    std::vector<int> output(width);
    for (int i = 0; i < width; ++i) {
      output[i] = i % (width / 10) ? 0 : height;
    }
    return output;
  };

  auto htop = Renderer([&] {
    auto frequency = vbox({
            text("Frequency [Mhz]") | hcenter,
            hbox({
                    vbox({
                            text("2400 "),
                            filler(),
                            text("1200 "),
                            filler(),
                            text("0 "),
                    }),
                    graph(std::ref(my_graph)) | flex,
            }) | flex,
    });

    auto utilization = vbox({
            text("Utilization [%]") | hcenter,
            hbox({
                    vbox({
                            text("100 "),
                            filler(),
                            text("50 "),
                            filler(),
                            text("0 "),
                    }),
                    graph(std::ref(my_graph)) | color(Color::RedLight),
            }) | flex,
    });

    auto ram = vbox({
            text("Ram [Mo]") | hcenter,
            hbox({
                    vbox({
                            text("8192"),
                            filler(),
                            text("4096 "),
                            filler(),
                            text("0 "),
                    }),
                    graph(std::ref(my_graph)) | color(Color::BlueLight),
            }) | flex,
    });

    return hbox({
                   vbox({
                           frequency | flex,
                           separator(),
                           utilization | flex,
                   }) | flex,
                   separator(),
                   ram | flex,
           }) |
           flex;
  });

  int tab_index = 0;
  std::vector<std::string> tab_entries = {
          "freq",
  };
  auto tab_selection =
          Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
  auto tab_content = Container::Tab(
          {
                  htop,
          },
          &tab_index);
  auto main_container = Container::Vertical({
          tab_selection,
          tab_content,
  });

  auto main_renderer = Renderer(main_container, [&] {
    return vbox({
            text("PerfUI") | bold | hcenter,
            tab_selection->Render(),
            tab_content->Render() | flex,
    });
  });
  screen.Loop(main_renderer);
}


int main() {
  std::cout << "main" << std::endl;
  sysmonitor::Cpu cpu;
  for (const auto &item: cpu.clusters) {
    std::cout << item.name;

    std::cout << "CORE:";
    for (const auto &f: item.cores) {
      std::cout << " " << f;
    }
    std::cout << std::endl;

    std::cout << "FREQ:";
    for (const auto &f: item.freq) {
      std::cout << " " << f;
    }
    std::cout << std::endl;
  }
  return 0;
}
