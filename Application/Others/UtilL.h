#pragma once
#include <string>
#include <chrono>
#include "FileSystem.h"
#include <typeinfo>
#include <imgui.h>

namespace UtilL {
    enum Color
    {
        WHITE = 0xffffff,
        BLACK = 0x000000,
        RED = 0xff0000,
        BLUE = 0x0000ff,
        GREEN = 0x00ff00,
        YELLOW = 0xffff00,
    };

    namespace Convert {
        std::string ToString(const std::wstring& wStr);
        std::string ToString(const char* cStr);
        std::wstring ToWString(const std::string& str);
        std::wstring ToWString(const char* cStr);

        // クラス名をstringで取得
        template<class T> inline std::string NameOf(bool removePrefix = false) {
            std::string str{ typeid(T).name() };

            if (removePrefix) {
                if (str.size() < 7)
                    return str;

                str[0] == 'c' ? str.erase(0, 6) : str.erase(0, 7);
                return str;
            }

            return str;
        }

        template<class T, size_t N> std::array<T, N> ToArray(const std::vector<T>& vec) {
            return std::array<T, vec.size()>{vec.begin(), vec.end()};
        }

        template<class T, size_t N> std::vector<T> ToVector(const std::array<T, N>& arr) {
            return std::vector<T>{arr.begin(), arr.end()};
        }
    }

    class Timer {
    public:
        // 定義
        using nanoseconds = std::chrono::nanoseconds;
        using microseconds = std::chrono::microseconds;
        using milliseconds = std::chrono::milliseconds;
        using seconds = std::chrono::seconds;
        using minutes = std::chrono::minutes;
        using hours = std::chrono::hours;

        // 静的関数
        /// <summary>
        /// <para>Windowsが起動してからの経過時間をミリ秒単位であらわした値を返す。</para>
        /// <para>日時等を取得するわけではないが、この関数の存在意義は時間計測にある。</para>
        /// <para>TimeUnitはstd::chrono::durationの別名にある[時間間隔を表す型]</para>
        /// <para>のみを使用できる。</para>
        /// </summary>
        /// <typeparam name="TimeUnit"></typeparam>
        /// <returns></returns>
        template<class TimeUnit>
        static int32_t GetNowCount(void);
        static std::string GetNowDateTime(void);

        // 関数
        void Start(float endTime = 1.0f);

        // int32_tの最大値の関係で、596時間程しか一時停止できない。
        void Pause(void); // 一時停止
        void Resume(void); // 再開

        void StartSlow(float spd = 1.f);
        void EndSlow(void);
        
        void Draw4Imgui(void)
        {
            int32_t current = GetNowCount<milliseconds>();
            int32_t elapsed_pause{};
            int32_t elapsed_slow{};

            if (mil_pauseTime_) elapsed_pause = (current - mil_pauseTime_);
            if (mil_slowTime_) elapsed_slow = uint32_t((current - mil_slowTime_) * (1 - spd_slow_));


            ImGui::Begin("maru");
            ImGui::Text("current:%d", current);
            ImGui::Text("s      :%d", mil_startTime_);
            ImGui::Text("p      :%d", mil_pauseTime_);
            ImGui::Text("tp     :%f", mil_totalPuaseTime_);
            ImGui::Text("e      :%f", sec_endTime_);
            ImGui::Text("s_slow :%f", spd_slow_);
            ImGui::Text("m_slowT:%d", mil_slowTime_);
            ImGui::Text("e_pause:%d", elapsed_pause);
            ImGui::Text("e_slow :%d", elapsed_slow);
            ImGui::Text("return2:%f", float{ (current - mil_startTime_ - mil_totalPuaseTime_ - elapsed_pause - elapsed_slow) / 1000.0f });
            ImGui::End();
        }
    public:
        // 変数
        int32_t mil_startTime_; // 計測開始時のPC内部の時間
        int32_t mil_pauseTime_; // 一時停止時のPC内部の時間
        float mil_totalPuaseTime_; // 停止していた時間が合計何秒か
        float sec_endTime_; // ゴールを何秒にするか

        float spd_slow_{ 1.f }; // 計測時間にスローモーションを適用できる。
        int32_t mil_slowTime_;

    public:
        // setter
        void SetEndTime(float endTime) { sec_endTime_ = endTime; }

        // getter
        const float GetElapsedTime(void); // 経過時間取得
        const float GetEndTime(void) { return sec_endTime_; } // 目標時間が何秒か
        const bool GetIsEnd(void) { return sec_endTime_ < ((GetNowCount<milliseconds>() - mil_startTime_) / 1000.0f); } // endTime < elapsedTime
    };

    enum class TimeType
    {
        MilliSecond,
        Second,
        Minute
    };

    class NTimer final
    {
    private:
        float timer_;		//タイマー
        float maxTimer_;	//タイマーの最大値
        bool isTimeOut_;	//タイマーが最大値に達したかフラグ

    public:
        NTimer() : timer_(0), maxTimer_(0), isTimeOut_(false) {}
        NTimer(const float maxTimer) : timer_(0), maxTimer_(maxTimer), isTimeOut_(false) {}

        //タイマーリセット
        void Reset();
        //更新(タイマー足されてく)
        //elapseTimer:経過時間。タイマー加算量に影響する。スローモーション用
        void Update(const bool isRoop, const float elapseTimer = 1.0f);
        //指定した値分タイマーを減らしてく
        void SubTimer(const float subTimer);

        //セッター
        void SetTimer(const float timer) { timer_ = timer; }
        void SetMaxTimer(const float maxTime_r) { maxTimer_ = maxTime_r; }
        void SetisTimeOut(bool isTimeOut) { isTimeOut_ = isTimeOut; }

        //ゲッター
        bool GetisTimeOut()const { return isTimeOut_; }
        const float GetMaxTimer()const { return (float)maxTimer_; }
        const float GetTimer()const { return (float)timer_; }
        //タイマーの進み具合を取得
        const float GetTimeRate()const { return (float)timer_ / (float)maxTimer_; }
        //システムが開始されてからの時間を取得
        float GetNowTime(const TimeType& timeType);
    };
}