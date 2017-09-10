#include <iostream>
#include <stdexcept>
#include "opencv2/core/core.hpp"
//#include <opencv2/core/face.hpp>
//#include "opencv.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


const char* keys =
{
    "{ help h      |                     | print help message }"
    "{ image i     |                     | specify input image}"
    "{ camera c    |                     | enable camera capturing }"
    "{ video v     | ../data/vtest.avi   | use video as input }"
    "{ directory d |                     | images directory}"
};

static void detectAndDraw(const HOGDescriptor &hog, Mat &img)
{
    vector<Rect> found, found_filtered;
    double t = (double) getTickCount();
    // Run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    hog.detectMultiScale(img, found, 0, Size(4,4), Size(32,32), 1.05, 2);
    t = (double) getTickCount() - t;
    cout << "detection time = " << (t*1000./cv::getTickFrequency()) << " ms" << endl;

    for(size_t i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];

        size_t j;
        // Do not add small detections inside a bigger detection.
        for ( j = 0; j < found.size(); j++ )
            if ( j != i && (r & found[j]) == r )
                break;

        if ( j == found.size() )
            found_filtered.push_back(r);
    }

    for (size_t i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];

        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }
}

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, keys);
    String parser_value ;

    VideoCapture cap(1); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
   /* 
    if (parser.has("help"))
    {
        cout << "\nThis program demonstrates the use of the HoG descriptor using\n"
            " HOGDescriptor::hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());\n";
        parser.printMessage();
        cout << "During execution:\n\tHit q or ESC key to quit.\n"
            "\tUsing OpenCV version " << CV_VERSION << "\n"
            "Note: camera device number must be different from -1.\n" << endl;
        return 0;
    }
        */
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    namedWindow("people detector", 1);

    string pattern_glob = "";
    string video_filename = "../data/vtest.avi";
    int camera_id = -1;
    

            Mat frame = imread("oneperson.jpg");

          //  if (frame.empty())
           //     break;

            detectAndDraw(hog, frame);

            imshow("people detector", frame);
            waitKey(0);
         //   int c = waitKey( vc.isOpened() ? 30 : 0 ) & 255;
          //  if ( c == 'q' || c == 'Q' || c == 27)
           //     break;
        
    

    return 0;
}
