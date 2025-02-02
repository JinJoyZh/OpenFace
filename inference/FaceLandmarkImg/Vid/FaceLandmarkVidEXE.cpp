///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017, Carnegie Mellon University and University of Cambridge,
// all rights reserved.
//
// ACADEMIC OR NON-PROFIT ORGANIZATION NONCOMMERCIAL RESEARCH USE ONLY
//
// BY USING OR DOWNLOADING THE SOFTWARE, YOU ARE AGREEING TO THE TERMS OF THIS LICENSE AGREEMENT.  
// IF YOU DO NOT AGREE WITH THESE TERMS, YOU MAY NOT USE OR DOWNLOAD THE SOFTWARE.
//
// License can be found in OpenFace-license.txt

//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite at least one of the following works:
//
//       OpenFace 2.0: Facial Behavior Analysis Toolkit
//       Tadas Baltru�aitis, Amir Zadeh, Yao Chong Lim, and Louis-Philippe Morency
//       in IEEE International Conference on Automatic Face and Gesture Recognition, 2018  
//
//       Convolutional experts constrained local model for facial landmark detection.
//       A. Zadeh, T. Baltru�aitis, and Louis-Philippe Morency,
//       in Computer Vision and Pattern Recognition Workshops, 2017.    
//
//       Rendering of Eyes for Eye-Shape Registration and Gaze Estimation
//       Erroll Wood, Tadas Baltru�aitis, Xucong Zhang, Yusuke Sugano, Peter Robinson, and Andreas Bulling 
//       in IEEE International. Conference on Computer Vision (ICCV),  2015 
//
//       Cross-dataset learning and person-specific normalisation for automatic Action Unit detection
//       Tadas Baltru�aitis, Marwa Mahmoud, and Peter Robinson 
//       in Facial Expression Recognition and Analysis Challenge, 
//       IEEE International Conference on Automatic Face and Gesture Recognition, 2015 
//
///////////////////////////////////////////////////////////////////////////////
// FaceTrackingVid.cpp : Defines the entry point for the console application for tracking faces in videos.

// Libraries for landmark detection (includes CLNF and CLM modules)
#include "LandmarkCoreIncludes.h"
#include "GazeEstimation.h"

#include <SequenceCapture.h>
#include <Visualizer.h>
#include <VisualizationUtils.h>
#include "RecorderOpenFace.h"
#include "RecorderOpenFaceParameters.h"

#define INFO_STREAM( stream ) \
std::cout << stream << std::endl

#define WARN_STREAM( stream ) \
std::cout << "Warning: " << stream << std::endl

#define ERROR_STREAM( stream ) \
std::cout << "Error: " << stream << std::endl

static void printErrorAndAbort(const std::string & error)
{
	std::cout << error << std::endl;
	abort();
}

#define FATAL_STREAM( stream ) \
printErrorAndAbort( std::string( "Fatal error: " ) + stream )

std::vector<std::string> get_arguments(int argc, char **argv)
{

	std::vector<std::string> arguments;

	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(std::string(argv[i]));
	}
	return arguments;
}

