-- premake5.lua
solution "hw4"
   configurations { "Debug", "Release" }

   -- A project defines one build target
   project "main"
      kind "ConsoleApp"
      language "C++"
      files { "src/*.cpp" }
      buildoptions "-std=c++11"

      configuration { "windows" }
        libdirs { "lib" }
        includedirs { "include", "src" }
        links { "FreeGLUT", "glu32", "opengl32", "FreeImage"}

      configuration { "macosx" }
        includedirs { "src" }
        linkoptions { "-framework Carbon -framework OpenGL -framework GLUT" }
        links { "FreeImage" }

      configuration { "not windows", "not macosx" }
        includedirs { "src" }
        links { "X11", "GL", "GLU", "glut", "freeimage" }

      configuration "Debug"
         defines { "DEBUG" } -- -DDEBUG
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" } -- -NDEBUG
         flags { "Optimize" }