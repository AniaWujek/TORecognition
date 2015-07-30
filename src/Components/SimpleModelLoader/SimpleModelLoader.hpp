/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#ifndef SIMPLEMODELLOADER_HPP_
#define SIMPLEMODELLOADER_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace SimpleModelLoader {

/*!
 * \class SimpleModelLoader
 * \brief SimpleModelLoader processor class.
 *
 *
 */
class SimpleModelLoader: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	SimpleModelLoader(const std::string & name = "SimpleModelLoader");

	/*!
	 * Destructor
	 */
	virtual ~SimpleModelLoader();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	// Input data streams

	// Output data streams
	Base::DataStreamOut<std::vector<cv::Mat> > out_models_imgs;
	Base::DataStreamOut<std::vector<std::string> > out_models_names;

	// Handlers

	// Properties


	// Handlers
	void loadModels();

	/// Load a single model from file indicated by function parameter.
	void loadSingleModel(std:: string filename_, std::string name_);

	std::vector<cv::Mat> models_imgs;
	std::vector<std::string> models_names;

	/// Sets load_model_flag when the used presses button.
	void onLoadModelButtonPressed();

	/// Flag used for loading models.
	bool load_model_flag;

	/// Loads image from file.
	bool loadImage(const std::string filename_, cv::Mat & image_);

};

} //: namespace SimpleModelLoader
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("SimpleModelLoader", Processors::SimpleModelLoader::SimpleModelLoader)

#endif /* SIMPLEMODELLOADER_HPP_ */
