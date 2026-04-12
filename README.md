WinMTR (Redux)
==============
**WinMTR (Redux)** is an extended fork of [Appnor's WinMTR](http://winmtr.net/) ([sourceforge](http://sourceforge.net/projects/winmtr/)) <br>
with IPv6 support, CLI/live-report support, ASN lookup, native Windows dark-mode support, and other enhancements and bug fixes.

### Download (binaries)
* [**view all available**](https://github.com/White-Tiger/WinMTR/releases)

#### Differences to [WinMTR](http://winmtr.net/) 0.98
- `[x]` - removed Windows 2000 support <br>
- `[x]` + added IPv6 support <br>
- `[x]` + added live CLI mode with ASN lookup support <br>
- `[x]` + added IPv4/IPv6 CLI selection with `-4` and `-6` <br>
- `[x]` + added system-following dark mode for the GUI <br>
- `[x]` + clickable entries when stopped.. *(why the heck wasn't it possible before?)* <br>
- `[x]` * added start delay of about 30ms for each hop *(870ms before the 30th hop gets queried) <br>
this should improve performance and reduces network load* <br>
- `[x]` ! fixed trace list freeze *(list didn't update while tracing, happens when tracing just one hop)* <br>
- `[x]` * theme support *(more fancy look :P)* <br>
- ~~`[ ]` + remembers window size~~ <br>
- `[ ]` ! CTRL+A works for host input <br>
- `[ ]` + host history: pressing del key or right mouse will remove selected entry <br>
- `[ ]` * new icon <br>

### Requirements
* Windows 7 or newer
* Microsoft Visual Studio with C++ and MFC support for local builds
* A modern Windows SDK and C++ toolset

### CLI usage
WinMTR defaults to CLI mode when you launch it from the command line with a target host. Use `--gui` if you want to force the desktop window instead.

```text
WinMTR.exe bgp.tools
WinMTR.exe bgp.tools -4
WinMTR.exe bgp.tools -6
WinMTR.exe --report-cycles 10 1.1.1.1
WinMTR.exe --report-seconds 30 --numeric example.com
WinMTR.exe --gui example.com
```

Supported CLI options include:
- `-g`, `--gui`
- `-r`, `--report`
- `-c`, `--report-cycles <count>`
- `-w`, `--report-seconds <seconds>`
- `-i`, `--interval <seconds>`
- `-s`, `--size <bytes>`
- `-n`, `--numeric`
- `-4`, `--ipv4`
- `-6`, `--ipv6`

CLI mode runs live by default, refreshes in place, and uses an alternate terminal screen where supported so repeated cycles do not fill the scrollback. Stop it with `Ctrl+C`.

CLI and exported reports include per-hop ASN data when it can be resolved.

### GUI usage
Launching `WinMTR.exe` without command-line arguments opens the desktop GUI directly. The GUI follows the Windows app light/dark setting where supported, including the main controls, status bar, and results table header.

Use `WinMTR.exe --gui <host>` if you want to open the GUI with a pre-filled host from a terminal.

### Build
Open `src\WinMTR.sln` in Visual Studio or build from a Developer PowerShell:

```powershell
MSBuild.exe src\WinMTR.sln /t:Build /p:Configuration=Release /p:Platform=x64
```

The release binary is written to `src\Release_x64\WinMTR.exe`.

### About me / why I decided to create this fork
There isn't that much to say actually, I've been using IPv6 for a few years now thanks to [**SixXS**](http://sixxs.net/)
and it always annoyed me that WinMTR couldn't handle IPv6... finally my ISP got some sort of IPv6 beta test.
And that's what I wanted to compare: native vs SixXS with long-term trace routes such as those WinMTR provides. <br>
Since there wasn't any WinMTR build with IPv6, I decided to do it myself ;) The result can be seen here :P <br>
*(after 1 day for IPv6, and 2 additional days to fix other stuff and polishing)*

**If you're looking for an alternative** *(not meant for long-term traces)* there's [**vTrace**](http://vtrace.pl).
It's some really interesting piece of Software ;) *(with more then just trace routes)*
~~~~
