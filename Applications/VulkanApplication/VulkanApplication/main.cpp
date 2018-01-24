#include "pch.h"
#include "VulkanApplication/Application.h"

using namespace VulkanApplication;

int main()
{
	try
	{
		Application application;
		application.Run();
	}
	catch (const std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
