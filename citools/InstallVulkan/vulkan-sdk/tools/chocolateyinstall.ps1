$ErrorActionPreference = 'Stop';
$toolsDir   = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"
$url        = "https://sdk.lunarg.com/sdk/download/$($env:ChocolateyPackageVersion)/windows/VulkanSDK-$($env:ChocolateyPackageVersion)-Installer.exe"

$packageArgs = @{
  packageName   = $env:ChocolateyPackageName
  unzipLocation = $toolsDir
  fileType      = 'exe'
  url           = $url

  softwareName  = 'vulkan-sdk*'
  checksum      = '7086ca300c3995caf5b5dc1c3ef50251fa95b650ee1bf22c9f5f52b94b228d38'
  checksumType  = 'sha256'

  silentArgs    = "/S /qn /norestart /l*v `"$($env:TEMP)\$($packageName).$($env:chocolateyPackageVersion).MsiInstall.log`""
  validExitCodes= @(0, 3010, 1641)
}

Install-ChocolateyPackage @packageArgs