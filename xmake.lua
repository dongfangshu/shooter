add_rules("mode.debug", "mode.release")
set_languages("c++20")

if is_plat("windows") then
    --windows自带图形
    add_requires("libsdl2","libsdl2_ttf", "libsdl2_image")
else 
    -- Linux/macOS 必须用系统 SDL，防止 headless
    add_requires("pkgconfig::sdl2")
    add_requires("pkgconfig::SDL2_ttf")
    add_requires("pkgconfig::SDL2_image")
end

target("shooter")
    set_kind("binary")
    add_files("src/*.cpp", "src/**/*.cpp")
    if is_plat("windows") then
        add_cxxflags("/utf-8")
        add_defines("SDL_MAIN_HANDLED")
    else 
        add_cxxflags("-finput-charset=UTF-8")
    end

    if is_plat("windows") then
        add_packages("libsdl2", "libsdl2_ttf", "libsdl2_image")
    else 
    add_packages("pkgconfig::sdl2",
                 "pkgconfig::SDL2_ttf",
                 "pkgconfig::SDL2_image")
    end
    -- add_syslinks("dbghelp")
    if is_mode("debug") then
        add_defines("ENABLE_LOG")
    end
    -- add_defines("ENABLE_INPUT_LOG")
    after_build(function (target)
        local install_dir = path.join(target:targetdir(), "assets")
        os.mkdir(install_dir)
        os.cp("assets/*", install_dir)
    end)