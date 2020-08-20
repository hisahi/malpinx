/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// scores.cc: code for handling high scores

#include <vector>
#include <array>
#include <fstream>
#include "scores.hh"
#include "formats.hh"

constexpr char scoreFileName[] = "malpinx.hsc";
constexpr int highScoreTableCount = (maxDifficultyLevel + 1) 
                                    * (maxPlaybackMode + 1);

static std::array<std::vector<ScoreEntry>, highScoreTableCount> tables;

static inline int getScoreTableIndex(
    DifficultyLevel difficulty, PlaybackMode mode)
{
    return static_cast<int>(difficulty) +
           static_cast<int>(mode) * (maxPlaybackMode + 1);
}

void LoadHighScores()
{
    std::ifstream file(scoreFileName, std::ios::binary);
    for (auto &v : tables) {
        v.clear();
    }
    if (!file.fail())
    {
        file.exceptions(std::ios::failbit | std::ios::badbit
                    | std::ios::eofbit);
        for (int i = 0; i < highScoreTableCount; ++i)
            tables[i] = std::move(LoadHighScoreTable(file));
    }
}

int GetHighScoreCount(DifficultyLevel difficulty, PlaybackMode mode)
{
    return tables[getScoreTableIndex(difficulty, mode)].size();
}

ScoreEntry GetHighScore(DifficultyLevel difficulty, PlaybackMode mode,
                                                    int rank)
{
    return tables[getScoreTableIndex(difficulty, mode)][rank];
}

// 0 if not high score, 1-10 for rank
int IsNewHighScore(DifficultyLevel difficulty, PlaybackMode mode,
                    std::uint32_t score, int stageNum)
{
    if (!score) return 0;
    int count = GetHighScoreCount(difficulty, mode);
    std::vector<ScoreEntry> &table = tables[
        getScoreTableIndex(difficulty, mode)];
    int i;

    for (i = 0; i < count; ++i)
    {
        if (score > table[i].score || (score == table[i].score
                                && stageNum > table[i].stage))
            return i + 1;
    }

    return count < HIGHSCORE_COUNTS ? count + 1 : 0;
}

void SubmitHighScore(DifficultyLevel difficulty, PlaybackMode mode,
                    ScoreEntry entry)
{
    int rank = IsNewHighScore(difficulty, mode, entry.score, entry.stage);
    if (!rank) return;
    
    std::vector<ScoreEntry> &table = tables[
        getScoreTableIndex(difficulty, mode)];
    table.insert(table.begin() + (rank - 1), entry);
    if (table.size() > HIGHSCORE_COUNTS)
        table.erase(table.begin() + HIGHSCORE_COUNTS, table.end());

    SaveHighScores();
}

void SaveHighScores()
{
    if (M_DEBUG) return;
    std::ofstream file(scoreFileName, std::ios::binary);
    if (!file.fail())
        for (int i = 0; i < highScoreTableCount; ++i)
            SaveHighScoreTable(file, tables[i]);
}
