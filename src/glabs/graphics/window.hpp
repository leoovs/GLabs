#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class Window
	{
	public:
		using CloseCallback = void();
		using SizeCallback = void(int32_t, int32_t);
		using MouseMoveCallback = void(float, float);

		Window() = default;
		Window(int32_t width, int32_t height, std::string_view title);

		Window(Window&& other) noexcept;

		Window& operator=(Window&& other) noexcept;

		~Window();

		int32_t GetWidth() const;
		int32_t GetHeight() const;
		GLFWwindow* GetNativeWindow() const;

		void SetCloseCallback(std::function<CloseCallback> callback);
		void SetSizeCallback(std::function<SizeCallback> callback);
		void SetMouseMoveCallback(std::function<MouseMoveCallback> callback);

		void PollEvents();
		void Present();

	private:
		static void BindThisToNativeWindow(Window* self, GLFWwindow* nativeWindow);
		static Window* GetThisFromNativeWindow(GLFWwindow* nativeWindow);

		void CreateNativeWindow(int32_t width, int32_t height, std::string_view title);
		void DestroyNativeWindow();

		void InitializeOglContext();
		void InitializeCallbacks();
		void RestoreThisInNativeWindow();

		GLFWwindow* mNativeWindow = nullptr;

		std::function<CloseCallback> mCloseCallback;
		std::function<SizeCallback> mSizeCallback;
		std::function<MouseMoveCallback> mMouseMoveCallback;
	};
}

