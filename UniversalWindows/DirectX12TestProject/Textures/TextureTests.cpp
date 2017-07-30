#include "pch.h"
#include "CppUnitTest.h"
#include "Core/DeviceResources.h"
#include "Core/Renderer.h"
#include "Core/Textures/Texture.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX;
using namespace DirectX12Engine;

namespace DirectX12TestProject
{
	TEST_CLASS(TextureTests)
	{
	public:
		TEST_METHOD(TextureTest1)
		{
			std::array<wchar_t, 512> filePath;
			GetModuleFileNameW(nullptr, filePath.data(), static_cast<DWORD>(filePath.size()));
			std::wstring f(filePath.data());
			std::wstring workingDirectory(L"../../../../UniversalWindows/WindowsApp/Working Directory/");

			auto deviceResources = std::make_shared<DeviceResources>();
			CommandListManager commandListManager(deviceResources);
			
			ID3D12GraphicsCommandList* commandList;
			commandListManager.CreateGraphicsCommandList(commandList);

			Texture texture;
			Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
			DirectX::DDS_ALPHA_MODE alphaMode;
			bool isCubeMap;
			texture.LoadTextureFromFile(
				deviceResources->GetD3DDevice(),
				commandList,
				L"sandstonecliff-albedo.dds",
				D3D12_RESOURCE_FLAG_NONE,
				DDS_LOADER_DEFAULT,
				alphaMode,
				isCubeMap,
				uploadBuffer
			);

			// Wait for upload:
			{
				commandList->Close();
				commandListManager.ExecuteCommandList(0);

				deviceResources->WaitForGpu();
			}
		}
	};
}