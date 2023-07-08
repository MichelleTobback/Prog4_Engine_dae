#pragma once
#include <vector>
#include <functional>

namespace dae
{
    template<typename Signature>
    class Delegate;

    template<typename T, typename... Args>
    class Delegate<T(Args...)>
    {
    public:
        void operator+=(const std::function<T(Args...)>& func)
        {
            m_Functions.emplace_back(func);
        }

        void operator-=(const std::function<T(Args...)>& func)
        {
            auto it{ std::find_if(m_Functions.begin(), m_Functions.end(), [&func](const auto& f)
                {
                    return f.target_type() == func.target_type();
                }) };

            if (it != m_Functions.end())
            {
                m_Functions.erase(it);
            }
        }

        void Invoke(Args... args) const
        {
            for (const auto& func : m_Functions)
            {
                func(args...);
            }
        }

        void Clear();

    private:
        std::vector<std::function<T(Args...)>> m_Functions;
    };

    template<typename T, typename ...Args>
    inline void Delegate<T(Args...)>::Clear()
    {
        m_Functions.clear();
    }


    template<typename T>
    class ObservableType
    {
    public:
        using OnValueChangedDelegate = Delegate<void(T)>;
        ObservableType(T value = T{})
            : m_Value{ value }
            , m_pOnValueChangedDelegate{ std::make_unique<OnValueChangedDelegate>() }
        {

        }

        void operator+=(const T& right)
        {
            m_Value += right;
            m_pOnValueChangedDelegate->Invoke(m_Value);
        }
        void operator-=(const T& right)
        {
            m_Value -= right;
            m_pOnValueChangedDelegate->Invoke(m_Value);
        }
        void operator*=(const T& right)
        {
            m_Value *= right;
            m_pOnValueChangedDelegate->Invoke(m_Value);
        }
        void operator/=(const T& right)
        {
            m_Value /= right;
            m_pOnValueChangedDelegate->Invoke(m_Value);
        }
        void operator=(const T& right)
        {
            m_Value = right;
            m_pOnValueChangedDelegate->Invoke(m_Value);
        }

        const T& operator()() const { return m_Value; }

        OnValueChangedDelegate& GetOnValueChangedDelegate() { return *m_pOnValueChangedDelegate; }

    private:
        T m_Value{};
        std::unique_ptr<OnValueChangedDelegate> m_pOnValueChangedDelegate{};
    };
}
