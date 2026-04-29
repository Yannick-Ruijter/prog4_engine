cmake_minimum_required(VERSION 3.26)
project(Minigin)

if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

include(FetchContent)

find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_MakeAvailable(glm)
endif()

find_package(SDL3 3.4 CONFIG QUIET)
if(NOT SDL3_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3
      URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
    find_package(SDL3 CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3
      GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
      GIT_TAG release-3.4.0
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL3)
  endif()
endif()

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.92.5-docking
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${sdl3_SOURCE_DIR}/include)
target_link_libraries(imgui PUBLIC SDL3::SDL3)
target_compile_features(imgui PUBLIC cxx_std_20)

FetchContent_Declare(
    imgui_plot
    GIT_REPOSITORY https://github.com/soulthreads/imgui-plot.git
)
FetchContent_MakeAvailable(imgui_plot)
target_include_directories(imgui_plot PUBLIC ${imgui_plot_SOURCE_DIR})
target_link_libraries(imgui_plot PUBLIC imgui)

find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if(NOT SDL3_ttf_FOUND)
  if (WIN32)
    message("sdl not found, retrieving from github")
    FetchContent_Declare(
      SDL3_ttf
      URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3_ttf)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
    find_package(SDL3_ttf CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3_ttf
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
      GIT_TAG release-3.2.2
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    set(SDL3TTF_INSTALL OFF)
    if(EMSCRIPTEN)
      list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten")
      set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
      set(SDL3TTF_VENDORED OFF)
      set(SDLTTF_PLUTOSVG OFF)
      add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
      add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
    endif()
    FetchContent_MakeAvailable(SDL3_ttf)
  endif()
endif()

if (WIN32)
  find_package(VLD CONFIG)
endif()

# Engine
add_library(minigin_lib STATIC
  "Minigin/Font.cpp"
  "Minigin/GameObject.cpp"
  "Minigin/InputManager.cpp"
  "Minigin/Minigin.cpp"
  "Minigin/Renderer.cpp"
  "Minigin/ResourceManager.cpp"
  "Minigin/Scene.cpp"
  "Minigin/SceneManager.cpp"
  "Minigin/Component.cpp"
  "Minigin/Game.cpp"
  "Minigin/TransformComponent.cpp"
  "Minigin/ControllerInput.cpp"
  "Minigin/KeyboardInput.cpp"
  "Minigin/Binding.cpp"
  "Minigin/TimeManager.cpp"
  "Minigin/Texture2DComponent.cpp"
)

target_compile_features(minigin_lib PUBLIC cxx_std_20)

target_include_directories(minigin_lib PUBLIC
  "${CMAKE_SOURCE_DIR}/Minigin/"
  "${imgui_SOURCE_DIR}"
  "${VLD_INCLUDE_DIR}"
)

target_link_libraries(minigin_lib PUBLIC
  SDL3::SDL3
  SDL3_ttf::SDL3_ttf
  glm::glm
  imgui
  imgui_plot
  ${VLD_LIBRARIES}
)

if(MSVC)
  target_compile_options(minigin_lib PRIVATE /W4 /WX)
else()
  target_compile_options(minigin_lib PRIVATE -Wall -Wextra -Werror)
endif()

# Game files
if(EMSCRIPTEN)
  set(TARGET_NAME minigin_web)
  set(TARGET_PARAM "")
else()
  set(TARGET_NAME minigin)
  set(TARGET_PARAM WIN32)
endif()

add_executable(${TARGET_NAME} ${TARGET_PARAM}
  "Game/Main.cpp"
  "Game/BurgerTime.cpp"
  "GameComponents/TextComponent.cpp"
  "GameComponents/RenderComponent.cpp"
  "GameComponents/FPSComponent.cpp"
  "GameComponents/OrbitComponent.cpp"
  "GameComponents/TrashTheCacheComponent.cpp"
  "GameComponents/HealthComponent.cpp"
  "GameComponents/LivesDisplayComponent.cpp"
  "GameComponents/ScoreComponent.cpp"
  "GameComponents/ScoreDisplayComponent.cpp"
  "GameComponents/PlayerStateComponent.cpp"
  "GameExtras/PlayerLivesObserver.cpp"
  "GameExtras/Achievements.cpp"
  "GameExtras/CustomCommands.cpp"
  "GameExtras/Subject.cpp"
  "GameExtras/ServiceProvider.cpp"
)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

target_include_directories(${TARGET_NAME} PRIVATE
  "${CMAKE_SOURCE_DIR}/Game/"
  "${CMAKE_SOURCE_DIR}/GameComponents/"
  "${CMAKE_SOURCE_DIR}/GameExtras/"
  "${CMAKE_SOURCE_DIR}/Data/"
)

# Linking minigin_lib pulls in all its PUBLIC dependencies automatically
target_link_libraries(${TARGET_NAME} PRIVATE minigin_lib)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Werror)
endif()


if(EMSCRIPTEN)
  set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\"")
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)
  set_target_properties(${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
  )
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif(WIN32)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

  add_custom_command(
  TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:SDL3::SDL3>"
      "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
      "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  VERBATIM
)

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )

elseif(APPLE)
  set_target_properties(${TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    INSTALL_RPATH "@executable_path/../Frameworks"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared>" "$<TARGET_BUNDLE_DIR:${TARGET_NAME}>/Contents/Frameworks/"
    COMMAND install_name_tool -change "@rpath/libSDL3_ttf.0.dylib" "@executable_path/../Frameworks/libSDL3_ttf.0.2.2.dylib" "$<TARGET_FILE:${TARGET_NAME}>"
    COMMAND install_name_tool -change "@rpath/libSDL3.0.dylib" "@executable_path/../Frameworks/libSDL3.0.dylib" "$<TARGET_FILE:${TARGET_NAME}>"
  )
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )
endif()