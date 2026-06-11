#include "HighScoresDisplay.hpp"
#include <GameObject.hpp>
#include <ObjectRenderer.hpp>
#include <TextDisplay.hpp>
#include <algorithm>
#include <fstream>
using namespace dae;

dae::HighScoresDisplay::HighScoresDisplay(GameObject &owner,
                                          std::string const &filePath,
                                          std::shared_ptr<Font> font)
    : Component(owner), m_FilePath{filePath}, m_Font{font} {
  ReadScores();
  CreateChildren();
}

void dae::HighScoresDisplay::AddToHighscores(std::string const &initials,
                                             int score) {
  std::ofstream stream{m_FilePath, std::ios::app};
  stream << "\n" << initials << "," << score;

  auto insertPos =
      std::find_if(m_Scores.begin(), m_Scores.end(),
                   [score](const auto &entry) { return score > entry.second; });
  int index = static_cast<int>(insertPos - m_Scores.begin());
  m_Scores.insert(insertPos, {initials, score});

  UpdateChildren(index);
}

void dae::HighScoresDisplay::ReadScores() {
  std::ifstream stream{m_FilePath};
  std::string line;
  while (std::getline(stream, line)) {
    const auto comma = line.find(',');
    if (comma == std::string::npos)
      continue;

    std::string initials = line.substr(0, comma);
    int score = std::stoi(line.substr(comma + 1));
    m_Scores.emplace_back(std::move(initials), score);
  }

  std::sort(m_Scores.begin(), m_Scores.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });
}

void dae::HighScoresDisplay::CreateChildren() {
  auto go = std::make_unique<dae::GameObject>();
  go->AddComponent<dae::ObjectRenderer>();
  go->AddComponent<dae::TextDisplay>("Highscores:", m_Font,
                                     SDL_Color{255, 50, 100, 255});
  go->SetParent(GetOwner(), false);
  go.release();

  constexpr float scoreStartY = 70.f;
  constexpr float scoreSpacing = 40.f;
  size_t childrenCap{10};
  for (size_t i = 0; i < std::min(m_Scores.size(), childrenCap); ++i) {
    const auto &[initials, score] = m_Scores[i];
    std::string entry =
        initials + ": " +
        std::string(8 - std::min<size_t>(8, std::to_string(score).length()),
                    '0') +
        std::to_string(score);

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->AddComponent<dae::TextDisplay>(entry, m_Font,
                                       SDL_Color{255, 50, 100, 255});
    go->SetParent(GetOwner(), false);
    go->GetComponent<dae::Transform>()->SetLocalPosition(
        0, scoreStartY + static_cast<float>(i) * scoreSpacing);
    m_ScoreTexts.push_back(go->GetComponent<TextDisplay>());
    go.release();
  }
}

void dae::HighScoresDisplay::UpdateChildren(int startIndex) {
  constexpr float scoreStartY = 70.f;
  constexpr float scoreSpacing = 40.f;
  constexpr int childrenCap = 10;

  int endIndex = std::min((int)m_Scores.size(), childrenCap);

  for (int i = startIndex; i < endIndex; ++i) {
    const auto &[initials, score] = m_Scores[i];
    std::string entry =
        initials + ": " +
        std::string(8 - std::min<size_t>(8, std::to_string(score).length()),
                    '0') +
        std::to_string(score);

    if (i < (int)m_ScoreTexts.size()) {
      m_ScoreTexts[i]->SetText(entry);
    } else {
      auto go = std::make_unique<dae::GameObject>();
      go->AddComponent<dae::ObjectRenderer>();
      go->AddComponent<dae::TextDisplay>(entry, m_Font,
                                         SDL_Color{255, 50, 100, 255});
      go->SetParent(GetOwner(), false);
      go->GetComponent<dae::Transform>()->SetLocalPosition(
          0, scoreStartY + static_cast<float>(i) * scoreSpacing);
      m_ScoreTexts.push_back(go->GetComponent<TextDisplay>());
      go.release();
    }
  }
}
