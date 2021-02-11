#include "Script.h"

Script::Script()
{
}

int PuntoX = 218;
int PuntoY = 339;
Mat binarizada;
Mat binarizada2;

Mat global;

Mat hsv;
Mat yrcb;

Mat clon;

int hMin = 102, sMin = 1, vMin = 1, hMax = 180, sMax = 255, vMax = 255;
int yMin = 59, crMin = 137, cbMin = 99, yMax = 255, crMax = 255, cbMax = 157;

RNG rng(1);

Ptr<SIFT> sDetector = SIFT::create();
vector<KeyPoint> keypointsFrame, keypointsGesto, keypointsGesto2, keypointsGesto3;
Mat descriptorFrame, descriptorGesto, descriptorGesto2, descriptorGesto3;
Mat gesto1 = imread("/home/christian/VisionParte2/ProyectoFinal/gestos/gesto1.png");
Mat gesto2 = imread("/home/christian/VisionParte2/ProyectoFinal/gestos/gesto2.png");
Mat gesto3 = imread("/home/christian/VisionParte2/ProyectoFinal/gestos/gesto3.png");

void Script::encenderCamara()
{
    VideoCapture cap(0);
    if (!cap.isOpened())
        return;
    Mat frame;
    Mat frame2;
    Mat mascara = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);
    Mat resAND;
    Mat resultadoBusqueda;
    Mat clonRoi;
    Mat clon2;
    Mat img_matches;
    Mat img_matches2;
    Mat img_matches3;



    puntosGesto();

    

    namedWindow("Video", WINDOW_AUTOSIZE);
    namedWindow("HSV-Mask", WINDOW_AUTOSIZE);
    namedWindow("YrCb-Mask", WINDOW_AUTOSIZE);
    namedWindow("Global-Mask", WINDOW_AUTOSIZE);
    //namedWindow("Contours",WINDOW_AUTOSIZE);

    createTrackbar("H-Min", "Video", &hMin, 180, eventoTrackbar, NULL);
    createTrackbar("S-Min", "Video", &sMin, 255, eventoTrackbar, NULL);
    createTrackbar("V-Min", "Video", &vMin, 255, eventoTrackbar, NULL);

    createTrackbar("H-Max", "Video", &hMax, 180, eventoTrackbar, NULL);
    createTrackbar("S-Max", "Video", &sMax, 255, eventoTrackbar, NULL);
    createTrackbar("V-Max", "Video", &vMax, 255, eventoTrackbar, NULL);

    createTrackbar("Y-Min", "Video", &yMin, 180, eventoTrackbar, NULL);
    createTrackbar("Cr-Min", "Video", &crMin, 255, eventoTrackbar, NULL);
    createTrackbar("Cb-Min", "Video", &cbMin, 255, eventoTrackbar, NULL);

    createTrackbar("Y-Max", "Video", &yMax, 180, eventoTrackbar, NULL);
    createTrackbar("Cr-Max", "Video", &crMax, 255, eventoTrackbar, NULL);
    createTrackbar("Cb-Max", "Video", &cbMax, 255, eventoTrackbar, NULL);

    while (1)
    {
        cap >> frame;
        //resize(frame, frame, Size(650, 400));
        flip(frame, frame, 1);
        clon = frame.clone();
        clon2 = frame.clone();

        Point minLoc, maxLoc;
        rectangle(frame, minLoc, Point(minLoc.x + PuntoX, minLoc.y + PuntoY), Scalar(0, 0, 255), 4);

        Mat roi = frame(Rect(0, 0, PuntoX, PuntoY));

        clonRoi = roi.clone();

        sDetector->detect(clon2, keypointsFrame);
        sDetector->compute(clon2, keypointsFrame, descriptorFrame);
        drawKeypoints(clon2, keypointsFrame, clon2);

        BFMatcher matcher;
        vector<vector<DMatch>> matches;
        matcher.knnMatch(descriptorGesto, descriptorFrame, matches, 2);

        vector<DMatch> matchesFiltrados;

        float ratio = 0.67;
        for (int i = 0; i < matches.size(); i++)
        {
            if (matches[i][0].distance < ratio * matches[i][1].distance)
                matchesFiltrados.push_back(matches[i][0]);
        }

        drawMatches(gesto1, keypointsGesto, clon2, keypointsFrame, matchesFiltrados, img_matches);
        imshow("Matches", img_matches);

        BFMatcher matcher2;
        vector<vector<DMatch>> matches2;
        matcher.knnMatch(descriptorGesto2, descriptorFrame, matches2, 2);

        vector<DMatch> matchesFiltrados2;

     
        for (int i = 0; i < matches2.size(); i++)
        {
            if (matches2[i][0].distance < ratio * matches2[i][1].distance)
                matchesFiltrados2.push_back(matches2[i][0]);
        }

        drawMatches(gesto2, keypointsGesto2, clon2, keypointsFrame, matchesFiltrados2, img_matches2);
        imshow("Matches 2", img_matches2);

        BFMatcher matcher3;
        vector<vector<DMatch>> matches3;
        matcher3.knnMatch(descriptorGesto3, descriptorFrame, matches3, 2);

        vector<DMatch> matchesFiltrados3;


        for (int i = 0; i < matches3.size(); i++)
        {
            if (matches3[i][0].distance < ratio * matches3[i][1].distance)
                matchesFiltrados3.push_back(matches[i][0]);
        }

        drawMatches(gesto3, keypointsGesto3, clon2, keypointsFrame, matchesFiltrados3, img_matches3);
        imshow("Matches 3", img_matches3);




       

        

    


    

        cvtColor(roi, hsv, COLOR_BGR2HSV);
        cvtColor(roi, yrcb, COLOR_BGR2YCrCb);

        inRange(hsv, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), binarizada);
        imshow("HSV-Mask", binarizada);

        inRange(yrcb, Scalar(yMin, crMin, cbMin), Scalar(yMax, crMax, cbMax), binarizada2);
        imshow("YrCb-Mask", binarizada2);

        //cambiar a or
        bitwise_or(binarizada, binarizada2, global);

        detectarContorno(global, clon);
        setMouseCallback("Video", eventoRaton, NULL);
        imshow("Video", frame);
        imshow("Global-Mask", global);

        //namedWindow("Video-HSV", WINDOW_AUTOSIZE);
        //imshow("Video-HSV", hsv);

        if (waitKey(10) >= 0)
            break;
    }
    destroyAllWindows();
}

