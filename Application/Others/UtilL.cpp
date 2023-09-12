#include "UtilL.h"
#include <chrono>
#include <cmath>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <imgui.h>

using namespace std::chrono;

template<class TimeUnit>
inline int32_t UtilL::Timer::GetNowCount(void)
{
    // template‚ÌŒ^‚Í[ŠÔŠÔŠu‚ğ‚ğ•\‚·Œ^]‚Å‚È‚¯‚ê‚Î‚È‚ç‚È‚¢B
    // ref: https://cpprefjp.github.io/reference/chrono/duration_aliases.html

    return (int32_t)duration_cast<TimeUnit>(steady_clock::now().time_since_epoch()).count();
}

std::string UtilL::Timer::GetNowDateTime(void)
{
    std::time_t now = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &now);

    std::stringstream ss;
    ss << 1900 + tm.tm_year << ":" << std::setfill('0') << std::setw(2) << tm.tm_mon + 1 << ":" << tm.tm_mday << " "
        << std::setfill('0') << std::setw(2) << tm.tm_hour << ":" << std::setfill('0') << std::setw(2) << tm.tm_min
        << ":" << std::setfill('0') << std::setw(2) << tm.tm_sec;
    return ss.str();
}

void UtilL::Timer::Start(float endTime)
{
    mil_startTime_ = GetNowCount<milliseconds>();
    sec_endTime_ = endTime;
}

void UtilL::Timer::Pause(void)
{
    mil_pauseTime_ = GetNowCount<milliseconds>();
}

void UtilL::Timer::Resume(void)
{
    // pauseTime_‚ª"0"‚È‚çreturnB
    if (mil_pauseTime_ == 0) return;

    // ’â~‚µ‚Ä‚¢‚½ŠÔ‚Ì‡Œv += ¡‚ÌŠÔ - ’â~‚µ‚½‚ÌŠÔ
    mil_totalPuaseTime_ += GetNowCount<milliseconds>() - mil_pauseTime_;
    // ’â~‚µ‚Ä‚¢‚½‚ÌŠÔ‚ğ‰Šú‰»
    mil_pauseTime_ = 0;
}

const float UtilL::Timer::GetElapsedTime(void)
{
    int32_t current = GetNowCount<milliseconds>();
    int32_t elapsed_pause{};
    int32_t elapsed_slow{};

    if (mil_pauseTime_) elapsed_pause = (current - mil_pauseTime_);
    if (mil_slowTime_) elapsed_slow = uint32_t((current - mil_slowTime_) * (1 - spd_slow_));

    return float{ (current - mil_startTime_ - mil_totalPuaseTime_ - elapsed_pause - elapsed_slow) / 1000.0f };
}

void UtilL::Timer::StartSlow(float spd)
{
    mil_slowTime_ = GetNowCount<milliseconds>();
    spd_slow_ = spd;
}

void UtilL::Timer::EndSlow(void)
{
    // pauseTime_‚ª"0"‚È‚çreturnB
    if (mil_slowTime_ == 0) return;

    // ’â~‚µ‚Ä‚¢‚½ŠÔ‚Ì‡Œv += ¡‚ÌŠÔ - ’â~‚µ‚½‚ÌŠÔ
    mil_totalPuaseTime_ += (GetNowCount<milliseconds>() - mil_slowTime_) * (1 - spd_slow_);
    // ’â~‚µ‚Ä‚¢‚½‚ÌŠÔ‚ğ‰Šú‰»
    mil_slowTime_ = 0;
    spd_slow_ = 1.f;
}

std::string UtilL::Convert::ToString(const std::wstring& wStr)
{
    int32_t necessarySize{ WideCharToMultiByte(CP_UTF8, 0, &wStr[0], (int32_t)wStr.size(), NULL, 0, NULL, NULL) };
    std::string toStr(necessarySize, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wStr[0], (int32_t)wStr.size(), &toStr[0], necessarySize, NULL, NULL);
    return toStr;
}

std::string UtilL::Convert::ToString(const char* cStr)
{
    int32_t necessarySize{ MultiByteToWideChar(CP_UTF8, 0, cStr, -1, NULL, 0) };
    std::wstring wStr(necessarySize, 0);
    MultiByteToWideChar(CP_UTF8, 0, cStr, -1, &wStr[0], necessarySize);
    int32_t utf8Size{ WideCharToMultiByte(CP_UTF8, 0, &wStr[0], -1, NULL, 0, NULL, NULL) };
    std::string toStr(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wStr[0], -1, &toStr[0], utf8Size, NULL, NULL);
    return toStr;
}


std::wstring UtilL::Convert::ToWString(const std::string& str)
{
    int32_t necessarySize{ MultiByteToWideChar(CP_UTF8, 0, &str[0], (int32_t)str.size(), NULL, 0) };
    std::wstring toWStr(necessarySize, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int32_t)str.size(), &toWStr[0], necessarySize);
    return toWStr;
}

std::wstring UtilL::Convert::ToWString(const char* cStr)
{
    int32_t necessarySize{ MultiByteToWideChar(CP_UTF8, 0, cStr, -1, NULL, 0) };
    std::wstring toWStr(necessarySize, 0);
    MultiByteToWideChar(CP_UTF8, 0, cStr, -1, &toWStr[0], necessarySize);
    return toWStr;
}

void UtilL::NTimer::Reset()
{
    timer_ = 0;
    isTimeOut_ = false;
}

void UtilL::NTimer::Update(const bool isRoop, const float elapseTimer)
{
    if (timer_ < maxTimer_)
    {
        timer_ += 1.0f * elapseTimer;
    }
    else
    {
        timer_ = maxTimer_;
        isTimeOut_ = true;
        if (isRoop == true)
        {
            Reset();
        }
    }
}

void UtilL::NTimer::SubTimer(const float subTimer)
{
    timer_ -= subTimer;
}

float UtilL::NTimer::GetNowTime(const TimeType& timeType)
{
    switch (timeType)
    {
    case TimeType::MilliSecond:
        return (float)GetTickCount64();
        break;
    case TimeType::Second:
        return (float)((GetTickCount64() / 1000) % 60);
        break;
    case TimeType::Minute:
        return (float)((GetTickCount64() / 60000) % 60);
        break;
    }

    return -1;
}
