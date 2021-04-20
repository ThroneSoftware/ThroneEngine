#include <Graphics/GraphicsInstance.h>

#include <Graphics/VulkanContextFactory.h>

int main()
{
	auto instance = trg::GraphicsInstance(std::make_unique<trg::GraphicsContext>(trg::VulkanContextFactory()));
	instance.present();
}