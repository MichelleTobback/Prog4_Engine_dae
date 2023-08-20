#pragma once
#include <memory>
#include <stack>
#include <functional>

namespace dae
{
	template <typename T, typename ... TArgs>
	class StackedStateMachine final
	{
	public:
		StackedStateMachine() = default;
		~StackedStateMachine() = default;

		StackedStateMachine(const StackedStateMachine& other) = delete;
		StackedStateMachine(StackedStateMachine&& other) = delete;
		StackedStateMachine& operator=(const StackedStateMachine& other) = delete;
		StackedStateMachine& operator=(StackedStateMachine&& other) = delete;

		T& GetCurrent() const
		{
			return *m_pStates.top();
		}
		void Push(const std::shared_ptr<T>& pState, TArgs&& ... prevArgs, TArgs&& ... nextArgs)
		{
			if (m_pStates.size() > 0)
				m_pStates.top()->OnExit(std::forward<TArgs>(prevArgs)...);
			m_pStates.push(pState);
			m_pStates.top()->OnEnter(std::forward<TArgs>(nextArgs)...);
		}
		void Pop(TArgs&& ... prevArgs, TArgs&& ... nextArgs)
		{
			if (m_pStates.size() > 0)
			{
				m_pStates.top()->OnExit(std::forward<TArgs>(prevArgs)...);
				m_pStates.pop();
				if (m_pStates.size() > 0)
					m_pStates.top()->OnEnter(std::forward<TArgs>(nextArgs)...);
			}
		}
		void Pop(TArgs&& ... prevArgs, TArgs&& ... nextArgs, int amount)
		{
			PopWhile(std::forward<TArgs>(prevArgs)..., std::forward<TArgs>(nextArgs)..., [=, &amount](T*)
				{
					return amount--;
				});
		}
		void PopWhile(TArgs&& ... prevArgs, TArgs&& ... nextArgs, const std::function<bool(T* pState)>& fn)
		{
			if (m_pStates.size() > 0)
			{
				m_pStates.top()->OnExit(std::forward<TArgs>(prevArgs)...);
				while (fn(&GetCurrent()))
				{
					m_pStates.pop();
				}
				if (m_pStates.size() > 0)
					m_pStates.top()->OnEnter(std::forward<TArgs>(nextArgs)...);
			}
		}

	private:
		std::stack<std::shared_ptr<T>> m_pStates;
	};

}