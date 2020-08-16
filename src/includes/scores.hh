/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// scores.hh: includes for scores.cc

#ifndef M_SCORES_HH
#define M_SCORES_HH

#include <cstdint>
#include "defs.hh"
#include "modes.hh"

constexpr int HIGHSCORE_COUNTS = 10;

struct ScoreEntry
{
    std::array<char, 3> name;
    char stage;
    std::uint32_t score;
};

void LoadHighScores();
int GetHighScoreCount(DifficultyLevel difficulty, PlaybackMode mode);
ScoreEntry GetHighScore(DifficultyLevel difficulty, PlaybackMode mode,
                                                    int rank);
// 0 if not high score, 1-10 for rank
int IsNewHighScore(DifficultyLevel difficulty, PlaybackMode mode,
                    std::uint32_t score, int stageNum);
void SubmitHighScore(DifficultyLevel difficulty, PlaybackMode mode,
                    ScoreEntry entry);
void SaveHighScores();

#endif // M_SCORES_HH
