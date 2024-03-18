#include "StrokeDetector.hpp"

StrokeDetector::StrokeDetector(std::string_view path) {
  loadTemplateStrokes(path);
}

void StrokeDetector::loadTemplateStrokes(std::string_view path) {
  namespace fs = std::filesystem;
  const fs::path templatedata{path};

  for (const auto& l1 : fs::directory_iterator{templatedata}) {
    if (!l1.is_directory()) {
      continue;
    }
    for (const auto& l2 : fs::directory_iterator{l1}) {
      if (!l2.is_directory()) {
        continue;
      }
      for (const auto& f : fs::directory_iterator{l2}) {
        if (!f.is_regular_file()) {
          continue;
        }
        const std::string stem = f.path().stem().string();
        std::string_view label = stem;
        label.remove_suffix(2);
        pugi::xml_document doc;
        pugi::xml_parse_result result =
            doc.load_file(f.path().string().c_str());

        std::vector<dollar::Point> points;
        for (pugi::xml_node node : doc.child("Gesture").children("Point")) {
          points.push_back(
              {node.attribute("X").as_float(), node.attribute("Y").as_float()});
        }
        if (!points.empty()) {
          addTemplate(dollar::Stroke(points, dollar::Orientation::Sensitive),
                      std::string{label});
        }
      }
    }
  }
}

void StrokeDetector::addTemplate(dollar::Stroke templateStroke,
                                 std::string label) {
  m_templateStrokes.push_back({templateStroke, label});
}

std::string StrokeDetector::recognize(const dollar::Stroke& target) {
  float maxScore = 0;
  std::string label;
  for (const Record& record : m_templateStrokes) {
    const float d = dollar::detail::optimalCosineDistance(
        target.getVectorizedStroke(), record.first.getVectorizedStroke());
    const float score = 1.0f / d;
    if (score > maxScore) {
      maxScore = score;
      label = record.second;
    }
  }
  return label;
}
// @}
