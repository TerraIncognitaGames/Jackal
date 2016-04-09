1. Нужно указать среде, что мы будем использовать эту библиотеку. Найдите в каталоге, в который вы установили вашу среду вложенные директории include и lib и создайте в include папку gl.
2. В include\gl скопируйте glut.h и glaux.h.
3. В lib скопируйте glut.lib и glaux.lib.

У меня это примерно так: D\Program Files\Microsoft Visual Studio 9.0\VC\include\GL и
D:\Program Files\Microsoft Visual Studio 9.0\VC\lib

Теперь в наших VC++ проектах можно использовать glut и glaux. Для этого нужно просто подключить вначале проекта загаловочный файлы

#include <gl\glut.h>
#include <gl\glaux.h>
