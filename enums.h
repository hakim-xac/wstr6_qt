#ifndef ENUMS_H
#define ENUMS_H

namespace KHAS {

///
/// \brief The AppType enum
///
enum class AppType{
    Release, Debug
};

///
/// \brief The SelectBase enum
///
enum class SelectBase{
    General
    , Paths
    , Headers
};

enum class BattleType{
    Random
    , Workout
    , CompanyBattle
    , CombatTraining
    , TeamBattle
    , HistoryBattles
    , FanMode
    , Sortie
    , BattleForTheFortifiedArea
    , RankedBattle
    , PitchedBattle
    , FrontLine
    , Tournament
    , GK
    , Unknown
};

enum class FieldNames{
    ActiveColumn
    , TypeSortColumns
    , CurrentPathIndex
    , CountOfPaths
    , WaitUpdateStatusBar_s
};


}

#endif // ENUMS_H
