#pragma once

#include <Component.hpp>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
namespace dae {
class Font;
class GameObject;
class TextDisplay;
class HighScoresDisplay : public Component {
public:
  HighScoresDisplay(GameObject &owner, std::string const &filePath,
                    std::shared_ptr<Font> font);
  void AddToHighscores(std::string const &name, int score);

private:
  std::vector<std::pair<std::string, int>> m_Scores{};
  std::string m_FilePath{};
  std::vector<TextDisplay *> m_ScoreTexts{};
  std::shared_ptr<Font> m_Font;
  void ReadScores();
  void CreateChildren();
  void UpdateChildren(int startIndex);
};
} // namespace dae
