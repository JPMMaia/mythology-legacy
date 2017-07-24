#pragma once

namespace Common 
{
	class Timer;
}

namespace GraphicsEngine
{
	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

	public:

		/// <sumary>
		/// Updates application state when the window size changes (e.g. device orientation change)
		/// </summary>
		virtual void CreateDeviceDependentResources() = 0;

		/// <sumary>
		/// Updates application state when the window size changes (e.g. device orientation change)
		/// </summary>
		virtual void CreateWindowSizeDependentResources() = 0;

		/// <sumary>
		/// Saves the current state of the renderer.
		/// </summary>
		virtual void SaveState() = 0;

		/// <sumary>
		/// Restores the previous state of the renderer.
		/// </summary>
		virtual void LoadState() = 0;

		virtual void FrameUpdate(const Common::Timer& timer) = 0;

		virtual bool Render(const Common::Timer& timer) = 0;
	};
}
