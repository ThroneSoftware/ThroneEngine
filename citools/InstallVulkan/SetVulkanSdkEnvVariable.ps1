$vulkanSdkVar1 = "VULKAN_SDK"
$vulkanSdkVar2 = "VK_SDK_PATH"

$vulkanSdkVersion = Get-ChildItem -Path "C:\VulkanSdk\"

$vulkanSdkPath = "C:\VulkanSdk\$vulkanSdkVersion"

Write-Host "##vso[task.setvariable variable=$($vulkanSdkVar1)]$($vulkanSdkPath)"
Write-Host "##vso[task.setvariable variable=$($vulkanSdkVar2)]$($vulkanSdkPath)"

