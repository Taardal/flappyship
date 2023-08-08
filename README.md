# Flappyship

<div float="left">
  <img src="./img/flappyship.gif" width="800" />
</div>

# Description

Flappy Bird clone made with my own [Storytime][storytime] game engine.

Both the game and the engine is made while learning myself game programming by following tutorials made by [TheCherno][thecherno] on YouTube. The game is based on the [Hazel: 1 Hour Game][thecherno:onehourgame:repo], and the engine is based on the [Game Engine Series][thecherno:gameengineseries] tutorial series.

# Prerequisites&nbsp;:vertical_traffic_light:

- [Git][git]
- [CMake][cmake]
- C++ compiler
    - Windows: [MSVC][msvc] (Bundled with [Visual Studio][msvs])
    - MacOS: [Clang][clang] (Bundled with [Xcode][xcode])
    - Linux: [GCC][gcc]

# Getting started&nbsp;:runner:

### Getting the code&nbsp;:octocat:

- Clone the repository: `git clone https://github.com/taardal/flappyship`

### Running the app&nbsp;:rocket:

- Generate project files: `cmake -S . -B build`
- Build executable from generated files: `cmake --build build`
- Navigate to the build folder: `cd build`
- Run the executable: `./flappyship`

# Resources&nbsp;:books:

- [The Cherno: Game Engine Series][thecherno:gameengineseries] (YouTube)
- [The Cherno: Batch Rendering][thecherno:batchrendering] (YouTube)
- [The Cherno: 1 Hour Game][thecherno:onehourgame:repo] (Github)
- [The Cherno: OpenGL][thecherno:opengl] (YouTube)
- [The Cherno: C++][thecherno:cpp] (YouTube)
- [OpenGL Tutorial][opengltutorial] (Website)
- [Learn OpenGL][learnopengl] (Website)


[clang]: http://clang.org/
[cmake]: https://cmake.org/
[gcc]: https://gcc.gnu.org/
[git]: https://git-scm.com
[learnopengl]: https://learnopengl.com/
[msvc]: https://visualstudio.microsoft.com/vs/features/cplusplus/
[msvs]: https://visualstudio.microsoft.com/
[opengltutorial]: http://www.opengl-tutorial.org/
[storytime]: https://github.com/taardal/storytime
[thecherno]: https://www.youtube.com/@TheCherno
[thecherno:batchrendering]: https://www.youtube.com/watch?v=Th4huqR77rI&list=PLlrATfBNZ98f5vZ8nJ6UengEkZUMC4fy5&ab_channel=TheCherno 
[thecherno:cpp]: https://www.youtube.com/watch?v=18c3MTX0PK0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&ab_channel=TheCherno
[thecherno:gameengineseries]: https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&ab_channel=TheCherno
[thecherno:onehourgame]: https://www.youtube.com/watch?v=qITIvVV6BHk&ab_channel=TheCherno
[thecherno:onehourgame:repo]: https://github.com/TheCherno/Hazel1HourGame
[thecherno:opengl]: https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&ab_channel=TheCherno
[xcode]: https://developer.apple.com/xcode/