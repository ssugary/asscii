#include <cstdlib>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <string>

std::string defaultAsciiChars = "Ñ@#W$987654321?!abc;:+=-,._                           ";

 /**
   * @struct RenderOptions
   * @brief stores the render options passed by the user.
   */
struct RenderOptions {
    std::string video_path;     ///< Path to the input video file. Empty if using webcam
    std::string palette;        ///< ASCII chars palette 
    bool help_flag = false;     ///< Flag to see the help window
    bool palette_flag = false;  ///< Flag to input a custom palette 
    bool webcam_flag = false;   ///< Flag to use the webcam 
    bool saveFrames = false;    ///< Flag to save the video frames
    bool show = false;          ///< Flag to display the video in a new window
    int cell_width = 8;         ///< Horizontal scale for the ASCII cell in pixels
    int cell_height = 12;       ///< Vertical scale for the ASCII cell in pixels
    float correction = 0.5f;    ///< Aspect ratio 

    void parseArgs(int argc, char* argv[]) {
        const char* keys =
            "{h help       |       | Print the help message }"
            "{v video      |       | The input file path }"
            "{p palette    |       | Custom ASCII character palette }"
            "{wc webcam    | false | Use the webcam instead of a video file }"
            "{w width      | 8     | Cell's width }"
            "{he height    | 12    | Cell's height }"
            "{co correction| 0.5   | Aspect ratio }"
            "{sf saveframes| false | Save the video frames in a directory}"
            "{s show       | true  | Shows the video frames in a new window (always true when wc = true)}"
            ;

        cv::CommandLineParser parser(argc, argv, keys);    ///< Opencv default parser 

        /// Checks if the user requested help
        if (parser.has("help")) {
            help_flag = true;
            parser.printMessage();
            return;
        }

        /// Check if the user inputs the video file path and saves it
        if (parser.has("video")) {
            video_path = parser.get<std::string>("video");
        }
        
        /// Check if the user wants to input a personalized ASCII palette 
        if (parser.has("palette")) {
            std::cout << "Enter the ASCII palette you want! (maximum 20 characters)\n";
            
            std::string temp;
            std::getline(std::cin, temp);       ///< Get the string with spaces
            palette = temp.substr(0, 20);          ///< Stores the new palette
            palette_flag = true; 
        }
        show = parser.get<bool>("show");             ///< Check if the user wants to show the video frames in a new window
        saveFrames = parser.get<bool>("saveframes"); ///< Check if the user wants to save the new video frames
        webcam_flag = parser.get<bool>("webcam");    ///< Check if the user wants to use the webcam
        cell_width = parser.get<int>("width");       ///< Check if the user inputs the cell's width and saves it
        cell_height = parser.get<int>("height");     ///< Check if the user inputs the cell's height and saves it
        correction = parser.get<float>("correction");///< Check if the user inputs a personalized aspect ratio and saves it
        
        if (!parser.check()) { ///< Error case
            parser.printErrors();
        }
    }
};


int main(int argc, char* argv[]) {

    RenderOptions ro;
    ro.parseArgs(argc, argv);   

    if (ro.help_flag)return EXIT_SUCCESS; ///< If it shows help, the program ends

    cv::VideoCapture cap;

    if(ro.webcam_flag && !ro.video_path.empty()){
        std::cerr << "Error: More than one video input selected. Choose -v=video.mp4 or -wc" << std::endl;
        return EXIT_FAILURE;
    }
    if (ro.webcam_flag) {
        cap.open(0, cv::CAP_V4L2); 
        ro.show = true; ///< always true when webcam_flag = true.
    } 
    else if (!ro.video_path.empty()) {
        cap.open(ro.video_path);
    } 
    else {
        std::cerr << "Error: Invalid quantity of video inputs. Use -v=video.mp4 or -wc" << std::endl;
        return EXIT_FAILURE;
    }

    std::string asciiChars = ro.palette_flag ? ro.palette : defaultAsciiChars; ///< Define the ASCII palette 
    const int ASCII_CHARS_SIZE = asciiChars.size();

    cv::Mat frame, gray, small; ///< 3 frame phases:
                                ///< frame: A simple video's capture 
                                ///< gray:  The initial frame at COLOR_BGR2GRAY channel
                                ///< small: The 'gray' frame in a smaller cell

    int frameCount{0};     ///< frame counter
    const int numCols = 100;     
    
    std::filesystem::path framesDir = "frames/"; ///< Directory that stores the frames

    if(ro.saveFrames){
        std::filesystem::create_directory(framesDir); ///< Create the directory that stores the frames
    }

    while(true) {
        cap >> frame;           ///< Capture frame
        if(frame.empty()) break;///< Video ends
        
        /// Changes the frame to the gray's representation
        if (frame.channels() >= 3) cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        else gray = frame;

        /// Set the Cell rows quantity 
        int numRows = static_cast<int>(numCols * gray.rows / gray.cols * ro.correction); 

        /// Changes the gray frame to a smaller representation
        cv::resize(gray, small, cv::Size(numCols, numRows)); 

        /// Define the Cell that displays the new video
        cv::Mat canvas = cv::Mat::zeros(numRows * ro.cell_height, numCols * ro.cell_width, CV_8UC3);

        std::string s{" "};
        for(int i = 0; i < small.rows; ++i) {
            for(int j = 0; j < small.cols; ++j) { ///< For each pixel (i, j) in the img

                /// Pixel (i, j) gray intensity (0 -> 255) 
                uchar intensity = small.at<uchar>(i, j); 

                /// Map the intensity to the range 0 -> ASCII_CHARS_SIZE - 1 (string acess range)
                s[0] = asciiChars[(intensity * (ASCII_CHARS_SIZE - 1)) / 255];
                
                /// Map the pixel position to the Cell's range
                cv::Point pos(j * ro.cell_width, (i + 1) * ro.cell_height); 

                /// Add pixel to the Cell 
                cv::putText(canvas, s, pos,     
                            cv::FONT_HERSHEY_SIMPLEX, 0.35, 
                            cv::Scalar(180, 180, 180), 1, cv::LINE_AA);
            }
        }
        
        if(ro.saveFrames){
            /// Add the frame to the frames directory
            std::string filename = framesDir.generic_string() + "frame_" + std::to_string(frameCount++) + ".png";
            imwrite(filename, canvas);
        }

        /// Displays the frame in a new window
        if(ro.show){
            cv::imshow("asscii", canvas);
        }

        /// Break if the key 'q' is pressed
        if((cv::waitKey(1) & 0xFF) == 'q') break;
    
        /// Show the video progress bar 
        if(!ro.webcam_flag){
            double progress = (double)cap.get(cv::CAP_PROP_POS_FRAMES) / cap.get(cv::CAP_PROP_FRAME_COUNT) * 100;
            std::cout << "\rRender: " << (int)progress << "% " << std::flush;
        }
    }
    std::cout << '\n';
    return EXIT_SUCCESS;
}