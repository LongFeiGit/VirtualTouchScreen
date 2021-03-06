#pragma once

#include <QPointF>
#include <QSize>
#include "opencv2/video/tracking.hpp"

class VirtualTouchScreen;

class GestureAlgos
{
public:
	GestureAlgos();
	static GestureAlgos* instance() {
		static GestureAlgos instance;
		return &instance;
	}
	//algorithms parameters
	void setScreenSize(const QSize &screen) {
		screen_ = screen;
	}
	void setImageSize(const QSize &image) {
		image_ = image;
	}
	QPointF imageCenter() const {
		return QPointF(image_.width()/2.0, image_.height()/2.0);
	}
	QSize imageSize() const {
		return image_;
	}
	void setCorrectionFactors(qreal scaleFactor, const QPointF &offset) {
		scaleFactor_ = scaleFactor;
		offset_ = offset;
	}
	void setMainWindow(const VirtualTouchScreen *w) {
		mainWnd_ = w;
	}
	//transforms to screen coordinates
	int imageToScreen(QPointF &pt);
	//filters
	int filterKalman(QPointF &ptThumb, QPointF &ptIndex);
	void filterLowPass(qreal &depthThumb, qreal &depthIndex);
	//detects which finger touches the virtual touch screen
	//the OS decides which gesture has been made
	enum TouchType {NONE = 0, 
		INDEX_DOWN = 0x01, THUMB_DOWN = 0x02, DOUBLE_DOWN = 0x03,
		INDEX_UP = 0x10, THUMB_UP = 0x20, DOUBLE_UP = 0x30,
		INDEX_UP_THUMB_DOWN = 0x12, INDEX_DOWN_THUMB_UP = 0x21};
	int isTouch(qreal depthThumb, qreal depthIndex);
private:
	int initKalman();
	struct BiquadState {
		unsigned int index;
		double mem_in[2];
		double mem_out[2];
	};
	double biquad(BiquadState *state, double in);
	void initBiquad();
	enum SosMat {NB_BIQUADS = 5};
	double sos_mat_[SosMat::NB_BIQUADS][6];
	double gain_;
	BiquadState biquadState[2][SosMat::NB_BIQUADS];//two parallel biquad filters
	QSize screen_;
	QSize image_;
	qreal scaleFactor_;
	QPointF offset_;
	cv::KalmanFilter KF_;
	cv::Mat_<float> measurement_;
	const VirtualTouchScreen *mainWnd_;
	GestureAlgos(const GestureAlgos&);
	GestureAlgos& operator=(const GestureAlgos&);
	~GestureAlgos() {};
};