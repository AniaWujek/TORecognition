/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#include <memory>
#include <string>

#include "SimpleModelLoader.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace SimpleModelLoader {

SimpleModelLoader::SimpleModelLoader(const std::string & name) :
		Base::Component(name)  {

}

SimpleModelLoader::~SimpleModelLoader() {
}

void SimpleModelLoader::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("out_models_imgs", &out_models_imgs);
	registerStream("out_models_names", &out_models_names);
	// Register handlers
	registerHandler("loadModels", boost::bind(&SimpleModelLoader::loadModels, this));

	registerHandler("Load models", boost::bind(&SimpleModelLoader::onLoadModelButtonPressed, this));

}

bool SimpleModelLoader::onInit() {

	return true;
}

bool SimpleModelLoader::onFinish() {
	return true;
}

bool SimpleModelLoader::onStop() {
	return true;
}

bool SimpleModelLoader::onStart() {
	return true;
}

void SimpleModelLoader::onLoadModelButtonPressed(){
	CLOG(LDEBUG) << "onLoadModelButtonPressed";
	load_model_flag = true;
}

void SimpleModelLoader::loadSingleModel(std::string filename_, std::string name_){
	CLOG(LTRACE) << "loadSingleModel";
	cv::Mat model_img;

	if ( loadImage(filename_, model_img) ) {
		std::vector<cv::KeyPoint> model_keypoints;
		cv::Mat model_descriptors;

		// Add to database.
		models_imgs.push_back(model_img);
		models_names.push_back(name_);
		CLOG(LNOTICE) << "Successfull load of model (" << models_names.size()-1 <<"): "<<models_names[models_names.size()-1];
	}//: if
}

bool SimpleModelLoader::loadImage(const std::string filename_, cv::Mat & image_) {
	CLOG(LTRACE) << "loadImage";
	try {
	        image_ = cv::imread( filename_ );
		return true;
	} catch (...) {
		CLOG(LWARNING) << "Could not load image from file " << filename_;
		return false;
	}
}

void SimpleModelLoader::loadModels() {

    loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/dilmah_ceylon_lemon.jpg", "dilmah ceylon lemon");
	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_earl_grey_classic.jpg", "lipton earl grey classic");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_earl_grey_lemon.jpg", "lipton earl grey lemon");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_green_tea_citrus.jpg", "lipton green tea citrus");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/lipton_tea_lemon.jpg", "lipton tea lemon");
//	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/twinings_earl_grey.jpg", "twinings earl grey");
//	loadSingleModel("/home/tkornuta/discode_ecovi/DCL/TORecognition/data/ahmad_daarjeling.png", "ahmad daarjeling");
	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/herbapol_mieta.png", "herbapol mieta");
	loadSingleModel("/home/awujek1/DCL/Ecovi/data/tea_covers/loyd.jpg", "loyd");

	out_models_imgs.write(models_imgs);
	out_models_names.write(models_names);

}



} //: namespace SimpleModelLoader
} //: namespace Processors
