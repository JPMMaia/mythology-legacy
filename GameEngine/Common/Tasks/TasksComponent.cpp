export module Common.Tasks.Components;

import Common.Tasks.Task;
import std.core;

namespace Common
{
	class TasksComponent
	{
	public:
		~TasksComponent()
		{
			// Join all threads before destroying:
			JoinAll();
		}

		void Add(Task&& task)
		{
			m_tasks.emplace_back(std::move(task));
		}

		void Update()
		{
			for (auto i = static_cast<int>(m_tasks.size()) - 1; i >= 0; --i)
			{
				auto& task = m_tasks[i];
				task.Update();
				if (task.IsReady())
					m_tasks.erase(m_tasks.begin() + i);
			}
		}

		void JoinAll() noexcept
		{
			while (!m_tasks.empty())
			{
				try
				{
					m_tasks.front().Join();
				}
				catch (const std::exception&)
				{
				}
				m_tasks.pop_front();
			}
		}

	private:
		std::deque<Task> m_tasks;
	};
}