void Script::eventoTrackbar(int v, void *p)
{
    //inRange(hsv,Scalar(hMin,sMin,vMin),Scalar(hMax,sMax,vMax),binarizada);
    //imshow("Video-Binario", binarizada);
}

void Script::detectarContorno(Mat frame, Mat clon)
{
    Mat canny;
    float ratio = 3.f;
    float cThreshold = 180.f;

    Canny(frame, canny, cThreshold * ratio, 3);
    vector<vector<Point>> puntosContorno;
    vector<Vec4i> jerarquia;

    findContours(canny, puntosContorno, jerarquia, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    int lrgctridx = 0;
    int maxarea = 0;

    for (int i = 0; i < puntosContorno.size(); i++)
    {
        double a = contourArea(puntosContorno[i]);
        if (a > maxarea)
        {
            maxarea = a;
            lrgctridx = i;
        }
    }

    //hull = cv.convexHull(contours)
    vector<vector<Point>> hull(puntosContorno.size());
    vector<vector<int>> hullsI(puntosContorno.size());
    vector<vector<Vec4i>> defectos(puntosContorno.size());

    for (size_t i = 0; i < puntosContorno.size(); i++)
    {
        convexHull(puntosContorno[i], hull[i], false);
        convexHull(puntosContorno[i], hullsI[i], false);
        if (hullsI[i].size() > 3)
        {
            convexityDefects(puntosContorno[i], hullsI[i], defectos[i]);
        }
    }

    //cout << defectos.size() << endl;
    if (defectos.size() == 1)
    {
        putText(clon, "gesto 1", Point(300,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
    }
    if (defectos.size() == 2)
    {
        putText(clon, "gesto 2", Point(300, 100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
    }
    if (defectos.size() == 5 )
    {
        putText(clon, "gesto 3", Point(300,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
    }
    

    for (size_t i = 0; i < puntosContorno.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(clon, puntosContorno, (int)i, Scalar(255, 0, 0), 2, LINE_8, jerarquia, 0);
        //drawContours( clon, puntosContorno, (int)i, color,jerarquia);

        drawContours(clon, hull, (int)i, color);

        for (const Vec4i &v : defectos[i])
        {
            float depth = v[3] / 256;
            if (depth > 10) //  filter defects by depth, e.g more than 10
            {
                int startidx = v[0];
                Point ptStart(puntosContorno[i][startidx]);
                int endidx = v[1];
                Point ptEnd(puntosContorno[i][endidx]);
                int faridx = v[2];
                Point ptFar(puntosContorno[i][faridx]);

                //line(clon, ptStart, ptEnd, Scalar(0, 255, 0), 1);
                //line(clon, ptStart, ptFar, Scalar(0, 255, 0), 1);
                //line(clon, ptEnd, ptFar, Scalar(0, 255, 0), 1);
                circle(clon, ptFar, 4, Scalar(0, 255, 0), 2);
            }
        }
    }

    imshow("Contours", clon);

    //cout << "#contornos"<< puntosContorno[0].size() <<endl;

    imshow("Canny", canny);
    //imshow("contorno dibujado",frame);
}

void Script::tomarFoto()
{
    VideoCapture cap(0);
    if (!cap.isOpened())
        return;

    Mat frame;
    namedWindow("Capturar Gesto", WINDOW_AUTOSIZE);
    namedWindow("ROI");
    while (1)
    {
        cap >> frame;
        flip(frame, frame, 1);
        Point minLoc, macLoc;
        rectangle(frame, minLoc, Point(minLoc.x + PuntoX, minLoc.y + PuntoY), Scalar(0, 0, 255), 4);
        Mat roi = frame(Rect(0, 0, PuntoX, PuntoY));
        setMouseCallback("Capturar Gesto", eventoRaton, NULL);
        imshow("Capturar Gesto", frame);
        imshow("ROI", roi);

        if (waitKey(10) == 32)
            break;
        if (waitKey(10) == 97)
            imwrite("/home/christian/VisionParte2/ProyectoFinal/gestos/gesto1.png", roi);
        if (waitKey(10) == 98)
            imwrite("/home/christian/VisionParte2/ProyectoFinal/gestos/gesto2.png", roi);
        if (waitKey(10) == 99)
            imwrite("/home/christian/VisionParte2/ProyectoFinal/gestos/gesto3.png", roi);
    }
}

void Script::eventoRaton(int evento, int x, int y, int flags, void *pData)
{

    if (evento == EVENT_LBUTTONDOWN)
    {
        PuntoX = x;
        PuntoY = y;
        cout << "Dio Click: " << x << "," << y << endl;
    }
    else if (evento == EVENT_MOUSEMOVE)
    {
        //PuntoX = x;
        //PuntoY = y;
        //cout << "Movio el ratón: " << x << "," << y << endl;
    }
}

void Script::comparar(Mat clon)
{
    Mat solvay = clon;
    Mat marie = imread("/home/christian/VisionParte2/ProyectoFinal/gestos/gesto1.png");

    Mat resultadoBusqueda;
    matchTemplate(solvay, marie, resultadoBusqueda, TM_SQDIFF_NORMED); // resultadoBusqueda es una imagen con valores en los pixeles en decimal

    // Normalizamos la imagen a un rango [0,1]
    normalize(resultadoBusqueda, resultadoBusqueda, 0, 1, NORM_MINMAX, -1, Mat());

    double minVal = 0, maxVal = 0;
    Point minLoc, maxLoc;
    minMaxLoc(resultadoBusqueda, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    rectangle(solvay, minLoc, Point(minLoc.x + marie.cols, minLoc.y + marie.rows), Scalar(10, 200, 200), 4);
    //rectangle(solvay,maxLoc,Point(maxLoc.x+marie.cols,maxLoc.y+marie.rows),Scalar(10,10,200),4);

    namedWindow("Original", WINDOW_AUTOSIZE);

    imshow("Original", solvay);

    waitKey(0);

    destroyAllWindows();
}
void Script::puntosGesto()
{
    sDetector->detect(gesto1, keypointsGesto);
    sDetector->compute(gesto1, keypointsGesto, descriptorGesto);
    drawKeypoints(gesto1, keypointsGesto, gesto1);

    sDetector->detect(gesto2, keypointsGesto2);
    sDetector->compute(gesto2, keypointsGesto2, descriptorGesto2);
    drawKeypoints(gesto2, keypointsGesto2, gesto2);

    sDetector->detect(gesto3, keypointsGesto3);
    sDetector->compute(gesto3, keypointsGesto3, descriptorGesto3);
    drawKeypoints(gesto3, keypointsGesto3, gesto3);
   
}