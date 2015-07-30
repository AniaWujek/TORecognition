/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#include <memory>
#include <string>

#include "DescriptorExtractor.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace DescriptorExtractor {

DescriptorExtractor::DescriptorExtractor(const std::string & name) :
		Base::Component(name) ,
		prop_filename("prop_filename", std::string("")),
		prop_read_on_init("prop_read_on_init", true),
		prop_extractor_type("prop_extractor_type", 0) {
	registerProperty(prop_filename);
	registerProperty(prop_read_on_init);
	registerProperty(prop_extractor_type);

}

DescriptorExtractor::~DescriptorExtractor() {
}

void DescriptorExtractor::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_descriptors", &out_descriptors);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&DescriptorExtractor::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool DescriptorExtractor::onInit() {

	return true;
}

bool DescriptorExtractor::onFinish() {
	return true;
}

bool DescriptorExtractor::onStop() {
	return true;
}

bool DescriptorExtractor::onStart() {
	return true;
}

void DescriptorExtractor::setDescriptorExtractor(){
	CLOG(LDEBUG) << "setDescriptorExtractor";
	// Check current extractor type.
	if (current_extractor_type == prop_extractor_type)
		return;

	// Set matcher.
	switch(prop_extractor_type) {
		case 1:
			extractor = cv::DescriptorExtractor::create("SURF");
			CLOG(LNOTICE) << "Using SURF descriptor";
			break;
		case 2:
			extractor = cv::DescriptorExtractor::create("BRIEF");
			CLOG(LNOTICE) << "Using BRIEF descriptor";
			break;
		case 3:
			extractor = cv::DescriptorExtractor::create("BRISK");
			CLOG(LNOTICE) << "Using BRISK descriptor";
			break;
		case 4:
			extractor = cv::DescriptorExtractor::create("ORB");
			CLOG(LNOTICE) << "Using ORB descriptor";
			break;
		case 5:
			extractor = cv::DescriptorExtractor::create("FREAK");
			CLOG(LNOTICE) << "Using FREAK descriptor";
			break;
		case 0 :
		default:
			extractor = cv::DescriptorExtractor::create("SIFT");
			CLOG(LNOTICE) << "Using SIFT descriptor";
			break;
	}//: switch
	// Remember current extractor type.
	current_extractor_type = prop_extractor_type;

	// Reload the model.
	load_model_flag = true;
}

void DescriptorExtractor::onNewImage() {

    CLOG(LTRACE) << "onNewImage";
    try {

        setDescriptorExtractor();

    } catch(...) {
		CLOG(LERROR) << "onNewImage failed";
	}//: catch

}



} //: namespace DescriptorExtractor
} //: namespace Processors