int main(int argc, char **argv)
{

	std::vector<std::string> arguments = get_arguments(argc, argv);
	//move some unnecessary outputs
    arguments.emplace_back("-nobadaligned");
    arguments.emplace_back("-nosimalign");
    arguments.emplace_back("-nohogalign");
    arguments.emplace_back("-no2Dfp");
    arguments.emplace_back("-noaus");
    arguments.emplace_back("-nopdmparams");
    arguments.emplace_back("-noeyepoints");

	// no arguments: output usage
	if (arguments.size() == 1)
	{
		std::cout << "For command line arguments see:" << std::endl;
		std::cout << " https://github.com/TadasBaltrusaitis/OpenFace/wiki/Command-line-arguments";
		return 0;
	}

	int skip_frame = 1;
	for (size_t i = 0; i < arguments.size(); ++i){
		if (arguments[i].compare("-skip_frame") == 0)
		{
			skip_frame = std::stoi(arguments[i + 1]);
			if(skip_frame < 1){
				skip_frame = 1;
			}
			break;
		}
	}

	LandmarkDetector::FaceModelParameters det_parameters(arguments);

	// The modules that are being used for tracking
	LandmarkDetector::CLNF face_model(det_parameters.model_location);
	if (!face_model.loaded_successfully)
	{
		std::cout << "ERROR: Could not load the landmark detector" << std::endl;
		return 1;
	}

	if (!face_model.eye_model)
	{
		std::cout << "WARNING: no eye model found" << std::endl;
	}

	// Open a sequence
	Utilities::SequenceCapture sequence_reader;

	// A utility for visualizing the results (show just the tracks)
	Utilities::Visualizer visualizer(true, false, false, false);

	// Tracking FPS for visualization
	Utilities::FpsTracker fps_tracker;
	fps_tracker.AddFrame();

	int sequence_number = 0;

	while (true) // this is not a for loop as we might also be reading from a webcam
	{

		// The sequence reader chooses what to open based on command line arguments provided
		if (!sequence_reader.Open(arguments))
			break;

		INFO_STREAM("Device or file opened");
		cv::Mat rgb_image = sequence_reader.GetNextFrame();

		INFO_STREAM("Starting tracking");
		int frame_index = 0;
		while (!rgb_image.empty()) // this is not a for loop as we might also be reading from a webcam
		{
			frame_index ++;
			if(frame_index % skip_frame != 0){
				// Grabbing the next frame in the sequence
				rgb_image = sequence_reader.GetNextFrame();
				continue;
			}
			Utilities::RecorderOpenFaceParameters recording_params(arguments, false, false,
			sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);
			if (!face_model.eye_model)
			{
				recording_params.setOutputGaze(false);
			}
			
			std::string in_filename = sequence_reader.name;
			std::string path = in_filename.substr(0, in_filename.find_last_of("."));
    		std::string suffix = in_filename.substr(in_filename.find_last_of("."), in_filename.length());
    		in_filename = path + "_" + std::to_string(frame_index) + suffix;
    		std::cout << "index of frame that being parsing: " << frame_index << std::endl;
			Utilities::RecorderOpenFace open_face_rec(in_filename, recording_params, arguments);

			// Reading the images
			cv::Mat_<uchar> grayscale_image = sequence_reader.GetGrayFrame();

			// The actual facial landmark detection / tracking
			bool detection_success = LandmarkDetector::DetectLandmarksInVideo(rgb_image, face_model, det_parameters, grayscale_image);

			// Gaze tracking, absolute gaze direction
			cv::Point3f gazeDirection0(0, 0, -1);
			cv::Point3f gazeDirection1(0, 0, -1);
			cv::Vec2f gaze_angle(0, 0);

			// If tracking succeeded and we have an eye model, estimate gaze
			if (detection_success && face_model.eye_model)
			{
				GazeAnalysis::EstimateGaze(face_model, gazeDirection0, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, true);
				GazeAnalysis::EstimateGaze(face_model, gazeDirection1, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, false);
				gaze_angle = GazeAnalysis::GetGazeAngle(gazeDirection0, gazeDirection1);
			}

			// Work out the pose of the head from the tracked model
			cv::Vec6d pose_estimate = LandmarkDetector::GetPose(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);

			// Keeping track of FPS
			fps_tracker.AddFrame();

			// Displaying the tracking visualizations
			visualizer.SetImage(rgb_image, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);
			visualizer.SetObservationLandmarks(face_model.detected_landmarks, face_model.detection_certainty, face_model.GetVisibilities());
			visualizer.SetObservationPose(pose_estimate, face_model.detection_certainty);
			visualizer.SetObservationGaze(gazeDirection0, gazeDirection1, LandmarkDetector::CalculateAllEyeLandmarks(face_model), LandmarkDetector::Calculate3DEyeLandmarks(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy), face_model.detection_certainty);
			visualizer.SetFps(fps_tracker.GetFPS());

			// Setting up the recorder output
			open_face_rec.SetObservationLandmarks(face_model.detected_landmarks, face_model.GetShape(sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy),
				face_model.params_global, face_model.params_local, face_model.detection_certainty, face_model.detection_success);
			open_face_rec.SetObservationPose(pose_estimate);
			open_face_rec.SetObservationGaze(gazeDirection0, gazeDirection1, gaze_angle, LandmarkDetector::CalculateAllEyeLandmarks(face_model), LandmarkDetector::Calculate3DEyeLandmarks(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy));
			open_face_rec.WriteObservation();

			open_face_rec.SetObservationVisualization(visualizer.GetVisImage());
			open_face_rec.WriteObservationTracked();
			open_face_rec.Close();

			// Grabbing the next frame in the sequence
			rgb_image = sequence_reader.GetNextFrame();
		}

		// Reset the model, for the next video
		face_model.Reset();
		sequence_reader.Close();
		sequence_number++;

	}
	return 0;
}

