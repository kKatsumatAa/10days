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

        // �N���X����string�Ŏ擾
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
        // ��`
        using nanoseconds = std::chrono::nanoseconds;
        using microseconds = std::chrono::microseconds;
        using milliseconds = std::chrono::milliseconds;
        using seconds = std::chrono::seconds;
        using minutes = std::chrono::minutes;
        using hours = std::chrono::hours;

        // �ÓI�֐�
        /// <summary>
        /// <para>Windows���N�����Ă���̌o�ߎ��Ԃ��~���b�P�ʂł���킵���l��Ԃ��B</para>
        /// <para>���������擾����킯�ł͂Ȃ����A���̊֐��̑��݈Ӌ`�͎��Ԍv���ɂ���B</para>
        /// <para>TimeUnit��std::chrono::duration�̕ʖ��ɂ���[���ԊԊu��\���^]</para>
        /// <para>�݂̂��g�p�ł���B</para>
        /// </summary>
        /// <typeparam name="TimeUnit"></typeparam>
        /// <returns></returns>
        template<class TimeUnit>
        static int32_t GetNowCount(void);
        static std::string GetNowDateTime(void);

        // �֐�
        void Start(float endTime = 1.0f);

        // int32_t�̍ő�l�̊֌W�ŁA596���Ԓ������ꎞ��~�ł��Ȃ��B
        void Pause(void); // �ꎞ��~
        void Resume(void); // �ĊJ

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
        // �ϐ�
        int32_t mil_startTime_; // �v���J�n����PC�����̎���
        int32_t mil_pauseTime_; // �ꎞ��~����PC�����̎���
        float mil_totalPuaseTime_; // ��~���Ă������Ԃ����v���b��
        float sec_endTime_; // �S�[�������b�ɂ��邩

        float spd_slow_{ 1.f }; // �v�����ԂɃX���[���[�V������K�p�ł���B
        int32_t mil_slowTime_;

    public:
        // setter
        void SetEndTime(float endTime) { sec_endTime_ = endTime; }

        // getter
        const float GetElapsedTime(void); // �o�ߎ��Ԏ擾
        const float GetEndTime(void) { return sec_endTime_; } // �ڕW���Ԃ����b��
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
        float timer_;		//�^�C�}�[
        float maxTimer_;	//�^�C�}�[�̍ő�l
        bool isTimeOut_;	//�^�C�}�[���ő�l�ɒB�������t���O

    public:
        NTimer() : timer_(0), maxTimer_(0), isTimeOut_(false) {}
        NTimer(const float maxTimer) : timer_(0), maxTimer_(maxTimer), isTimeOut_(false) {}

        //�^�C�}�[���Z�b�g
        void Reset();
        //�X�V(�^�C�}�[������Ă�)
        //elapseTimer:�o�ߎ��ԁB�^�C�}�[���Z�ʂɉe������B�X���[���[�V�����p
        void Update(const bool isRoop, const float elapseTimer = 1.0f);
        //�w�肵���l���^�C�}�[�����炵�Ă�
        void SubTimer(const float subTimer);

        //�Z�b�^�[
        void SetTimer(const float timer) { timer_ = timer; }
        void SetMaxTimer(const float maxTime_r) { maxTimer_ = maxTime_r; }
        void SetisTimeOut(bool isTimeOut) { isTimeOut_ = isTimeOut; }

        //�Q�b�^�[
        bool GetisTimeOut()const { return isTimeOut_; }
        const float GetMaxTimer()const { return (float)maxTimer_; }
        const float GetTimer()const { return (float)timer_; }
        //�^�C�}�[�̐i�݋���擾
        const float GetTimeRate()const { return (float)timer_ / (float)maxTimer_; }
        //�V�X�e�����J�n����Ă���̎��Ԃ��擾
        float GetNowTime(const TimeType& timeType);
    };
}