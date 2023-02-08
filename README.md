# UP2_FW

My personal fork of the UP-Squared Firmware repository. For experimental usage.
Original from: https://github.com/tianocore-training/PlatformBuildLab_UP2_FW

## Noteworthy Additions & Modifications

* Added support for newer boards with Micron LPDDR4; should be able to boot boards with a green sticker on them. See this [Product Change Notice](https://up-board.org/pcn-up-squared-dram-emmc-bios/)
* Updated some build scripts to get around issues with having multiple Visual Studio's installed. You'll have to enter a VS Command Prompt environment manually. Defaults to Python2 now, assuming its install directory. If you have issues building, revert to the original build scripts.
