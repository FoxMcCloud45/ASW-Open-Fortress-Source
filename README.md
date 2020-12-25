# Alien Swarm Open Fortress Source

## Information

This is a port of the [Open Fortress code base reverse-engineered from Team Fortress 2 binaries](https://github.com/openfortress/Open-Fortress-Source-Base) from the Source 2013 Multiplayer engine branch to the Alien Swarm engine branch.  
The code here is based off [Valve's Alien Swarm SDK](https://store.steampowered.com/app/630)
(free for all Steam users) as well as Kosire's [Swarm SDK Template](https://github.com/FoxMcCloud45/swarm-sdk-template).  
The original code as well as some parts of the ported code are based on [Valve's Source SDK 2013 code](https://github.com/ValveSoftware/source-sdk-2013).

See the [README for the Open Fortress Source Base repository](https://github.com/openfortress/Open-Fortress-Source-Base/blob/master/README.md) for more information on the base Open Fortress project.  
The Open Fortress project has a [Discord server](https://discord.gg/Jk3NUb7) you can join for additional details on their work.  

I am **not** affiliated with the Open Fortress development team. I am just making this port as a hobbyist to have a version of Open Fortress that can benefit from various features exclusive to the Alien Swarm engine branch:

- [Alien Swarm](https://developer.valvesoftware.com/wiki/Alien_Swarm) on Valve Developer Community.
- [Alien Swarm (engine branch)](https://developer.valvesoftware.com/wiki/Alien_Swarm_(engine_branch)) on Valve Developer Community.
- [Left 4 Dead (engine branch)](https://developer.valvesoftware.com/wiki/Left_4_Dead_(engine_branch)) on Valve Developer Community.

Please read LICENSE.md for information about licensing. Please note that the Source 1 SDK license
initially made for the Source SDK 2013 also applies to the Alien Swarm SDK.

#### NOTE: This code is highly experimental!

The reverse-engineering project being a complex process, code from the Open Fortress Source Base is highly experimental and will most likely break in many ways as development goes on. While the goal of the Alien Swarm engine port is to make the Open Fortress Source Base functional on the Alien Swarm engine, most unimplemented features and bugs from the original codebase will most likely be ported over as well. Moreover, the port may carry many bugs the original project does not have.

Things that we are working on are marked with an `OFSTATUS` or `OFTODO` comment in the source code.

#### Pull requests are not currently accepted.

Although the nature of the Alien Swarm port doesn't necessarily require code to be proven as "original" (that is to say, not imported from projects like [NickNineTheEagle's TF2 code](https://github.com/NicknineTheEagle/TF2-Base) which use leaked code), I decided not to accept pull requests to keep the code clean of "dirty code" as well; by principle and also due to the original Open Fortress source code licensing.

However, you are free to open issues or fork the project to integrate your own fixes or features.

## Installing

**Note:** Unlike the Source SDK 2013 engine, the Alien Swarm engine is only available on Microsoft Windows. Mac OS X and GNU/Linux are not supported, **even for dedicated servers**. Running a dedicated server for Alien Swarm or any Alien Swarm mod (such as this project) requires the use of Microsoft Windows or [Wine](https://www.winehq.org/). [ReactOS](https://reactos.org/) remains untested but SteamCMD does not seem to be functional enough on it to download files from Steam servers.

### Requirements

You will need Visual Studio C++ 2010 to build the source code. The free Express edition is useable. Due to how hard it can be to find Visual Studio C++ 2010 Express nowadays, I'm providing a link to the [offline installer for the English Visual Studio C++ 2010 Express software in ISO format](https://debian.fmi.uni-sofia.bg/~aangelov/VS2010Express1.iso). However, you will have to activate it by yourself. Also, this link is not mine so it could go offline at any time, in which case I'll replace or remove the link but I'm not a provider for that software.

You should be able to use Visual Studio C++ 2012 as well to build the code, using [these instructions](https://developer.valvesoftware.com/wiki/Compiling_under_VS2012). The free Express edition should be compatible.

### Client and Server

These instructions apply for both a client or server installation.

**Note: Temporary. Will be made better soon, and a pre-made `build` folder will be made available soon enough.**

- ~~Download the game content.~~ (Game content is not yet available for this port. More work is needed [upstream](https://github.com/openfortress/Open-Fortress-Source-Base) beforehand. Right now, I use the `build` folder from the [Swarm SDK Template](https://github.com/FoxMcCloud45/swarm-sdk-template)) renamed to `of` for now.)
- Download the code (by cloning it via Git or by downloading the repository via GitHub's web interface). You will be able to download a release from the [Releases page](https://github.com/FoxMcCloud45/ASW-Open-Fortress-Source/releases) once somewhat-useable builds come through.
- Compile it using your Visual Studio C++ installation.
- You will get a `swarmsdktemplate` folder in your `sourcemods` directory. `sourcemods` is located in `Steam\steamapps`. Note that `sourcemods` only exists in Steam's installation directory, not in external game libraries. Source mods can only be installed in Steam's installation folder; if you want to use an external hard drive, you will need to use NTFS junctions (aka. symbolic links).
- Download the `build` folder from the [Swarm SDK Template](https://github.com/FoxMcCloud45/swarm-sdk-template)), by cloning the repository or by downloading it in ZIP format.
- Place the `build` folder in `sourcemods` and rename it `of`.
- Edit `of\gameinfo.txt` and replace "Swarm SDK Template" by "Open Fortress".
- Rename `resource\swarmsdktemplate_english.txt` to `of_english.txt`, edit it and replace "Swarm SDK Template" by "Open Fortress".
- Create `scripts\client_precache.txt` with:

```
precache
{
}
```

This is required, or the game will crash when loading a map.

- Copy `sourcemods\swarmsdktemplate\bin\*.dll` to `sourcemods\of\bin`. Replace existing DLLs.
- Restart Steam.

You should have the experimental Open Fortress in your Library. Right-click it, open `Properties`, and add the following launch options:

```
-console -override_vpk
```

Note: The `-override_vpk` will be required until proper releases with VPK files are made, which will probably only come later during development. It's also possible that development versions will not use VPKs (requiring the `-override_vpk` switch) and that said VPKs will be reserved to production builds.

The mod should be ready to run.