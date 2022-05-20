/*
 * @Author: crescendo
 * @Date: 2022-05-20 13:05:40
 * @Last Modified by: crescendo
 * @Last Modified time: 2022-05-20 13:06:27
 * 舞夜惊魂·改 挂机
 * https://github.com/Rottenham/ne-no-sun
 * AvZ版本：22-02-13
 * 部分代码参考自全麦的PE经典十二炮挂机脚本
 */

#include "avz.h"

using namespace AvZ;

using std::to_string;
TickRunner tick_runner_kill_QQ_16;
//获取指定波次刷出的僵尸总血量，用于刷新判断
int CurrentZombiesHP(int wave)
{
    auto zombies = GetMainObject()->zombieArray();
    int hp = 0;
    for (int index = 0; index < GetMainObject()->zombieTotal(); ++index)
    {
        if (zombies[index].isExist() && !zombies[index].isDisappeared() && !zombies[index].isDead() && zombies[index].existTime() == NowTime(wave))
        {
            hp += zombies[index].hp() + zombies[index].oneHp() + zombies[index].twoHp() / 5;
        }
    }
    return hp;
}
//获取指定波次刷出的僵尸的总血量最高的行数（不包括水路），用于激活位置判断
int RowExistHighestHPZombies(int wave)
{
    auto zombies = GetMainObject()->zombieArray();
    int row_have_highest_zombies_hp = 1;
    int highest_hp_of_all_the_row = 0;
    int hp_of_all_the_row[7];
    for (int row : {1, 2, 5, 6})
    {
        hp_of_all_the_row[row] = 0;
        for (int index = 0; index < GetMainObject()->zombieTotal(); ++index)
        {
            if (zombies[index].isExist() && !zombies[index].isDisappeared() && !zombies[index].isDead() && zombies[index].existTime() == NowTime(wave) && zombies[index].row() + 1 == row)
            {
                hp_of_all_the_row[row] += zombies[index].hp() + zombies[index].oneHp() + zombies[index].twoHp() / 5;
            }
        }
        if (hp_of_all_the_row[row] > highest_hp_of_all_the_row)
        {
            row_have_highest_zombies_hp = row;
        }
    }
    return row_have_highest_zombies_hp;
}

// 处理延迟
void notRefresh(int wave)
{
    if (CurrentZombiesHP(wave) - GetMainObject()->zombieRefreshHp() > 1750)
    {
        InsertGuard ig(true);
        SetTime(1202 - 200 - 373, wave);
        pao_operator.pao({{2, 9}, {4, 9}});
        SetTime(166 + 601, wave);
        Card(PUFF_SHROOM, 4, 9);
        Card(M_PUFF_SHROOM, 4, 9);
        SetTime(100, wave + 1);
        Shovel(4, 9);
    }
}

//主程序
void Script()
{
    std::vector<Grid> pao_list = {{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {1, 6}, {2, 6}, {3, 5}, {3, 7}, {4, 5}, {4, 7}, {5, 6}};
    // 跳帧运行，阵受损时停止
    auto condition = [=]()
    {
        auto plants = GetMainObject()->plantArray();
        for (each : pao_list)
        {
            if (plants[GetPlantIndex(each.row, each.col)].hp() < 300)
            {
                return false;
            }
        }
        return true;
    };
    auto callback = [=]()
    {
        SetGameSpeed(0.5);
        ShowErrorNotInQueue("发生阵损");
        LeftClick(740, 13);
    };
    // SkipTick(condition, callback);

    //游戏倍速
    SetGameSpeed(5);
    //调试窗口
    SetErrorMode(CONSOLE);
    //脚本循环生效
    OpenMultipleEffective('Q', AvZ::MAIN_UI_OR_FIGHT_UI);
    SelectCards({XPG_8, M_XPG_8, 40, 41, 42, 43, 44, 45, 46, 47});
    SetTime(-599, 1);
    //设定炮列表
    pao_operator.resetPaoList(pao_list);

    // PP P6常规操作
    for (int wave : {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19})
    {
        SetTime(303 - 373, wave);
        pao_operator.pao(2, 9);
        SetTime(166);
        Card(PUFF_SHROOM, 4, 9);
        Card(M_PUFF_SHROOM, 4, 9);
        SetTime(282);
        Shovel(4, 9);
        SetTime(359 - 373);
        pao_operator.pao(4, 9);
        if (wave != 9 && wave != 19)
        {
            InsertTimeOperation(1202 - 200 - 373, wave, [=]() { // 629
                if (GetRunningWave() == wave)
                {
                    notRefresh(wave);
                }
            });
        }
    }
    SetTime(100, 11);
    Shovel(3, 9);
    for (int wave : {9, 19})
    {
        for (int i = 1; i <= 3; i++)
        {
            SetTime(303 - 373 + 601 * i, wave);
            pao_operator.pao(2, 9);
            SetTime(166 + 601 * i);
            Card(PUFF_SHROOM, 4, 9);
            Card(M_PUFF_SHROOM, 4, 9);
            SetTime(282 + 601 * i);
            Shovel(4, 9);
            SetTime(359 - 373 + 601 * i);
            pao_operator.pao(4, 9);
        }
    }
    for (int wave : {20})
    {
        SetTime(310 - 373, wave);
        pao_operator.pao({{2, 9}, {4, 9}, {2, 9}, {4, 9}});
        SetTime(420 - 373);
        pao_operator.pao({{1, 9}, {4, 9}, {2, 9}, {4, 9}});
        SetTime(-30);
        pao_operator.pao({{2, 7}, {4, 7}});
        SetTime(1100 - 373);
        pao_operator.recoverPao({{2, 9}, {4, 9}});
    }
    SetTime(4300, 9);
    Card(PUFF_SHROOM, 2, 9);
    for (int wave : {10})
    {
        SetTime(410 - 373, wave);
        pao_operator.pao(2, 9);
        SetTime(466 - 373);
        pao_operator.pao(4, 8.5);
        SetTime(467);
        Card(PUFF_SHROOM, 3, 9);
        Card(M_PUFF_SHROOM, 3, 9);
        SetTime(280);
        Shovel(2, 9);
        InsertTimeOperation(1202 - 200 - 373, 10, [=]()
                            {
            if (GetRunningWave() == 10 && GetMainObject()->refreshCountdown() > 200)
                notRefresh(10); });
    }
}