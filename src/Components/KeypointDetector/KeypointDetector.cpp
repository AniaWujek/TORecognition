/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#include <memory>
#include <string>

#include "KeypointDetector.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace KeypointDetector {

KeypointDetector::KeypointDetector(const std::string & name) :
		Base::Component(name) ,
		prop_filename("filename", std::string("")),
		prop_read_on_init("read_on_init", true),
		prop_detector_type("keypoint_detector_type", 0), //ok
		prop_extractor_type("descriptor_extractor_type", 0),
		prop_matcher_type("descriptor_matcher_type", 0),
		prop_returned_model_number("returned_model_number", 0),
		prop_recognized_object_limit("recognized_object_limit", 1) {
	registerProperty(prop_filename);
	registerProperty(prop_read_on_init);
	registerProperty(prop_detector_type);
	registerProperty(prop_extractor_type);
	registerProperty(prop_matcher_type);
	registerProperty(prop_returned_model_number);
	registerProperty(prop_recognized_object_limit);

}

KeypointDetector::~KeypointDetector() {
}

void KeypointDetector::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_scene_keypoints", &out_scene_keypoints);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&KeypointDetector::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool KeypointDetector::onInit() {

    // Initialize detector.
	current_detector_type = -1;
	setKeypointDetector();
	if (prop_read_on_init)
		load_model_flag = true;
	else
		load_model_flag = false;
	return true;
}

bool KeypointDetector::onFinish() {
	return true;
}

bool KeypointDetector::onStop() {
	return true;
}

bool KeypointDetector::onStart() {
	return true;
}

void KeypointDetector::setKeypointDetector(){
	CLOG(LDEBUG) << "setKeypointDetector";
	// Check current detector type.
	if (current_detector_type == prop_detector_type)
		return;

	// Set detector.
	switch(prop_detector_type) {
		case 1:
			detector = cv::FeatureDetector::create("STAR");
			CLOG(LNOTICE) << "Using STAR detector";
			break;
		case 2:
			detector = cv::FeatureDetector::create("SIFT");
			CLOG(LNOTICE) << "Using SIFT detector";
			break;
		case 3:
			detector = cv::FeatureDetector::create("SURF");
			CLOG(LNOTICE) << "Using SURF detector";
			break;
		case 4:
			detector = cv::FeatureDetector::create("ORB");
			CLOG(LNOTICE) << "Using ORB detector";
			break;
		case 5:
			detector = cv::FeatureDetector::create("BRISK");
			CLOG(LNOTICE) << "Using BRISK detector";
			break;

		case 6:
			detector = cv::FeatureDetector::create("MSER");
			CLOG(LNOTICE) << "Using MSER detector";
			break;
		case 7:
			detector = cv::FeatureDetector::create("GFTT");
			CLOG(LNOTICE) << "Using GFTT detector";
			break;
		case 8:
			detector = cv::FeatureDetector::create("HARRIS");
			CLOG(LNOTICE) << "Using HARRIS detector";
			break;
		case 9:
			detector = cv::FeatureDetector::create("Dense");
			CLOG(LNOTICE) << "Using Dense detector";
			break;
		case 10:
			detector = cv::FeatureDetector::create("SimpleBlob");
			CLOG(LNOTICE) << "Using SimpleBlob detector";
			break;
		case 0 :
		default:
			detector = cv::FeatureDetector::create("FAST");
			CLOG(LNOTICE) << "Using FAST detector";
			break;
	}//: switch
	// Remember current detector type.
	current_detector_type = prop_detector_type;

	// Reload the model.
	load_model_flag = true;
}

bool KeypointDetector::extractFeatures(const cv::Mat image_, std::vector<cv::KeyPoint> & keypoints_) {
	CLOG(LTRACE) << "extractFeatures";
        cv::Mat gray_img;

	try {
		// Transform to grayscale - if requred.
		if (image_.channels() == 1)
			gray_img = image_;
		else
			cvtColor(image_, gray_img, cv::COLOR_BGR2GRAY);

		// Detect the keypoints.
		detector->detect( gray_img, keypoints_ );

		return true;
	} catch (...) {
		CLOG(LWARNING) << "Could not extract features from image";
		return false;
	}//: catch
}

void KeypointDetector::loadModels(){
	CLOG(LDEBUG) << "loadModels";

	if (!load_model_flag)
		return;
	load_model_flag = false;

	// Clear database.
	models_imgs.clear();
	models_keypoints.clear();
	//models_descriptors.clear();
	models_names.clear();

	// Load single model - for now...
//	loadSingleModel(prop_filename, "c3po-ultra-model");

	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/dilmah_ceylon_lemon.jpg", "dilmah ceylon lemon");
	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_earl_grey_classic.jpg", "lipton earl grey classic");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_earl_grey_lemon.jpg", "lipton earl grey lemon");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_green_tea_citrus.jpg", "lipton green tea citrus");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_tea_lemon.jpg", "lipton tea lemon");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/twinings_earl_grey.jpg", "twinings earl grey");
//	loadSingleModel("/home/tkornuta/discode_ecovi/DCL/TORecognition/data/ahmad_daarjeling.png", "ahmad daarjeling");
	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/herbapol_mieta.png", "herbapol mieta");
	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/loyd.jpg", "loyd");

}

bool KeypointDetector::loadImage(const std::string filename_, cv::Mat & image_) {
	CLOG(LTRACE) << "loadImage";
	try {
	        image_ = cv::imread( filename_ );
		return true;
	} catch (...) {
		CLOG(LWARNING) << "Could not load image from file " << filename_;
		return false;
	}
}

void KeypointDetector::loadSingleModel(std::string filename_, std::string name_){
	CLOG(LTRACE) << "loadSingleModel";
	cv::Mat model_img;

	if ( loadImage(filename_, model_img) ) {
		std::vector<cv::KeyPoint> model_keypoints;
		extractFeatures(model_img, model_keypoints);

		// Add to database.
		models_imgs.push_back(model_img);
		models_keypoints.push_back(model_keypoints);
		models_names.push_back(name_);
		CLOG(LNOTICE) << "Successfull load of model (" << models_names.size()-1 <<"): "<<models_names[models_names.size()-1];
	}//: if
}



void KeypointDetector::onNewImage() {

    CLOG(LTRACE) << "onNewImage";
    try {

        // Change keypoint detector type (if required).
        setKeypointDetector();

        // Re-load the model - extract features from model.
		loadModels();

		std::vector<cv::KeyPoint> scene_keypoints;

		// Load image containing the scene.
		cv::Mat scene_img = in_img.read();

		// Extract features from scene.
		extractFeatures(scene_img, scene_keypoints);
		CLOG(LINFO) << "Scene features: " << scene_keypoints.size();

		std::vector< std::vector<cv::KeyPoint> > out_keypoints;
		out_keypoints.push_back(scene_keypoints);
		for(int i = 0; i < models_keypoints.size(); ++i) {
            out_keypoints.push_back(models_keypoints[i]);
		}
		out_scene_keypoints.write(out_keypoints);




    } catch (...) {
        CLOG(LERROR) << "onNewImage failed";
    }

}



} //: namespace KeypointDetector
} //: namespace Processors
