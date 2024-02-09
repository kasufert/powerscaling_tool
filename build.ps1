Remove-Item build/dbg/main.exe, *.obj, *.pdb

cl.exe main.cpp -DSFML_STATIC -Zi -W4 -EHsc -MTd `
    -diagnostics:classic `
    -external:W0 -std:c++latest `
    -IC:\src\vcpkg\installed\x64-windows-static\include `
    /link /LIBPATH:C:\src\vcpkg\installed\x64-windows-static\debug\lib `
    sfml-system-s-d.lib sfml-graphics-s-d.lib sfml-window-s-d.lib sfml-audio-s-d.lib `
    winmm.lib gdi32.lib opengl32.lib freetyped.lib user32.lib Advapi32.lib `
    openal32.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib ole32.lib `
    ws2_32.lib Shell32.lib zlibd.lib libpng16d.lib `
    brotlicommon-static.lib brotlienc-static.lib brotlidec-static.lib `
    bz2d.lib `
    fmtd.lib `
    /out:build/dbg/main.exe `
    /subsystem:console