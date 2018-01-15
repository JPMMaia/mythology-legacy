#include "pch.h"
#include "VulkanApplication/Application.h"

#include <exception>
#include <iostream>

using namespace VulkanApplication;

int main()
{
	Application application;

	try
	{
		application.Run();
	}
	catch (const std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
