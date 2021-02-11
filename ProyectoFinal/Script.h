#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <fstream> // escribe y lee archivos
#include <sstream>
// Las librerias de C puro acaban en .h (cabeceras)
#include <dirent.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <bits/stdc++.h>

#include <iostream>
#include <cstdlib>

#include <cmath> // Esta librería contiene las funciones para realizar operaciones matemáticas (sin, cos, tan, log, exp, etc.)
#include <ctime>
#include <vector>

// Cuando se carga la cabecer opencv.hpp automáticamente se cargan las demás cabeceras
//#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>       // Contiene los elementos básicos como el objeto Mat (matriz que representa la imagen)
#include <opencv2/highgui/highgui.hpp> // Contiene los elementos para crear una interfaz gráfica básica
// OpenCV no está pensado para crear interfaces gráficas potentes. Se centra en la visión artificial y PDI. Si se desea crear una interfaz gráfica completa, se debe usar QT

#include <opencv2/imgcodecs/imgcodecs.hpp> // Contiene las funcionalidad para acceder a los códecs que permiten leer diferentes formatos de imagen (JPEG, JPEG-2000, PNG, TIFF, GIF, etc.)

// Librerías para acceder al video y para poder escribir vídeos en disco
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>

#include <opencv2/imgproc/imgproc.hpp> // Librería para realizar operaciones de PDI

#include <opencv2/objdetect/objdetect.hpp> // Librería contiene funciones para detección de objetos

#include <opencv2/features2d/features2d.hpp> // Librería que contiene funciones para detección de descriptores como SIFT

#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

class Script
{

    private:
    public:
    void encenderCamara();
    static void eventoRaton(int evento,int x, int y, int flags, void *pData);
    static void eventoTrackbar(int v, void *p);
    void detectarContorno(Mat,Mat);
    void tomarFoto();
    void comparar(Mat);
    void puntosGesto();
    
   

    


    Script();
};