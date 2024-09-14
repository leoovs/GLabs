#include "glabs/graphics/window.hpp"

namespace glabs
{
	Window::Window(int32_t width, int32_t height, std::string_view title)
	{
		CreateNativeWindow(width, height, title);
		InitializeOglContext();
		BindThisToNativeWindow(this, mNativeWindow);
		InitializeCallbacks();
	}

	Window::Window(Window&& other) noexcept
		: mNativeWindow(std::exchange(other.mNativeWindow, nullptr))
		, mCloseCallback(std::move(other.mCloseCallback))
		, mSizeCallback(std::move(other.mSizeCallback))
	{
		RestoreThisInNativeWindow();
	}

	Window& Window::operator=(Window&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeWindow();
		mNativeWindow = std::exchange(other.mNativeWindow, nullptr);
		mCloseCallback = std::move(other.mCloseCallback);
		mSizeCallback = std::move(other.mSizeCallback);
		RestoreThisInNativeWindow();

		return *this;
	}

	Window::~Window()
	{
		DestroyNativeWindow();
	}

	int32_t Window::GetWidth() const
	{
		assert(nullptr != mNativeWindow);
		int width = 0;
		glfwGetWindowSize(mNativeWindow, &width, nullptr);

		return static_cast<int32_t>(width);
	}

	int32_t Window::GetHeight() const
	{
		assert(nullptr != mNativeWindow);
		int height = 0;
		glfwGetWindowSize(mNativeWindow, nullptr, &height);

		return static_cast<int32_t>(height);
	}

	void Window::SetCloseCallback(std::function<CloseCallback> callback)
	{
		mCloseCallback = std::move(callback);
	}

	void Window::SetSizeCallback(std::function<SizeCallback> callback)
	{
		mSizeCallback = std::move(callback);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::Present()
	{
		glfwSwapBuffers(mNativeWindow);
	}

	void Window::BindThisToNativeWindow(Window* self, GLFWwindow* nativeWindow)
	{
		glfwSetWindowUserPointer(nativeWindow, static_cast<void*>(self));
	}

	Window* Window::GetThisFromNativeWindow(GLFWwindow* nativeWindow)
	{
		void* userPointer = glfwGetWindowUserPointer(nativeWindow);
		Window* self = static_cast<Window*>(userPointer);

		return self;
	}

	void Window::CreateNativeWindow(
		int32_t width,
		int32_t height,
		std::string_view title
	)
	{
		// Request the OpenGL v4.6 Core profile.
		// Sadly, compatibility profile is not supported by RenderDoc.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mNativeWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		assert(nullptr != mNativeWindow);
	}

	void Window::DestroyNativeWindow()
	{
		if (nullptr != mNativeWindow)
		{
			glfwDestroyWindow(std::exchange(mNativeWindow, nullptr));
		}
	}

	void Window::InitializeOglContext()
	{
		glfwMakeContextCurrent(mNativeWindow);
		GLenum result = glewInit();

		assert(GLEW_OK == result);
	}

	void Window::InitializeCallbacks()
	{
		glfwSetWindowCloseCallback(
			mNativeWindow,
			[](GLFWwindow* nativeWindow) -> void
			{
				Window* self = GetThisFromNativeWindow(nativeWindow);

				if (self->mCloseCallback)
				{
					self->mCloseCallback();
				}
			}
		);

		glfwSetWindowSizeCallback(
			mNativeWindow,
			[](GLFWwindow* nativeWindow, int width, int height) -> void
			{
				Window* self = GetThisFromNativeWindow(nativeWindow);

				if (self->mSizeCallback)
				{
					self->mSizeCallback(static_cast<int32_t>(width), static_cast<int32_t>(height));
				}
			}
		);
	}

	void Window::RestoreThisInNativeWindow()
	{
		BindThisToNativeWindow(this, mNativeWindow);
	}
}

