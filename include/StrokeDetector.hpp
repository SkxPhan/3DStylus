#ifndef STROKE_DETECTOR_H
#define STROKE_DETECTOR_H

#include <filesystem>
#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <string_view>
#include <utility>

#include "dollar.hpp"

using Record = std::pair<dollar::Stroke, std::string>;

class StrokeDetector {
  std::vector<Record> m_templateStrokes;

 public:
  StrokeDetector(std::string_view path);
  void addTemplate(dollar::Stroke templateStroke, std::string label);
  auto getNumberOfTemplate() { return m_templateStrokes.size(); }
  std::string recognize(const dollar::Stroke& sample);

 private:
  void loadTemplateStrokes(std::string_view path);
};

#endif  // STROKE_DETECTOR_H