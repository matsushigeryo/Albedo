#include <Windows.h>
#include <DxLib.h>
#include "enemy.h"
#include "game.h"
#include "global.h"
#include "input.h"
#include "player_bullet.h"

static void Init() {
    is_running = true;
    render_buffer = MakeScreen(kGraphicWidth, kGraphicHeight, false);
    game::Init();
}

static void Update() {
    game::Update();
    if (game::g_game.input->OnPressedKeyboard(KEY_INPUT_ESCAPE)) {
        is_running = false;
    }
}

static void Draw() {
    SetDrawScreen(render_buffer);
    DrawBox(0, 0, kGraphicWidth, kGraphicHeight, GetColor(0, 0, 0), true);
    game::Draw();
    SetDrawScreen(DX_SCREEN_BACK);
    DrawExtendGraph(0, 0, kWindowWidth, kWindowHeight, render_buffer, false);

    int debug_line = 0;
    DrawFormatString(3, 3 + 16 * debug_line++, GetColor(255, 255, 255), "%.1fFPS", current_FPS);
    DrawFormatString(3, 3 + 16 * debug_line++, GetColor(255, 255, 255), "USING %.2fMB", GetWindowsMemoryUsage() / 1024.0f / 1024.0f);
    debug_line++;
    DrawFormatString(3, 3 + 16 * debug_line++, GetColor(255, 255, 255), "PLAYER BULLET COUNT: %d", game::g_game.player_bullet_manager->GetActiveCount());
    DrawFormatString(3, 3 + 16 * debug_line++, GetColor(255, 255, 255), "ENEMY COUNT: %d", game::g_game.enemy_manager->GetActiveCount());
    DrawFormatString(3, 3 + 16 * debug_line++, GetColor(255, 255, 255), "ENEMY BULLET COUNT: %d", game::g_game.enemy_bullet_manager->GetActiveCount());

    ScreenFlip();
}

static void Exit() {
    DeleteGraph(render_buffer);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

    // コンソール画面の立ち上げ
    AllocConsole();
    freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
    SetConsoleOutputCP(CP_UTF8);

    // DxLibの初期設定
    SetApplicationLogFileName("dxlib.log");
    ChangeWindowMode(true);
    SetWindowVisibleFlag(false);
    SetWindowStyleMode(7);
    SetGraphMode(kWindowWidth, kWindowHeight, 32);
    SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
    SetMainWindowText(kWindowTitle);
    SetUseDirectInputFlag(false);
    SetWaitVSyncFlag(false);
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    // DxLibの初期化
    if (DxLib_Init() == -1) {
        return -1;
    }

    // DirectX11が適応されているか確認
    if (GetUseDirect3DVersion() != DX_DIRECT3D_11) {
        MessageBoxA(GetMainWindowHandle(), "DirectX11に対応した環境で実行してください。", "エラー", MB_OK | MB_ICONERROR);
        DxLib_End();
        return -1;
    }

    long long current_process_time = GetNowHiPerformanceCount();
    long long previous_process_time = GetNowHiPerformanceCount();

    SetWindowVisibleFlag(true);

    Init();

    while (ProcessMessage() == 0 && is_running) {

        // 更新と描画
        Update();
        Draw();

        // フレームレート制御
        current_process_time = GetNowHiPerformanceCount();
        long long process_duration_time = current_process_time - previous_process_time;
        long long wait_time = kIdealFrameDuration - process_duration_time;

        if (wait_time > 0) {
            while (wait_time > 2000) {
                WaitTimer(1);
                current_process_time = GetNowHiPerformanceCount();
                wait_time = kIdealFrameDuration - (current_process_time - previous_process_time);
            }
            while (GetNowHiPerformanceCount() - previous_process_time < kIdealFrameDuration) {
            }
        }

        current_process_time = GetNowHiPerformanceCount();
        long long frameDurationTime = current_process_time - previous_process_time;
        current_FPS = 1000000.0f / static_cast<float>(frameDurationTime);
        previous_process_time = current_process_time;
    }

    // 終了
    Exit();
    DxLib_End();
    FreeConsole();

    return 0;
}