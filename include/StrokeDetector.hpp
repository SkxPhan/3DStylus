#ifndef STROKE_DETECTOR_H
#define STROKE_DETECTOR_H

#include <string>

#include "dollar.hpp"

using Record = std::pair<dollar::Stroke, std::string>;

class StrokeDetector {
  std::vector<dollar::Point> m_buffer;
  std::vector<Record> m_templateStrokes;

 public:
  void loadTemplateStrokes(std::string_view path);
  void addTemplate(dollar::Stroke templateStroke, std::string label);
  auto getNumberOfTemplate() { return m_templateStrokes.size(); }
  void appendToBufferAndDetectStroke(dollar::Point);
  std::string recognize(const dollar::Stroke& sample);

 private:
  bool isMoving(const std::vector<std::pair<float, float>>& points,
                int nbrPreviousPoint = 20);
};

#endif  // STROKE_DETECTOR_H