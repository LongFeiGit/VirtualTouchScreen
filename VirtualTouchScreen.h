#ifndef VirtualTouchScreen_H
#define VirtualTouchScreen_H

#include <QtWidgets/QMainWindow>
#include <Windows.h>
#include <QPoint>
#include <QPointF>
#include <QVector>
#include <QMutex>
#include "util_pipeline.h"

class GestureThread;
class GestureAlgos;
class TouchInputEmulator;
class ConfigDialog;

class VirtualTouchScreen : public QMainWindow
{
	Q_OBJECT
	friend class GestureThread;
	friend class ConfigDialog;
public:
	explicit VirtualTouchScreen(QWidget *parent = 0);
	~VirtualTouchScreen();
	enum {OFFSET_X = 200, OFFSET_Y = 100, 
		SCALE_FACTOR_x100 = 100};
	enum Hand {THUMB = 0, INDEX, POINTS};
public slots:
	void showMenu();
	void showHelp();
	void onMoveIndex();
	void onMoveThumb();
	void onSwipe(BYTE code);
	//touch screen slots
	void onTouchDown(const QPoint &ptThumb, const QPoint &ptIndex);
	void onTouchDown(const QPoint &ptIndex);
	void onTouchUp(const QPoint &ptThumb, const QPoint &ptIndex);
	void onTouchUp(const QPoint &ptIndex);

private:
	void setupActions();
	void loadSettings();
	void saveSettings();
	GestureThread *gestureThread;
	GestureAlgos *gestureAlgos;
	QString pointerIconPath;
	QPoint offset;
	float scaleFactor;
	ConfigDialog *config;
	QVector<QPointF> handSkeletonPoints_;
	QMutex skeletonPointMutex_;
	TouchInputEmulator *touch_;
};

#endif // VirtualTouchScreen_H
